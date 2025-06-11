//
// Created by lorenzo on 09/06/2025.
//

#ifndef HTTPMANAGEMENTSERVER_H
#define HTTPMANAGEMENTSERVER_H

//
// Libs
//

#pragma region Libs

// Libs
#include <ESPAsyncWebServer.h>

// Internal
#include "settings.h"
#include "Management.h"
#include "Utils.h"

#pragma endregion

//
// Registers
//

#pragma region Registers

// Registri
extern AsyncWebServer httpServer;

#pragma endregion

//
// Utils
//

#pragma region Utils

/**
 * Ottieni l'ID della luce e fai tutti i controlli in merito. Serve per ottenere in modo sicuro il valore, in quanto C è
 * molto raw.
 *
 * @param request La richiesta dalla quale estrapolare i dati
 * @param lightId Dove caricare l'ID della luce
 * @return Se i controlli sono passati
 */
bool doLightsValueChecksAndFetchLightID(AsyncWebServerRequest *request, int* lightId);

#pragma endregion

//
// Requests
//

#pragma region Requests

/**
 * Gestisce le richieste sulla pagina root del server.
 *
 * @param request La richiesta async
 */
void handleRoot(AsyncWebServerRequest* request);

/**
 * Gestisce le richieste inviate per ottenere informazioni sullo stato della luce. Può essere utilizzato qualora si volesse
 * integrare dei sensori che indichino lo stato attuale del relay (luce).
 *
 * @param request La richiesta async
 */
void handleFetchLightStatus(AsyncWebServerRequest* request);

/**
 * Gestisce le richieste intente a cambiare lo stato di una luce, invertendolo rispetto all'attuale e salvandone il
 * nuovo stato nella memoria.
 *
 * @param request La richiesta async
 */
void handleLightToggle(AsyncWebServerRequest* request);

/**
 * Gestisce le richieste completamente invalide.
 *
 * @param request La richiesta async
 */
void handleNotFound(AsyncWebServerRequest* request);

/**
 * Gestisce le richieste per spegnere tutti i led. Sia per condizioni di emergenza che per comodità.
 *
 * @param request La richiesta async
 */
void handleShutdown(AsyncWebServerRequest* request);

#pragma endregion

//
// Routine
//

#pragma region Routine

/**
 * Avvia il server HTTP configurando tutti gli handler per le varie funzionalità.
 */
void launchHTTPServer();

#pragma endregion

#endif //HTTPMANAGEMENTSERVER_H
