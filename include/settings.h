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

// Numero di luci
#define LIGHTS                              8

// Bytes della EEPROM da leggere
#define EEPROM_BYTES                        LIGHTS

// WIFI
#define HOSTNAME                            "scroksbox.local"

// HTTP Server
#define SERVER_PORT                         80

#endif //SETTINGS_H
