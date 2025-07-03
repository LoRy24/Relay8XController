//
// Created by lorenzo on 09/06/2025.
//

// Libs
#include "../../include/Management.h"

//
// Registri
//

#pragma region Registers

// Stati delle luci
int lightsStates[8] = {
    LOW, LOW, LOW, LOW, LOW, LOW, LOW, LOW
};

// Pins delle luci
int lightsPins[8] = {
    LIGHT_1, LIGHT_2, LIGHT_3, LIGHT_4, LIGHT_5, LIGHT_6, LIGHT_7, LIGHT_8
};

// Pins dei pulsanti
int lightsButtonsPins[8] {
    LIGHT_BUTTON_1, LIGHT_BUTTON_2, LIGHT_BUTTON_3, LIGHT_BUTTON_4, LIGHT_BUTTON_5, LIGHT_BUTTON_6, LIGHT_BUTTON_7, LIGHT_BUTTON_8
};

#pragma endregion

//
// Impl
//

#pragma region Impl

void setLightState(const int number, const int state) {
    // Applicalo in anteprima
    digitalWrite(lightsPins[number], !state);

    // Impostalo nel registro
    lightsStates[number] = state;

    // Salvataggio nella EEPROM
    EEPROM.write(number, state);
    EEPROM.commit();
}

void toggleLightState(const int number) {
    setLightState(number, !lightsStates[number]);
}

#pragma endregion
