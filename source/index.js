/*

  2025
  Author: Giulio Segre
  www.giulio-virtual-world.com

  This is the code for a discord bot that interfaces with the robotic arm
  
*/


require('dotenv').config();
const { Client, GatewayIntentBits } = require('discord.js');
const { SerialPort } = require('serialport');
const { ReadlineParser } = require('@serialport/parser-readline');

// ==================== CONFIGURAZIONE ====================
const CONFIG = {
    discord: {
        token: process.env.DISCORD_TOKEN, // Your discord bot token in the .env file
        channelId: '1234567890123456789', // The ID of the channel where the bot should read/write messages
        botId: '1234567890123456789' // Your discord bot ID
    },
    serial: {
        port: 'COM3', // The port where Arduino is
        baudRate: 9600
    },
    motors: {
        1: { min: -360, max: 360, name: 'Base', isRelative: true },
        2: { min: 10, max: 150, name: 'Spalla', isRelative: true },
        3: { min: 5, max: 150, name: 'Gomito', isRelative: true },
        4: { min: 0, max: 180, name: 'Polso', isRelative: false },
        5: { min: 0, max: 90, name: 'Pinza', isRelative: false }
    }
};

// ==================== STATO GLOBALE ====================
const state = {
    positions: { // default positions (must be the same as the one in the .ino file)
        base: 0,
        spalla: 0,
        gomito: 0,
        polso: 100,
        pinza: 50
    },
    isArduinoReady: false,
    serialPort: null,
    parser: null,
    controlChannel: null,
    lastDiscordMessage: '', // Per evitare duplicati
    lastDiscordMessageTime: 0
};

// ==================== CLIENT DISCORD ====================
const client = new Client({
    intents: [
        GatewayIntentBits.Guilds,
        GatewayIntentBits.GuildMessages,
        GatewayIntentBits.MessageContent
    ]
});

// ==================== GESTIONE SERIALE ====================
class SerialManager {
    static init() {
        try {
            state.serialPort = new SerialPort({
                path: CONFIG.serial.port,
                baudRate: CONFIG.serial.baudRate
            });

            state.parser = state.serialPort.pipe(new ReadlineParser({ delimiter: '\n' }));

            this.setupEventHandlers();
            console.log('✅ Connessione seriale inizializzata');
        } catch (error) {
            console.error('❌ Impossibile aprire la porta seriale:', error.message);
        }
    }

    static setupEventHandlers() {
        state.serialPort.on('open', () => {
            console.log('✅ Connessione seriale aperta');
            DiscordManager.sendMessage('🔌 **Connessione con Arduino stabilita!**');
        });

        state.parser.on('data', (data) => {
            console.log('Arduino:', data);
            this.handleArduinoMessage(data);
        });

        state.serialPort.on('error', (err) => {
            console.error('❌ Errore seriale:', err.message);
            DiscordManager.sendMessage(`❌ **Errore di connessione:** ${err.message}`);
        });

        state.serialPort.on('close', () => {
            console.log('🔌 Connessione seriale chiusa');
            state.isArduinoReady = false;
            DiscordManager.sendMessage('⚠️ **Connessione con Arduino persa!**');
        });
    }

    static handleArduinoMessage(data) {
        // Aggiorna sempre il tracciamento posizioni
        PositionTracker.update(data);

        if (!state.controlChannel || !data.trim()) return;

        // Messaggi da inviare SEMPRE su Discord
        const criticalMessages = [
            'pronto all\'uso',
            'configurato per il controllo',
            'Calibrazione non riuscita',
            'ERR:',
            'Connessione'
        ];

        // Messaggi da ignorare SEMPRE (spam del joystick)
        const ignoredMessages = [
            'Pronto per il prossimo comando',
            'Comando non riconosciuto',
            'Pinza APERTA',
            'Pinza CHIUSA',
            'Powered by',
            'Web Site:',
            'Discord Server:',
            'CONTROLLO SIMULTANEO',
            '╔═══',
            '💻 MONITOR',
            '🕹️ JOYSTICK',
            '📱 APP:',
            'Pronto per ricevere comandi'
        ];

        // Segnala quando Arduino è pronto
        if (data.includes('pronto all\'uso') || data.includes('configurato per il controllo')) {
            state.isArduinoReady = true;
            DiscordManager.sendMessage(
                '✅ **Braccio robotico calibrato e pronto!**\n\n' +
                'Puoi inviare comandi nel formato: `[motore][gradi]`\n' +
                'Esempio: `1360` per ruotare la base di 360°\n\n' +
                'Usa `!help` per ulteriori informazioni'
            );
            return;
        }

        // Ignora messaggi noti
        if (ignoredMessages.some(msg => data.includes(msg))) {
            return;
        }

        // Invia solo messaggi critici o movimenti da comandi seriali
        const isMovementMessage = data.includes('ruotata') || data.includes('ruotato') || 
                                   data.includes('mosso a') || data.includes('mossa a');
        
        const isCritical = criticalMessages.some(msg => data.includes(msg));

        // Invia su Discord solo se:
        // 1. È un messaggio critico
        // 2. È un movimento E non è uno spam recente
        if (isCritical || (isMovementMessage && this.shouldSendMovementMessage(data))) {
            DiscordManager.sendMessage(`🤖 ${data}`);
        }
    }

