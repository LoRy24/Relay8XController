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
    // Stato
    bool state = true;

    // Oggetto risposta
    AsyncWebServerResponse* response;

    // Verifica che sia presente l'ID della luce
    if (!request->hasParam("lightId")) {
        response = request->beginResponse(404, "text/json", R"({"message":"Light's ID is missing!"})");
        state = false;
        goto outValidation;
    }

    // Prova a convertire e caricare l'ID della luce
    try {
        *lightId = request->getParam("lightId")->value().toInt();
    } catch (...) {
        response = request->beginResponse(500, "text/json", R"({"message":"Invalid lightId!"})");
        state = false;
        goto outValidation;
    }

    // Vedi che sia compreso tra 1 e il limite
    if (*lightId < 1 || *lightId > LIGHTS) {
        response = request->beginResponse(500, "text/json", R"({"message":"Invalid lightId!"})");
        state = false;
    }

    outValidation:

    if (!state) {
        // Aggiunge gli header e manda la risposta
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", "GET, POST");
        response->addHeader("Access-Control-Allow-Headers", "Content-Type");
        request->send(response);
    }

    // Ritorna lo stato
    return state;
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

    // Oggetto risposta con headers
    const auto response = request->beginResponse(200, "text/json", buffer);
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");

    // Risponde
    request->send(response);
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

    // Oggetto risposta con headers
    const auto response = request->beginResponse(200, "text/json", buffer);
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");

    // Risponde
    request->send(response);
}

void handleNotFound(AsyncWebServerRequest* request) {
    // Oggetto risposta con headers
    const auto response = request->beginResponse(200, "text/json", R"({"message":"Resource not found!"})");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");

    // Risponde
    request->send(response);
}

void handleShutdown(AsyncWebServerRequest* request) {
    // Spegne tutte le luci
    for (int i = 0; i < LIGHTS; i++)
        setLightState(i, LOW);

    // Oggetto risposta con headers
    const auto response = request->beginResponse(200, "text/json", R"({"message":"Shutdown executed!"})");
    response->addHeader("Access-Control-Allow-Origin", "*");
    response->addHeader("Access-Control-Allow-Methods", "GET, POST");
    response->addHeader("Access-Control-Allow-Headers", "Content-Type");

    // Risponde
    request->send(response);
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

    httpServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(SPIFFS, "/panel.html", "text/html");
    });

    // Not found status
    httpServer.onNotFound([](AsyncWebServerRequest *request) {
        handleNotFound(request);
    });

    // Inizializza
    httpServer.begin();
}

#pragma endregion
