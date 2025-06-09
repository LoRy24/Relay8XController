//
// Created by lorenzo on 09/06/2025.
//

#ifndef MANAGEMENT_H
#define MANAGEMENT_H

//
// Libs
//

#pragma region Libs

// Libs
#include <Arduino.h>
#include <EEPROM.h>

// Internal
#include "settings.h"

#pragma endregion

//
// Global Registers
//

#pragma region Global Registers

// Registri esterni
extern int lightsStates[8];
extern int lightsPins[8];

#pragma endregion

//
// Management Functions
//

#pragma region Management Functions

/**
 * Cambia lo stato di una luce (ovvero del relay che la controlla) impostandone uno nuovo. Lo salva inoltre nella memoria
 * EEPROM per renderlo persistente agli spegnimenti.
 *
 * @param number Il numero della luce (relay) al quale cambiare stato
 * @param state Il nuovo stato da impostare
 */
void setLightState(int number, int state);

/**
 * Inverte lo stato di una luce.
 *
 * @param number Il numero del relay o della luce
 */
void toggleLightState(int number);

#pragma endregion

#endif //MANAGEMENT_H
