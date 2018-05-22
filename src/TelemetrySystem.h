/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef TELEMETRYSYSTEM_H
#define TELEMETRYSYSTEM_H

#include <atomic>
#include <thread>

#include "HX711.h"
#include "ISNAILVC10.h"

namespace tids {

class TelemetrySystem {
private:
    ISNAILVC10 *currentSensor;
    HX711 *weightOnBitSensor;

    float current;
    float weightOnBit;

    std::thread telemetryThread;
    std::atomic<bool> telemetryThreadShouldCancel;

public:
    TelemetrySystem(ISNAILVC10 *currentSensor, HX711 *weightOnBitSensor);
    virtual ~TelemetrySystem();

    // Start updating telemetry
    int start();

    // Stop updating telemetry
    int stop();

    // Get current in amps
    float getCurrent();

    // Get weight on bit in kg
    float getWeightOnBit();

private:
    // Update telemetry values
    void updateTelemetry();
};

} /* namespace tids */

#endif /* TELEMETRYSYSTEM_H */
