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
  <li><a href="https://amzn.to/3XupfNZ" target="_blank">Arduino compatibile (es. Arduino Uno o Mega)</a></li>
  <li><a href="https://amzn.to/3JZZGRP" target="_blank">x1 Breadboard</a></li>
  <li><a href="https://amzn.to/480kFfp" target="_blank">x1 Joystick Module</a></li>
  <li><a href="https://amzn.to/41p0DJT" target="_blank">x5 Driver ULN2003</a></li>
  <li><a href="https://amzn.to/41p0DJT" target="_blank">x5 Stepper Motor 28BYJ-48</a></li>
  <li><a href="https://amzn.to/469RmWM" target="_blank">x3 Finecorsa a linguetta</a></li>
  <li><a href="https://amzn.to/3XvNDPj" target="_blank">x2 Servo MG90S</a></li>
  <li><a href="https://amzn.to/40EHeDS" target="_blank">Jumper wires</a></li>
  <li><a href="https://makerworld.com/it/models/2000062-robotic-arm-v1-0" target="_blank">Parti stampate in 3D</a></li>
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

# Codici sorgente e modelli 3D
<ul>
  <li><a href="source/robotic-arm.ino" target="_blank">Arduino firmware</a></li>
  <li><a href="source/index.js" target="_blank">Discord Bot (node.js)</a></li>
  <li><a href="https://makerworld.com/it/models/2000062-robotic-arm-v1-0" target="_blank">Modello 3D</a></li>
</ul>

<br>
