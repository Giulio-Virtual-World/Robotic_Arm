/*

  2025
  Author: Giulio Segre
  www.giulio-virtual-world.com
  
*/


#include <Stepper.h>
#include <Servo.h>

// Joystick
#define VRX_PIN  A0 // X pin
#define VRY_PIN  A1 // Y pin
#define SW_PIN 7  // Pulsante

// Costanti e variabili
bool baseCalibrated = false;
bool spallaCalibrated = false;
bool gomitoCalibrated = false;
int baseCalibrationTimeout = 0; // contatore dei passi dello stepper base
int spallaCalibrationTimeout = 0; // contatore dei passi dello stepper spalla
int gomitoCalibrationTimeout = 0; // contatore dei passi dello stepper gomito
const int baseSensor = 6;
const int spallaSensor = 5;
const int gomitoSensor = 4;
const int polsoPin = 3;
int pinzaPin = 2;
int basePosition = 0;
int spallaPosition = 0;
int gomitoPosition = 0;

// Joystick config
bool pinzaAperta = true;  // Stato della pinza

// Stepper config
const int stepsPerRevolution = 2048;
const int rpm = 5;
const float stepsPerDegree = (float)stepsPerRevolution / 360.0; // Passi per 1° degli stepper

// Stepper
Stepper base(stepsPerRevolution, 38, 42, 40, 44);
Stepper spalla1(stepsPerRevolution, 31, 35, 33, 37);
Stepper spalla2(stepsPerRevolution, 30, 34, 32, 36);
Stepper gomito1(stepsPerRevolution, 23, 27, 25, 29);
Stepper gomito2(stepsPerRevolution, 22, 26, 24, 28);

// Servo
Servo polso;
Servo pinza;

// Funzioni
void reduceBaseStepperPower() { // Disabilita alimentazione stepper base quando non in uso
  digitalWrite(38, LOW);
  digitalWrite(40, LOW); 
  digitalWrite(42, LOW);
  digitalWrite(44, LOW);
}

void reduceSpallaStepperPower() { // Disabilita alimentazione stepper spalla quando non in uso
  digitalWrite(30, LOW);
  digitalWrite(31, LOW); 
  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  digitalWrite(34, LOW);
  digitalWrite(35, LOW); 
  digitalWrite(36, LOW);
  digitalWrite(37, LOW);
}

void reduceGomitoStepperPower() { // Disabilita alimentazione stepper gomito quando non in uso
  digitalWrite(22, LOW);
  digitalWrite(23, LOW); 
  digitalWrite(24, LOW);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);
  digitalWrite(27, LOW); 
  digitalWrite(28, LOW);
  digitalWrite(29, LOW);
}

void baseCalibration() { // Ricerca punto 0 della base
  if (digitalRead(baseSensor) != HIGH) {
    base.step(1);
    baseCalibrationTimeout++;
  } else {
    Serial.println("Punto 0 della base trovato con successo!");
    baseCalibrated = true;
    delay(500);
    reduceBaseStepperPower();
  }
  return;
}

void spallaCalibration() { // Ricerca punto 0 della spalla
  if (digitalRead(spallaSensor) != HIGH) {
    spalla1.step(1);
    spalla2.step(-1);
    spallaCalibrationTimeout++;
  } else {
    Serial.println("Punto 0 della spalla trovato con successo!");
    spallaCalibrated = true;
    delay(500);
    while (digitalRead(spallaSensor) == HIGH) {
      spalla1.step(-1);
      spalla2.step(1);
    }
    reduceSpallaStepperPower();
  }
  return;
}

void gomitoCalibration() {
  if (digitalRead(gomitoSensor) != HIGH) {
    gomito1.step(-1);
    gomito2.step(1);
    gomitoCalibrationTimeout++;
  } else {
    Serial.println("Punto 0 del gomito trovato con successo!");
    gomitoCalibrated = true;
    delay(500);
    while (digitalRead(gomitoSensor) == HIGH) {
      gomito1.step(1);
      gomito2.step(-1);
    }
    reduceGomitoStepperPower();
  }
}

void defaultPosition() { // Porta tutti i motori stepper/servo alla loro posizione di default
  // Prima li porto qualche grado sopra e poi li abbasso, per sforzare meno i motori
  polso.write(100);
  pinza.write(45);
  rotateSpalla(40);
  reduceSpallaStepperPower(); // Spengo lo stepper "spalla" prima di muovere il gomito, per diminuire la richiesta di corrente
  rotateGomito(60);
  reduceGomitoStepperPower(); // Spengo lo stepper "gomito" prima di muovere la base, per diminuire la richiesta di corrente
  rotateBase(-10);
  delay(1000);
  polso.write(130);
  pinza.write(50);
  delay(1000);
  polso.write(135); // Mi assicuro di non sforzare il motore
  
  // Altri motori

}

