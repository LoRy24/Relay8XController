//
// Created by lorenzo on 09/06/2025.
//

//Libs
#include "../../include/HttpManagementServer.h"

//
// Registers
//

#pragma region Registers

// Server HTTP
AsyncWebServer httpServer(SERVER_PORT);

#pragma endregion

//
// Utils
//

#pragma region Utils

bool doLightsValueChecksAndFetchLightID(AsyncWebServerRequest *request, int* lightId) {
    // Verifica che sia presente l'ID della luce
    if (!request->hasParam("lightId")) {
        request->send(404, "text/json", R"({"message":"Light's ID is missing!"})");
        return false;
    }

    // Prova a convertire e caricare l'ID della luce
    try {
        *lightId = request->getParam("lightId")->value().toInt();
    } catch (...) {
        request->send(500, "text/json", R"({"message":"Invalid lightId!"})");
        return false;
    }

    // Vedi che sia compreso tra 1 e il limite
    if (*lightId < 1 || *lightId > LIGHTS) {
        request->send(500, "text/json", R"({"message":"Invalid lightId!"})");
        return false;
    }

    // Tutto apposto
    return true;
}

#pragma endregion

//
// Requests
//

#pragma region Requests

void handleRoot(AsyncWebServerRequest* request) {
    // Risponde
    request->send(200, "text/json", R"({"status":"ok"})");
}

void handleFetchLightStatus(AsyncWebServerRequest* request) {
    // Carica l'ID della luce
    int lightId = 0;
    if (!doLightsValueChecksAndFetchLightID(request, &lightId)) return;
    lightId--;

    // Prepara la risposta dinamicamente
    char buffer[256] = {};
    sprintf(buffer, R"({"message":"Light state fetched successfully!","state":"%s"})", convertSignalToText(lightsStates[lightId]));

    // Risponde
    request->send(200, "text/json", buffer);
}

void handleLightToggle(AsyncWebServerRequest* request) {
    // Carica l'ID della luce
    int lightId = 0;
    if (!doLightsValueChecksAndFetchLightID(request, &lightId)) return;
    lightId--;

    // Aggiorna lo stato a livello hardware
    toggleLightState(lightId);

    // Prepara la risposta
    char buffer[256] = {};
    sprintf(buffer, R"({"message":"Light toggled successfully!","newState":"%s"})", convertSignalToText(lightsStates[lightId]));

    // Invia la risposta
    request->send(200, "text/json", buffer);
}

void handleNotFound(AsyncWebServerRequest* request) {
    // Risponde
    request->send(404, "text/plain", R"({"message":"Resource not found!"})");
}

void handleShutdown(AsyncWebServerRequest* request) {
    // Spegne tutte le luci
    for (int i = 0; i < LIGHTS; i++)
        setLightState(i, LOW);

    // Invia la risposta
    request->send(200, "text/json", R"({"message":"Shutdown executed!"})");
}

#pragma endregion

//
// Routine
//

#pragma region Routine

void launchHTTPServer() {
    // Root
    httpServer.on("/api/status", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleRoot(request);
    });

    // Reset
    httpServer.on("/api/lights/shutdown", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleShutdown(request);
    });

    // Light status
    httpServer.on("/api/lights/status", HTTP_GET, [](AsyncWebServerRequest* request) {
        handleFetchLightStatus(request);
    });

    // Toggle light
    httpServer.on("/api/lights/toggle", HTTP_POST, [](AsyncWebServerRequest* request) {
        handleLightToggle(request);
    });

    // Not found status
    httpServer.onNotFound([](AsyncWebServerRequest *request) {
        handleNotFound(request);
    });

    // Inizializza
    httpServer.begin();
}

#pragma endregion
