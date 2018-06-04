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

#include "TelemetrySystem.h"

#include <chrono>

namespace tids {

TelemetrySystem::TelemetrySystem(ISNAILVC10 *currentSensor, HX711 *weightOnBitSensor) {
    this->currentSensor = currentSensor;
    this->weightOnBitSensor = weightOnBitSensor;
    
    float current = -1;
    float weightOnBit = -1;

    this->telemetryThreadShouldCancel = true;
}

TelemetrySystem::~TelemetrySystem() {
    this->stop();
}

// Start updating telemetry
int TelemetrySystem::start() {
    // Return if the telemetry thread already exists
    if (!this->telemetryThreadShouldCancel) {
        return -1;
    }

    // Reset cancellation token
    this->telemetryThreadShouldCancel = false;
    // Start temperature regulation on new thread
    this->telemetryThread = std::thread(&TelemetrySystem::updateTelemetry, this);
}

// Stop updating telemetry
int TelemetrySystem::stop() {
    // Cancel and join telemetry thread
    this->telemetryThreadShouldCancel = true;
    this->telemetryThread.join();
}

// Get current in amps
float TelemetrySystem::getCurrent() {
    return this->current;
}

// Get weight on bit in kg
float TelemetrySystem::getWeightOnBit() {
    return this->weightOnBit;
}

// Update telemetry values
void TelemetrySystem::updateTelemetry() {
    // Run until cancellation token
    while (!this->telemetryThreadShouldCancel) {
        // Get current
        this->current = this->currentSensor->getCurrent();

        // Get weight on bit
        if (this->weightOnBitSensor->isReady()) {
            this->weightOnBit = this->weightOnBitSensor->getWeight();
        }

        // Repeat every .1 seconds
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} /* namespace tids */