// ------------------------------ Movimenti ------------------------------
int DegreesToSteps(int gradi) {
  return round(gradi * stepsPerDegree);
}

void rotateBase(int n) {
  int newPosition = basePosition + n; // Calcola la posizione richiesta
  
  if (newPosition <= 360 && newPosition >= -360) {
    int passi = DegreesToSteps(n);
    base.step(passi);
    basePosition = newPosition; // Aggiorna la posizione
    
    Serial.print("Base ruotata di ");
    Serial.print(n);
    Serial.print("°. Posizione attuale: ");
    Serial.println(basePosition);

  } else {
    Serial.println("\n<< ⚠ Non puoi ruotare il braccio più di 360°, potrebbe intrecciare i fili! >>\n");
    Serial.print("Posizione attuale: ");
    Serial.print(basePosition);
    Serial.print("°, posizione richiesta: ");
    Serial.print(newPosition);
    Serial.println("°");
  }
  reduceBaseStepperPower(); // Disabilita alimentazione dopo il movimento
  return;
}

void rotateSpalla(int n) {
  int newPosition = spallaPosition + n; // Calcola la posizione richiesta
  
  if (newPosition <= 150 && newPosition >= 10) {
    int passi = DegreesToSteps(n);
    int direzione = (passi > 0) ? 1 : -1;
    int passiAssoluti = abs(passi); // Valore assoluto per il loop

    for (int i=0; i<passiAssoluti; i++) {
      spalla1.step(-direzione);
      spalla2.step(direzione);
    }
    spallaPosition = newPosition; // Aggiorna la posizione
    
    Serial.print("Spalla ruotata di ");
    Serial.print(n);
    Serial.print("°. Posizione attuale: ");
    Serial.println(spallaPosition);

  } else {
    Serial.println("\n<< ⚠ L'angolo della spalla deve rimanere tra 10° e 150°! >>\n");
    Serial.print("Posizione attuale: ");
    Serial.print(spallaPosition);
    Serial.print("°, posizione richiesta: ");
    Serial.print(newPosition);
    Serial.println("°");
  }
  reduceSpallaStepperPower(); // Disabilita alimentazione dopo il movimento
  return;
}

void rotateGomito(int n) {
  int newPosition = gomitoPosition + n; // Calcola la posizione richiesta

  if (newPosition <= 150 && newPosition >= 5) {
    int passi = DegreesToSteps(n);
    int direzione = (passi > 0) ? 1 : -1;
    int passiAssoluti = abs(passi); // Valore assoluto per il loop

    for (int i=0; i<passiAssoluti; i++) {
      gomito1.step(direzione);
      gomito2.step(-direzione);
    }
    gomitoPosition = newPosition; // Aggiorna la posizione
    
    Serial.print("Gomito ruotato di ");
    Serial.print(n);
    Serial.print("°. Posizione attuale: ");
    Serial.println(gomitoPosition);

  } else {
    Serial.println("\n<< ⚠ L'angolo del gomito deve rimanere tra 5° e 150°! >>\n");
    Serial.print("Posizione attuale: ");
    Serial.print(gomitoPosition);
    Serial.print("°, posizione richiesta: ");
    Serial.print(newPosition);
    Serial.println("°");
  }
  reduceGomitoStepperPower(); // Disabilita alimentazione dopo il movimento
  return;
}

void armHeight(int n) {
  int spallaNewPosition = spallaPosition + n; // Calcola la posizione richiesta della spalla
  int gomitoNewPosition = gomitoPosition + n; // Calcola la posizione richiesta della base

  if (spallaNewPosition <= 150 && spallaNewPosition >= 5 && gomitoNewPosition <= 150 && gomitoNewPosition >= 5) {
    int passi = DegreesToSteps(n);
    int direzione = (passi > 0) ? 1 : -1;
    int passiAssoluti = abs(passi); // Valore assoluto per il loop

    for (int i=0; i<passiAssoluti; i++) {
      spalla1.step(-direzione);
      spalla2.step(direzione);
      gomito1.step(direzione);
      gomito2.step(-direzione);
    }
    spallaPosition = spallaNewPosition; // Aggiorna la posizione della spalla
    gomitoPosition = gomitoNewPosition; // Aggiorna la posizione del gomito
    
    Serial.print("Spalla e Gomito ruotati di ");
    Serial.print(n);
    Serial.print("°. Posizioni attuali: ");
    Serial.println(spallaPosition);
    Serial.println(", ");
    Serial.println(gomitoPosition);

  } else if (spallaNewPosition <= 150 && spallaNewPosition >= 5) {
    rotateSpalla(n);
  } else if (gomitoNewPosition <= 150 && gomitoNewPosition >= 5) {
    rotateGomito(n);
  } else {
    Serial.println("\n<< ⚠ Il movimento richiesto non è nel range consentito da 5° a 150° >>\n");
  }
  reduceSpallaStepperPower();
  reduceGomitoStepperPower();
}

