//
// Created by lorenzo on 09/06/2025.
//

// Libs
#include "../../include/Utils.h"

//
// Registers
//

#pragma region Registers

// IPv4
String ipv4;

#pragma endregion

//
// Impl
//

#pragma region Impl

void setupWiFi() {
    // Inizia la connessione
    WiFiClass::setHostname(HOSTNAME);
    WiFi.begin(SSID, PASSWORD);

    // Notifica la connessione
    Serial.print("Connessione a ");
    Serial.print(SSID);
    Serial.print(" ");

    // Attendi che il micro-controllore si connetta
    while (WiFiClass::status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // Notifica la connessione
    Serial.println("Connesso!");

    // Salva l'indirizzo
    ipv4 = WiFi.localIP().toString();
}

#pragma endregion
