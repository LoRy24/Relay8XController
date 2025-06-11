//
// Created by lorenzo on 09/06/2025.
//

#ifndef UTILS_H
#define UTILS_H

// Inclusioni
#include <WiFi.h>

// Interne
#include "secrets.h"
#include "settings.h"

// IP
extern String ipv4;

/**
 * Configura la connessione wireless.
 */
void setupWiFi();

/**
 * Converte un valore digitale in "acceso" e "spento", per rendere tutto umano.
 *
 * @param value Il valore da convertire in testo
 * @return Il relativo in testo
 */
const char* convertSignalToText(int value);

/**
 * Converte un valore testuale di "acceso" e "spento" in un segnale digitale utilizzabile tramite
 * @code digitalWrite(...)@endcode.
 *
 * @param value Il valore testuale da convertire in segnale digitale
 * @return Il segnale digitale rispettivo
 */
int convertStringToSignal(const char* value);

#endif //UTILS_H
