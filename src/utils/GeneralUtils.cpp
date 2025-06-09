//
// Created by lorenzo on 09/06/2025.
//

// Libs
#include "Utils.h"

//
// Impl
//

#pragma region Impl

const char* convertSignalToText(const int value) {
    return value == 0 ? "off" : "on";
}

int convertStringToSignal(const char* value) {
    return value == "off" ? LOW : HIGH;
}

#pragma endregion
