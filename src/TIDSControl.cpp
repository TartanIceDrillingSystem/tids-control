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

#include "TIDSControl.h"

#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>

namespace tids {

#define WEIGHT_ON_BIT_MIN_KG 0.5
#define WEIGHT_ON_BIT_MAX_KG 10.0

#define HOLE_DIAMETER_MM 102.0
#define HOLE_SEPARATION_MM 152.0

#define X_AXIS_LENGTH_MM 1000.0
#define Z_AXIS_LENGTH_MM 2000.0

#define Z_AXIS_MOVE_MM 3.0

TIDSControl::TIDSControl() {
    // Power

    this->powerController = new PowerController(TIDS_POWERCONTROLLER_PIN_RELAYCHILLER_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYDRILLMOTOR_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYHEATER1_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYHEATER2_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYPROXIMITYSENSORS_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYSTEPPERMOTORX_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYSTEPPERMOTORZ_GPIO);
    // Telemetry

    this->currentSensor = new ISNAILVC10(TIDS_CURRENTSENSOR_PIN_ADC);

    this->loadCell = new HX711(TIDS_LOADCELL_PIN_DOUT_GPIO, TIDS_LOADCELL_PIN_PD_SCK_GPIO, -56500.0f);

    this->telemetrySystem = new TelemetrySystem(this->currentSensor, this->loadCell);

    // Drill

    this->drillMotor = new bbbkit::DCMotor(TIDS_DRILLMOTOR_PIN_PWM, 1000, 0.0);

    this->drillEncoder = new MMPEU(TIDS_DRILLENCODER_PIN_A_GPIO,
                                    TIDS_DRILLENCODER_PIN_B_GPIO,
                                    TIDS_DRILLENCODER_PIN_INDEX_GPIO);

    this->drillCurrentSensor = new LTS6NP(TIDS_DRILLCURRENTSENSOR_PIN_ADC);

    this->drillingSystem = new DrillingSystem(this->drillMotor, this->drillEncoder, this->drillCurrentSensor);

    // X-axis

    this->stepperMotorX = new CVD524K(TIDS_STEPPERMOTORX_PIN_PLS_GPIO,
                                        TIDS_STEPPERMOTORX_PIN_DIR_GPIO,
                                        TIDS_STEPPERMOTORX_PIN_AWO_GPIO,
                                        TIDS_STEPPERMOTORX_PIN_CS_GPIO,
                                        TIDS_STEPPERMOTORX_PIN_ALM_GPIO,
                                        TIDS_STEPPERMOTORX_PIN_TIM_GPIO);

    this->proximitySensorXHome = new LJ12A34ZBY(TIDS_PROXIMITYSENSORXHOME_PIN_GPIO);

    this->xAxis = new PositioningAxis(X_AXIS_LENGTH_MM, 3.0, this->stepperMotorX, this->proximitySensorXHome, NULL);

    // Z-axis

    this->stepperMotorZ = new TB6600(TIDS_STEPPERMOTORZ_PIN_PUL_GPIO,
                                        TIDS_STEPPERMOTORZ_PIN_DIR_GPIO,
                                        TIDS_STEPPERMOTORZ_PIN_ENA_GPIO);

    this->proximitySensorZHome = new LJ12A34ZBY(TIDS_PROXIMITYSENSORZHOME_PIN_GPIO);

    this->proximitySensorZBottom = new LJ12A34ZBY(TIDS_PROXIMITYSENSORZBOTTOM_PIN_GPIO);

    this->zAxis = new PositioningAxis(Z_AXIS_LENGTH_MM, 4.0, this->stepperMotorZ, this->proximitySensorZHome, this->proximitySensorZBottom);

    // Melting

    this->heaterCapMotor = new DS3218(TIDS_HEATERCAPMOTOR_PIN_PWM);

    this->heaterThermometer = new MLX90614(TIDS_HEATERTHERMOMETER_BUS_I2C);

    this->meltingSystem = new MeltingSystem(this->powerController, this->heaterCapMotor, this->heaterThermometer);
}

TIDSControl::~TIDSControl() {
    delete this->meltingSystem;
    delete this->heaterCapMotor;
    delete this->heaterThermometer;
    
    delete this->zAxis;
    delete this->stepperMotorZ;
    delete this->proximitySensorZHome;
    delete this->proximitySensorZBottom;

    delete this->xAxis;
    delete this->stepperMotorX;
    delete this->proximitySensorXHome;

    delete this->drillingSystem;
    delete this->drillMotor;
    delete this->drillEncoder;
    delete this->drillCurrentSensor;

    delete this->telemetrySystem;
    delete this->currentSensor;
    delete this->loadCell;

    this->powerController->turnOffAllRelays();
    delete this->powerController;
}

// Tartan Ice Drilling System

int TIDSControl::run() {
    // Turn off all relays
    this->powerController->turnOffAllRelays();

    // Determine x-axis target position
    for (float targetXPosition = HOLE_DIAMETER_MM; targetXPosition < (X_AXIS_LENGTH_MM - HOLE_DIAMETER_MM); targetPosition += HOLE_SEPARATION_MM) {
        // Turn on contact sensors, x-axis, z-axis
        this->powerController->setProximitySensorsRelayState(PowerController::STATE::ON);
        this->powerController->setStepperMotorXRelayState(PowerController::STATE::ON);
        this->powerController->setStepperMotorZRelayState(PowerController::STATE::ON);
        
        // Move x-axis and z-axis to home
        this->xAxis->moveToHome();
        this->zAxis->moveToHome();

        // Close melting chamber cap
        this->meltingSystem->closeCap();

        // Move to x-axis target location
        this->xAxis->moveTo(targetXPosition);

        // Turn on drill
        this->powerController->setDrillMotorRelayState(PowerController::STATE::ON);

        // Start drill
        this->drillingSystem->start();

        // Move the z-axis down until the bottom sensor is triggered
        int timeout = 0;
        while (!this->zAxis->isAtEnd() && timeout < 30) {
            // Keep weight on bit below WEIGHT_ON_BIT_MAX_KG
            if (this->telemetrySystem->getWeightOnBit() < WEIGHT_ON_BIT_MAX_KG) {
                timeout = 0;
                zAxis->moveBy(Z_AXIS_MOVE_MM);
            }
            // If weight on bit is above limit, try again (with eventual timeout)
            else {
                timeout++;
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        }

        // Stop drill
        this->drillingSystem->stop();

        // Move z-axis to home
        this->zAxis->moveToHome();

        // Move x-axis to home
        this->xAxis->moveToHome();

        // Rotate drill to be at index location, lined up for melting chamber
        this->drillingSystem->rotateToIndex();

        // Turn off drill
        this->powerController->setDrillMotorRelayState(PowerController::STATE::OFF);

        // Open melting chamber cap
        this->meltingSystem->openCap();

        // Move z-axis down until weight on bit registers above threshold
        while (this->telemetrySystem->getWeightOnBit() < WEIGHT_ON_BIT_MIN_KG) {
            zAxis->moveBy(Z_AXIS_MOVE_MM);
        }

        // Wait for ice to enter chamber
        std::this_thread::sleep_for(std::chrono::seconds(5));

        // Move z-axis to home
        this->zAxis->moveToHome();

        // Turn off contact sensors, x-axis, z-axis
        this->powerController->setProximitySensorsRelayState(PowerController::STATE::OFF);
        this->powerController->setStepperMotorXRelayState(PowerController::STATE::OFF);
        this->powerController->setStepperMotorZRelayState(PowerController::STATE::OFF);

        // Turn on melting chamber
        this->meltingSystem->start();

        // Wait for ice to melt
        std::this_thread::sleep_for(std::chrono::minutes(5));

        // Turn off melting chamber
        this->meltingSystem->stop();

        // Turn off all relays
        this->powerController->turnOffAllRelays();
    }
}

// Tests

int TIDSControl::testPowerController() {
    this->powerController->turnOffAllRelays();
    return 0;
}

int TIDSControl::testCurrentSensor() {
    return 0;
}

int TIDSControl::testLoadCell() {
    this->loadCell->tare();
    for (int i = 0; i < 50; i++) {
        while (!this->loadCell->isReady()) {}
        std::cout << "Load cell weight: " << this->loadCell->readWeight() << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    return 0;
}

int TIDSControl::testDrillMotor() {
    return 0;
}

int TIDSControl::testDrillMotorAndEncoder() {
    return 0;
}

int TIDSControl::testDrillCurrentSensor() {
    return 0;
}

int TIDSControl::testAxisX() {
    return 0;
}

int TIDSControl::testAxisZ() {
    return 0;
}

int TIDSControl::testHeater() {
    this->powerController->turnOffAllRelays();
    this->powerController->setHeaterRelayState(PowerController::STATE::ON);
    std::this_thread::sleep_for(std::chrono::seconds(10));
    this->powerController->setHeaterRelayState(PowerController::STATE::OFF);
    return 0;
}

int TIDSControl::testHeaterCapMotor() {
    return 0;
}

int TIDSControl::testHeaterThermometer() {
    return 0;
}

} /* namespace tids */