    static shouldSendMovementMessage(message) {
        const now = Date.now();
        const timeSinceLastMessage = now - state.lastDiscordMessageTime;
        
        // Evita spam: invia solo se è passato almeno 1 secondo dall'ultimo messaggio
        if (timeSinceLastMessage < 1000 && state.lastDiscordMessage === message) {
            return false;
        }

        state.lastDiscordMessage = message;
        state.lastDiscordMessageTime = now;
        return true;
    }

    static sendCommand(command) {
        if (!state.serialPort || !state.serialPort.isOpen) {
            return { success: false, message: 'Porta seriale non disponibile' };
        }

        state.serialPort.write(command + '\n', (err) => {
            if (err) console.error('Errore nell\'invio:', err.message);
        });

        return { success: true };
    }
}

// ==================== TRACCIAMENTO POSIZIONI ====================
class PositionTracker {
    static update(data) {
        const patterns = {
            base: /Base ruotata.*Posizione attuale:\s*(-?\d+)/,
            spalla: /Spalla ruotata.*Posizione attuale:\s*(-?\d+)/,
            gomito: /Gomito ruotato.*Posizione attuale:\s*(-?\d+)/,
            polso: /Polso mosso a\s*(\d+)/,
            pinza: /Pinza mossa a\s*(\d+)/
        };

        for (const [key, pattern] of Object.entries(patterns)) {
            const match = data.match(pattern);
            if (match) {
                state.positions[key] = parseInt(match[1]);
            }
        }

        // Reset posizioni dopo calibrazione
        if (data.includes('Punto 0 della base trovato')) state.positions.base = 0;
        if (data.includes('Punto 0 della spalla trovato')) state.positions.spalla = 0;
        if (data.includes('Punto 0 del gomito trovato')) state.positions.gomito = 0;
    }

    static reset() {
        state.positions = { base: 0, spalla: 0, gomito: 0, polso: 100, pinza: 50 };
    }

    static getEmbed() {
        return {
            color: 0x00ff00,
            title: '📍 Posizioni Attuali',
            fields: [
                { name: '🔵 Base', value: `${state.positions.base}°`, inline: true },
                { name: '🟢 Spalla', value: `${state.positions.spalla}°`, inline: true },
                { name: '🟡 Gomito', value: `${state.positions.gomito}°`, inline: true },
                { name: '🟠 Polso', value: `${state.positions.polso}°`, inline: true },
                { name: '🔴 Pinza', value: `${state.positions.pinza}°`, inline: true }
            ],
            timestamp: new Date()
        };
    }
}

// ==================== VALIDAZIONE COMANDI ====================
class CommandValidator {
    static validate(command) {
        const match = command.match(/^([1-5])(-?\d+)$/);
        
        if (!match) {
            return { 
                valid: false, 
                message: 'Formato comando non valido! Usa: [motore][gradi]\nEsempio: `145` o `2-90`' 
            };
        }

        const motor = parseInt(match[1]);
        const degrees = parseInt(match[2]);
        const config = CONFIG.motors[motor];

        // Ottieni posizione corrente
        const positionKey = ['base', 'spalla', 'gomito', 'polso', 'pinza'][motor - 1];
        const currentPosition = state.positions[positionKey];

        // Calcola posizione finale
        const finalPosition = config.isRelative ? currentPosition + degrees : degrees;

        // Valida range
        if (finalPosition < config.min || finalPosition > config.max) {
            return { 
                valid: false, 
                message: 
                    `⚠️ Movimento non valido per ${config.name}!\n` +
                    `Posizione attuale: ${currentPosition}°\n` +
                    `Movimento richiesto: ${degrees > 0 ? '+' : ''}${degrees}°\n` +
                    `Posizione finale: ${finalPosition}°\n` +
                    `Range consentito: ${config.min}° a ${config.max}°`
            };
        }

        return { 
            valid: true, 
            motor: config.name, 
            degrees,
            currentPosition,
            finalPosition
        };
    }
}

