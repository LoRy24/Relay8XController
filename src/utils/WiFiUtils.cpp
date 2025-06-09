//
// Created by lorenzo on 09/06/2025.
//

// Libs
#include "Utils.h"

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
}

#pragma endregion
