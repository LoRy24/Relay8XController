//
// Created by lorenzo on 09/06/2025.
//

#ifndef SETTINGS_H
#define SETTINGS_H

// PINS delle luci
#define LIGHT_1                             27
#define LIGHT_2                             26
#define LIGHT_3                             25
#define LIGHT_4                             33
#define LIGHT_5                             32
#define LIGHT_6                             13
#define LIGHT_7                             12
#define LIGHT_8                             23

// Pins dei pulsanti
#define LIGHT_BUTTON_1                      26
#define LIGHT_BUTTON_2                      27
#define LIGHT_BUTTON_3                      28
#define LIGHT_BUTTON_4                      29
#define LIGHT_BUTTON_5                      30
#define LIGHT_BUTTON_6                      31
#define LIGHT_BUTTON_7                      33
#define LIGHT_BUTTON_8                      36

// Numero di luci
#define LIGHTS                              8

// Bytes della EEPROM da leggere
#define EEPROM_BYTES                        LIGHTS

// WIFI
#define HOSTNAME                            "scroksbox.local"

// HTTP Server
#define SERVER_PORT                         80

#endif //SETTINGS_H