// ==================== GESTIONE DISCORD ====================
class DiscordManager {
    static sendMessage(content) {
        if (state.controlChannel) {
            state.controlChannel.send(content).catch(err => 
                console.error('Errore invio messaggio Discord:', err.message)
            );
        }
    }

    static getHelpEmbed() {
        return {
            color: 0x0099ff,
            title: '🦾 Comandi Braccio Robotico',
            description: 'Controlla il braccio robotico tramite Discord!',
            fields: [
                {
                    name: 'Formato Comandi',
                    value: '`[motore][gradi]`\nEsempio: `145` o `2-90`'
                },
                {
                    name: '🔢 Motori Disponibili',
                    value: 
                        '**1** - Base (range: -360° a +360°) *relativo*\n' +
                        '**2** - Spalla (range: 10° a 150°) *relativo*\n' +
                        '**3** - Gomito (range: 5° a 150°) *relativo*\n' +
                        '**4** - Polso (range: 0° a 180°) *assoluto*\n' +
                        '**5** - Pinza (range: 0° a 90°) *assoluto*'
                },
                {
                    name: '📝 Esempi',
                    value: 
                        '`1360` - Ruota la base di +360°\n' +
                        '`2-45` - Ruota la spalla di -45°\n' +
                        '`4120` - Muovi il polso a 120° (posizione assoluta)\n' +
                        '`550` - Muovi la pinza a 50° (posizione assoluta)'
                },
                {
                    name: '🛠️ Altri Comandi',
                    value: 
                        '`!help` - Mostra questo messaggio\n' +
                        '`!status` - Stato della connessione\n' +
                        '`!position` - Mostra posizioni attuali\n' +
                        '`!reset` - Reset posizioni tracciate'
                }
            ],
            footer: {
                text: 'Movimenti relativi: il valore si aggiunge alla posizione attuale\n' +
                      'Movimenti assoluti: il valore è la posizione finale'
            },
            timestamp: new Date()
        };
    }
}

// ==================== GESTIONE COMANDI ====================
class CommandHandler {
    static async handle(message) {
        const content = message.content.trim().toLowerCase();

        // Comandi speciali
        const commands = {
            '!help': () => message.reply({ embeds: [DiscordManager.getHelpEmbed()] }),
            '!status': () => this.handleStatus(message),
            '!position': () => message.reply({ embeds: [PositionTracker.getEmbed()] }),
            '!reset': () => this.handleReset(message)
        };

        if (commands[content]) {
            commands[content]();
            return;
        }

        // Comando di movimento
        this.handleMovementCommand(message);
    }

    static handleStatus(message) {
        const status = state.serialPort && state.serialPort.isOpen ? '🟢 Connesso' : '🔴 Disconnesso';
        const readyStatus = state.isArduinoReady ? '✅ Pronto' : '⏳ In calibrazione';
        message.reply(`**Stato Arduino:** ${status}\n**Braccio:** ${readyStatus}`);
    }

    static handleReset(message) {
        PositionTracker.reset();
        message.reply('🔄 Posizioni tracciate resettate ai valori di default.');
    }

    static handleMovementCommand(message) {
        const validation = CommandValidator.validate(message.content.trim());
        
        if (!validation.valid) {
            message.reply(validation.message);
            return;
        }

        if (!state.isArduinoReady) {
            message.reply('⏳ Attendi che il braccio finisca la calibrazione!');
            return;
        }

        const result = SerialManager.sendCommand(message.content.trim());
        
        if (result.success) {
            message.react('✅');
        } else {
            message.reply(`❌ ${result.message}`);
        }
    }
}

// ==================== EVENT HANDLERS ====================
client.once('clientReady', () => {
    console.log(`✅ Bot connesso come ${client.user.tag}`);
    
    state.controlChannel = client.channels.cache.get(CONFIG.discord.channelId);
    
    if (state.controlChannel) {
        DiscordManager.sendMessage('🤖 **Bot online!**\nIn attesa di calibrazione Arduino...');
        SerialManager.init();
    } else {
        console.error('❌ Canale non trovato!');
    }
});

client.on('messageCreate', async (message) => {
    if (message.author.id === CONFIG.discord.botId) return;
    if (message.channel.id !== CONFIG.discord.channelId) return;
    
    CommandHandler.handle(message);
});

client.on('error', (error) => {
    console.error('Errore Discord:', error);
});

// ==================== CLEANUP ====================
process.on('SIGINT', () => {
    console.log('\n🛑 Chiusura bot...');
    if (state.serialPort && state.serialPort.isOpen) {
        state.serialPort.close();
    }
    client.destroy();
    process.exit();
});

// ==================== AVVIO ====================
client.login(CONFIG.discord.token);
