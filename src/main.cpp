//
// By Lorenzo Rocca
//

//
// Macro
//

#pragma region Macro

// Libs
#include <Arduino.h>
#include <EEPROM.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

// Secrets
#include "secrets.h"

// PINS delle luci
#define LIGHT_1                             27
#define LIGHT_2                             26
#define LIGHT_3                             25
#define LIGHT_4                             33
#define LIGHT_5                             32
#define LIGHT_6                             13
#define LIGHT_7                             12
#define LIGHT_8                             23

// Numero di luci
#define LIGHTS                              8

// Bytes della EEPROM da leggere
#define EEPROM_BYTES                        LIGHTS

// WIFI
#define HOSTNAME                            "scroksbox.local"

// HTTP Server
#define SERVER_PORT                         80

#pragma endregion

//
// Valori Globali
//

#pragma region Globals

// Stati delle luci
int lightsStates[8] = {
    LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};

// Pins delle luci
int lightsPins[8] = {
    LIGHT_1, LIGHT_2, LIGHT_3, LIGHT_4, LIGHT_5, LIGHT_6, LIGHT_7, LIGHT_8
};

// Server HTTP
AsyncWebServer httpServer(SERVER_PORT);

#pragma endregion

//
// Utils
//

#pragma region Utils

/**
 * Converte un valore digitale in "acceso" e "spento", per rendere tutto umano.
 *
 * @param value Il valore da convertire in testo
 * @return Il relativo in testo
 */
const char* convertSignalToText(const int value) {
    return value == 0 ? "off" : "on";
}

/**
 * Converte un valore testuale di "acceso" e "spento" in un segnale digitale utilizzabile tramite
 * @code digitalWrite(...)@endcode.
 *
 * @param value Il valore testuale da convertire in segnale digitale
 * @return Il segnale digitale rispettivo
 */
int convertStringToSignal(const char* value) {
    return value == "off" ? LOW : HIGH;
}

/**
 * Ottieni l'ID della luce e fai tutti i controlli in merito. Serve per ottenere in modo sicuro il valore, in quanto C è
 * molto raw.
 *
 * @param request La richiesta dalla quale estrapolare i dati
 * @param lightId Dove caricare l'ID della luce
 * @return Se i controlli sono passati
 */
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
// Operazioni di controllo
//

#pragma region Controlling

/**
 * Cambia lo stato di una luce (ovvero del relay che la controlla) impostandone uno nuovo. Lo salva inoltre nella memoria
 * EEPROM per renderlo persistente agli spegnimenti.
 *
 * @param number Il numero della luce (relay) al quale cambiare stato
 * @param state Il nuovo stato da impostare
 */
void setLightState(const int number, const int state) {
    // Applicalo in anteprima
    digitalWrite(lightsPins[number], !state);

    // Impostalo nel registro
    lightsStates[number] = state;

    // Salvataggio nella EEPROM
    EEPROM.write(number, state);
    EEPROM.commit();
}

/**
 * Inverte lo stato di una luce.
 *
 * @param number Il numero del relay o della luce
 */
void toggleLightState(const int number) {
    setLightState(number, !lightsStates[number]);
}

#pragma endregion

//
// Server HTTP
//

#pragma region HTTP Server

/**
 * Gestisce le richieste sulla pagina root del server.
 *
 * @param request La richiesta async
 */
void handleRoot(AsyncWebServerRequest* request) {
    // Risponde
    request->send(200, "text/json", R"({"status":"ok"})");
}

/**
 * Gestisce le richieste inviate per ottenere informazioni sullo stato della luce. Può essere utilizzato qualora si volesse
 * integrare dei sensori che indichino lo stato attuale del relay (luce).
 *
 * @param request La richiesta async
 */
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

/**
 * Gestisce le richieste intente a cambiare lo stato di una luce, invertendolo rispetto all'attuale e salvandone il
 * nuovo stato nella memoria.
 *
 * @param request La richiesta async
 */
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

/**
 * Gestisce le richieste completamente invalide.
 *
 * @param request La richiesta async
 */
void handleNotFound(AsyncWebServerRequest* request) {
    // Risponde
    request->send(404, "text/plain", R"({"message":"Resource not found!"})");
}

/**
 * Gestisce le richieste per spegnere tutti i led. Sia per condizioni di emergenza che per comodità.
 *
 * @param request La richiesta async
 */
void handleShutdown(AsyncWebServerRequest* request) {
    // Spegne tutte le luci
    for (int i = 0; i < LIGHTS; i++)
        setLightState(i, LOW);

    // Invia la risposta
    request->send(200, "text/json", R"({"message":"Shutdown executed!"})");
}

/**
 * Avvia il server HTTP configurando tutti gli handler per le varie funzionalità.
 */
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

//
// NIC WiFi
//

#pragma region NIC

/**
 * Configura la connessione wireless.
 */
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
