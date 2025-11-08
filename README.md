# Braccio Robotico Stampato in 3D e Controllato Multi-Piattaforma
Questo progetto raccoglie tutto il necessario per costruire, programmare e controllare a distanza e via joystick un braccio robotico interamente stampato in 3D. È stato sviluppato da zero, combinando modellazione 3D, Arduino, Node.js, un'interfaccia web e un bot Discord.<br>
<a href="https://www.giulio-virtual-world.com/projects/robotic-arm/" target="_blank">➥ Visualizza il progetto completo</a>

<img src="https://www.giulio-virtual-world.com/projects/robotic-arm/img/1.jpg" style="border-radius: 5px;">
<br>

# Funzionalità Principali
<ul>
  <li>Braccio interamente realizzato in stampa 3D.</li>
  <li>Firmware Arduino dedicato al controllo del braccio.</li>
  <li>Backend Node.js per gestire comunicazioni e API.</li>
  <li>Bot Discord per il controllo remoto tramite messaggi.</li>
  <li>Possibilità di integrazione con interfacce web.</li>
</ul>

<br>

# Requisiti
<ul>
  <li>Arduino compatibile (es. Arduino Uno o Mega).</li>
  <li>x1 Breadboard</li>
  <li>x1 Joystick Module</li>
  <li>x1 Modulo MB102</li>
  <li>x5 Driver ULN2003</li>
  <li>x5 Stepper Motor 28BYJ-48</li>
  <li>x3 Finecorsa a linguetta</li>
  <li>x2 Servo MG90S</li>
  <li>Jumper wires</li>
  <li>Parti stampate in 3D.</li>
  <li>Node.js installato.</li>
  <li>Server o PC attivo per il controllo remoto.</li>
</ul>

<br>

# Come Utilizzarlo
<ol>
  <li><b>Stampare in 3D le parti:</b> tutte le parti sono progettate per essere semplici da assemblare.</li>
  <li><b>Assemblare il braccio:</b> utilizzare i file stampati e i motori indicati nel progetto.</li>
  <li><b>Caricare il firmware su Arduino:</b> aprire <a href="source/robotic-arm.ino" target="_blank">robotic-arm.ino</a>, configurare la porta e caricare.</li>
  <li><b>Installare le dipendenze Node.js:</b> eseguire `npm install` nella cartella del progetto.</li>
  <li><b>Configurare il Bot Discord:</b> inserire il token e gli ID richiesti nel file di configurazione <a href="source/index.js" target="_blank">index.js</a> e avviare il bot.</li>
</ol>

<br>

# Licenza e Utilizzo
Il codice è liberamente scaricabile e utilizzabile nei propri progetti. Non è consentito rivenderlo o attribuirsi il merito del lavoro originale. Un credito al progetto originale è sempre gradito.

<br>

# Codici sorgente e modelli 3D:
<ul>
  <li><a href="source/robotic-arm.ino" target="_blank">Arduino firmware</a></li>
  <li><a href="source/index.js" target="_blank">Discord Bot (node.js)</a></li>
</ul>

<br>

⚠ Al momento il modello 3D non è disponibile
