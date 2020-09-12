
#pragma once

#include "Emulator.h"

class Controller
{
public:

    void run(Emulator* emulator);

protected:

    Emulator* myEmulator;
};