// ------------------------------ Gestione comandi seriale ------------------------------
void handleSerialControl() {
  if (Serial.available() > 0) {
    String comando = Serial.readString();
    comando.trim(); // Rimuove spazi e caratteri di fine riga
    
    if (comando.length() < 2) {
      Serial.println("Comando troppo corto! Formato: [motore][gradi]");
      Serial.println("Esempio: 1360 (base 360°), 2-90 (spalla -90°)");
      return;
    }
    
    // Estrae il numero del motore (prima cifra)
    int motore = comando.charAt(0) - '0';
        
    // Estrae i gradi (resto della stringa)
    String gradiStr = comando.substring(1);
    int gradi = gradiStr.toInt();
    
    // Verifica che il motore sia valido
    if (motore < 1 || motore > 5) {
      Serial.println("Motore non valido! Usa: 1=base, 2=spalla, 3=gomito, 4=polso, 5=pinza");
      return;
    }
    
    switch(motore) {
      case 1: // Base
        rotateBase(gradi);
        break;
        
      case 2: // Spalla
        rotateSpalla(gradi);
        break;
        
      case 3: // Gomito
        rotateGomito(gradi);
        break;
        
      case 4: // Polso
        if (gradi >= 0 && gradi <= 180) {
          polso.write(180-gradi); // Per invertire i gradi
          Serial.print("Polso mosso a ");
          Serial.print(gradi);
          Serial.println("°");
        } else {
          Serial.println("Angolo polso non valido! Range: 0-180°");
        }
        break;
      
      case 5: // Pinza
        if (gradi >= 0 && gradi <= 90) {
          pinza.write(gradi);
          Serial.print("Pinza mossa a ");
          Serial.print(gradi);
          Serial.println("°");
        } else {
          Serial.println("Angolo pinza non valido! Range: 0-90°");
        }
        break;
    }
  }
}

// ------------------------------ Gestione Joystick ------------------------------
void handleJoystickControl() {
  // Leggi i valori del joystick
  int xValue = analogRead(VRX_PIN);
  int yValue = analogRead(VRY_PIN);
  
  // Centra i valori (512 è il centro nominale)
  int xOffset = xValue - 512;
  int yOffset = yValue - 512;

  const int deadZone = 150; // Zona morta per evitare movimenti indesiderati

  // Controllo ASSE X -> Spalla e Gomito
  if (abs(xOffset) > deadZone) {
    int n = map(abs(xOffset), deadZone, 512, 1, 20); // Da 1 a 20 gradi
    if (xOffset > 0) {
      armHeight(-n);  // Su
    } else {
      armHeight(n);  // Giù
    }
    delay(50); // Piccola pausa per evitare movimenti troppo rapidi
  }
  
  // Controllo ASSE Y -> Base
  if (abs(yOffset) > deadZone) {
    int n = map(abs(yOffset), deadZone, 512, 1, 40); // Da 1 a 40 gradi
    if (yOffset > 0) {
      rotateBase(-n);  // Sinistra
    } else {
      rotateBase(n);  // Destra
    }
    delay(50); // Piccola pausa per evitare movimenti troppo rapidi
  }

  // Controllo PULSANTE -> Pinza
  int buttonReading = digitalRead(SW_PIN);
  
  if (buttonReading == 0) {
    pinzaAperta = !pinzaAperta;
    if (pinzaAperta) {
      pinza.write(50);  // Apri pinza
      Serial.println("Pinza APERTA");
    } else {
      pinza.write(0);  // Chiudi pinza
      Serial.println("Pinza CHIUSA");
    }
    delay(300); // Delay per dare il tempo di prendere o lasciare oggetti
  }
}

