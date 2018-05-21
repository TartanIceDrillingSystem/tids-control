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

#include "MeltingSystem.h"

#include <chrono>

namespace tids {

#define CAP_MOTOR_ANGLE_OPEN 45
#define CAP_MOTOR_ANGLE_CLOSED -45

#define TEMPERATURE_MIN_C 110
#define TEMPERATURE_MAX_C 120

MeltingSystem::MeltingSystem(PowerController* powerController, DS3218 *capMotor, MLX90614 *thermometer) {
    this->powerController = powerController;
    this->capMotor = capMotor;
    this->thermometer = thermometer;
    this->regulateTemperatureThreadShouldCancel = true;
}

MeltingSystem::~MeltingSystem() {
    this->stop();
}

// Open melting chamber cap
int MeltingSystem::openCap() {
    this->capMotor->setAngle(CAP_MOTOR_ANGLE_OPEN);
}

// Close melting chamber cap
int MeltingSystem::closeCap() {
    this->capMotor->setAngle(CAP_MOTOR_ANGLE_CLOSED);
}

// Start heater and chiller and adjust based on thermometer
int MeltingSystem::start() {
    // Return if the temperature regulation thread already exists
    if (this->regulateTemperatureThreadShouldCancel) {
        return -1;
    }

    // Turn on heater and chiller
    this->powerController->setChillerRelayState(PowerController::STATE::ON);
    this->powerController->setHeaterRelayState(PowerController::STATE::ON);

    // Reset cancellation token
    this->regulateTemperatureThreadShouldCancel = false;
    // Start temperature regulation on new thread
    this->regulateTemperatureThread = std::thread(&MeltingSystem::regulateTemperature, this);
}

// Stop heater and chiller
int MeltingSystem::stop() {
    // Cancel and join temperature regulation thread
    this->regulateTemperatureThreadShouldCancel = true;
    this->regulateTemperatureThread.join();

    // Turn off heater and chiller
    this->powerController->setChillerRelayState(PowerController::STATE::OFF);
    this->powerController->setHeaterRelayState(PowerController::STATE::OFF);
}

// Continuously turn the heater on/off to regulate evaporation temperature
void MeltingSystem::regulateTemperature() {
    // Run until cancellation token
    while (!this->detectionShouldCancel) {
        // Get temperature of induction chamber
        float temperature = this->thermometer->getObjectTemperature();

        // If temperature is below the minimum, turn the heater on
        if (temperature < TEMPERATURE_MIN_C) {
            this->powerController->setHeaterRelayState(PowerController::STATE::OFF);
        }

        // If temperature is above the maximum, turn the heater off
        else if (temperature > TEMPERATURE_MAX_C) {
            this->powerController->setHeaterRelayState(PowerController::STATE::ON);
        }

        // Repeat every second
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

} /* namespace tids */
