# Braccio Robotico Stampato in 3D e Controllato Multi-Piattaforma
Questo progetto include tutto il necessario per costruire, programmare e controllare un braccio robotico interamente stampato in 3D, sia tramite joystick sia da remoto attraverso un bot Discord e un’interfaccia web. È stato sviluppato completamente da zero, integrando modellazione 3D, Arduino, Node.js, interfaccia web (HTML/CSS/JS/PHP), bot Discord e Webhook Discord.<br>
<a href="https://www.giulio-virtual-world.com/projects/robotic-arm/" target="_blank">➥ Visualizza il progetto completo</a>

<img src="https://www.giulio-virtual-world.com/projects/projects/robotic-arm/img/1.jpg" style="border-radius: 5px;">
<br>

# Aspetti Principali
<ul>
  <li>Braccio interamente realizzato in stampa 3D.</li>
  <li>Firmware Arduino dedicato al controllo del braccio.</li>
  <li>Backend Node.js per gestire comunicazioni e API.</li>
  <li>Bot Discord per il controllo remoto tramite messaggi.</li>
  <li>Possibilità di integrazione con interfacce web.</li>
</ul>
<img src="https://www.giulio-virtual-world.com/projects/projects/robotic-arm/img/Diagramma_funzionamento.png" style="border-radius: 5px; width: 60%;">

<br>

# Requisiti
<ul>
  <li><a href="https://amzn.to/3XupfNZ" target="_blank"><img alt="Arduino compatibile" src="https://img.shields.io/badge/🔗-Arduino_compatibile-007ec6"></a> (es. Arduino Uno o Mega)</li>
  <li><a href="https://amzn.to/3JZZGRP" target="_blank"><img alt="x1 Breadboard" src="https://img.shields.io/badge/🔗-x1_|_Breadboard-007ec6"></a></li>
  <li><a href="https://amzn.to/480kFfp" target="_blank"><img alt="x1 Modulo Joystick" src="https://img.shields.io/badge/🔗-x1_|_Modulo_Joystick-007ec6"></a></li>
  <li><a href="https://amzn.to/41p0DJT" target="_blank"><img alt="x5 Driver ULN2003" src="https://img.shields.io/badge/🔗-x5_|_Driver_ULN2003-007ec6"></a></li>
  <li><a href="https://amzn.to/41p0DJT" target="_blank"><img alt="x5 Stepper Motor 28BYJ-48" src="https://img.shields.io/badge/🔗-x5_|_Stepper_Motor_28BYJ‒48-007ec6"></a></li>
  <li><a href="https://amzn.to/469RmWM" target="_blank"><img alt="x3 Finecorsa a linguetta" src="https://img.shields.io/badge/🔗-x3_|_Finecorsa_a_linguetta-007ec6"></a></li>
  <li><a href="https://amzn.to/3XvNDPj" target="_blank"><img alt="x2 Servo MG90S" src="https://img.shields.io/badge/🔗-x2_|_Servo_MG90S-007ec6"></a></li>
  <li><a href="https://amzn.to/3QX2XEP" target="_blank"><img alt="x3 Resistenze 10KΩ" src="https://img.shields.io/badge/🔗-x3_|_10KΩ_Resistor-007ec6"></a> (pull-down)</li>
  <li><a href="https://amzn.to/40EHeDS" target="_blank"><img alt="Cavetti jumper" src="https://img.shields.io/badge/🔗-Cavetti_Jumper-007ec6"></a></li>
  <li><a href="https://makerworld.com/it/models/2000062-robotic-arm-v1-0" target="_blank"><img alt="Parti stampate in 3D" src="https://img.shields.io/badge/🔗-Parti_stampate_in_3D-00b000"></a></li>
  <li><a href="https://github.com/Giulio-Virtual-World/Robotic_Arm#requisiti" target="_blank"><img alt="Node.js installato" src="https://img.shields.io/badge/Node.js_installato-8A2BE2"></a></li>
  <li><a href="https://github.com/Giulio-Virtual-World/Robotic_Arm#requisiti" target="_blank"><img alt="Server o PC attivo per controllo remoto" src="https://img.shields.io/badge/Server_o_PC_attivo_per_il_controllo_remoto-8A2BE2"></a></li>
</ul>

<br>

# Come Utilizzarlo
<ol>
  <li><b>Stampare in 3D le parti:</b> tutte le parti sono progettate per essere semplici da assemblare.</li>
  <li><b>Assemblare il braccio:</b> utilizzare i file stampati e i motori indicati nel progetto.</li>
  <li><b>Caricare il firmware su Arduino:</b> aprire <a href="source/robotic-arm.ino" target="_blank">robotic-arm.ino</a>, configurare la porta e caricare.</li>
  <li><b>Scaricare il file <a href="source/index.js" target="_blank">index.js</a></b> (bot Discord)</li>
  <li><b>Installare le dipendenze Node.js:</b> eseguire `npm install discord.js dotenv serialport` nella cartella del progetto.</li>
  <li><b>Configurare il Bot Discord:</b> inserire il token e gli ID richiesti nel file di configurazione <a href="source/index.js" target="_blank">index.js</a> e avviare il bot in locale.</li>
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