// ------------------------------ Setup e Loop ------------------------------
void setup() {
  Serial.begin(9600);

  base.setSpeed(rpm);
  spalla1.setSpeed(rpm);
  spalla2.setSpeed(rpm);
  gomito1.setSpeed(rpm);
  gomito2.setSpeed(rpm);

  polso.attach(polsoPin);
  pinza.attach(pinzaPin);

  pinMode(baseSensor, INPUT);
  pinMode(spallaSensor, INPUT);
  pinMode(gomitoSensor, INPUT);
  pinMode(SW_PIN, INPUT_PULLUP); // Pulsante Joystick

  polso.write(100);
  pinza.write(50);

  Serial.println("  | Powered by: Giulio Segre\n  | Web Site: www.giulio-virtual-world.com\n  | Discord Server: https://discord.gg/5g3JzfbMjv \n\n");
  Serial.println("Braccio impostato alla posizione di default");
  Serial.println("Avvio calibrazione");
  
  while (baseCalibrated == false) {
    if (baseCalibrationTimeout <= stepsPerRevolution/2*3) { //Se supera 1.5 giri
      baseCalibration();
    } else {
      Serial.println("ERR: Calibrazione non riuscita");
      Serial.println("\n >> Non è stato trovato il punto 0 della base, il finecorsa potrebbe non funzionare correttamente.");
      Serial.println(" >> Se hai bisogno di assistenza contatta assistenza@giulio-virtual-world.com o apri un ticket nel nel server discord: https://discord.gg/5g3JzfbMjv");
      reduceBaseStepperPower();
      while (true) {delay(100);} // Loop per fermare il codice
    }
  }

  baseCalibrationTimeout = 0; // Resetta il contatore per la calibrazione successiva

  while (spallaCalibrated == false) {
    if (spallaCalibrationTimeout <= stepsPerRevolution) { //Se supera 1 giro
      spallaCalibration();
    } else {
      Serial.println("ERR: Calibrazione non riuscita");
      Serial.println("\n >> Non è stato trovato il punto 0 della spalla, il finecorsa potrebbe non funzionare correttamente.");
      Serial.println(" >> Se hai bisogno di assistenza contatta assistenza@giulio-virtual-world.com o apri un ticket nel nel server discord: https://discord.gg/5g3JzfbMjv");
      reduceSpallaStepperPower();
      while (true) {delay(100);} // Loop per fermare il codice
    }
  }

  spallaCalibrationTimeout = 0; // Resetta il contatore per la calibrazione successiva

  while (gomitoCalibrated == false) {
    if (gomitoCalibrationTimeout <= stepsPerRevolution) { //Se supera 1 giro
      gomitoCalibration();
    } else {
      Serial.println("ERR: Calibrazione non riuscita");
      Serial.println("\n >> Non è stato trovato il punto 0 del gomito, il finecorsa potrebbe non funzionare correttamente.");
      Serial.println(" >> Se hai bisogno di assistenza contatta assistenza@giulio-virtual-world.com o apri un ticket nel nel server discord: https://discord.gg/5g3JzfbMjv");
      reduceGomitoStepperPower();
      while (true) {delay(100);} // Loop per fermare il codice
    }
  }

  gomitoCalibrationTimeout = 0; // Resetta il contatore per la calibrazione successiva

  defaultPosition();
  Serial.println("\n<< Braccio calibrato con successo e pronto all'uso! >>\n");
  
  Serial.println("═══════════════════════════════════════════════════════");
  Serial.println("  CONTROLLO SIMULTANEO ATTIVO");
  Serial.println("═══════════════════════════════════════════════════════");
  Serial.println("\n💻 MONITOR SERIALE:");
  Serial.println("  Formato: [motore][gradi]");
  Serial.println("  1=base, 2=spalla, 3=gomito, 4=polso, 5=pinza");
  Serial.println("  Esempio: 1360 (base +360°), 2-90 (spalla -90°)");
  Serial.println("\n🕹️  JOYSTICK:");
  Serial.println("  Asse X: Spalla e Gomito");
  Serial.println("  Asse Y: Base");
  Serial.println("  Pulsante: Apri/Chiudi Pinza");
  Serial.println("\n📱 APP:");
  Serial.println("  In attesa di implementazione");
  Serial.println("\n═══════════════════════════════════════════════════════");
  Serial.println("Pronto per ricevere comandi da qualsiasi sorgente!\n");
}

void loop() {
  // Gestisce continuamente sia seriale che joystick
  handleSerialControl();
  handleJoystickControl();
  
  // Aggiungere handleAppControl() quando implementata la modalità C
}
