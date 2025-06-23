//
// By Lorenzo Rocca
//

//
// Macro
//

#pragma region Macro

// Libs
#include <Arduino.h>
#include <SPIFFS.h>

// Secrets
#include "../include/Utils.h"
#include "../include/settings.h"
#include "../include/HttpManagementServer.h"
#include "../include/Management.h"

#pragma endregion

//
// Routine
//

#pragma region Routine

/**
 * Funzione di setup
 */
void setup() {
    // Init comunicazione seriale
    Serial.begin(115200);

    // Messaggio di benvenuto
    Serial.println("|---------------------------------------------------------|");
    Serial.println("");
    Serial.println("Comunicazione seriale per Relay 8X Controller inizializzata!");
    Serial.println("Made by Lorenzo Rocca :3");
    Serial.println("");
    Serial.println("|---------------------------------------------------------|");

    // Init luci
    Serial.println("");
    Serial.println("Avvio il caricamento dello stato delle luci");

    // EEPROM
    EEPROM.begin(EEPROM_BYTES);

    // SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("Errore nell'avvio di SPIFFS");
        return;
    }

    // Init delle luci
    for (int i = 0; i < LIGHTS; i++) {
        pinMode(lightsPins[i], OUTPUT);
        lightsStates[i] = EEPROM.read(i);
        digitalWrite(lightsPins[i], !lightsStates[i]);
    }

    // Caricamento completato luci inizio WiFi
    Serial.println("Caricamento stato luci completato!");
    Serial.println("");
    Serial.println("|---------------------------------------------------------|");
    Serial.println("");
    Serial.println("Avvio della connessione alla rete");

    // WiFi
    setupWiFi();

    // Completato e avvia HTTP server
    Serial.println("Connessione alla rete completata!");
    Serial.println("");
    Serial.println("|---------------------------------------------------------|");
    Serial.println("");
    Serial.println("Avvio server HTTP");

    // HTTP Server
    launchHTTPServer();

    Serial.println("Server HTTP avviato!");
    Serial.println("");
    Serial.println("|---------------------------------------------------------|");
}

/**
 * Loop
 */
void loop() {
    // TODO Pulsanti
}

#pragma endregion
