/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Class for power controller backed by relay
    https://en.wikipedia.org/wiki/Solid-state_relay

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

#include "PowerController.h"

#include <iostream>

namespace tids {

PowerController::PowerController(bbbkit::GPIO::PIN pinRelayChiller, bbbkit::GPIO::PIN pinRelayDrillMotor, bbbkit::GPIO::PIN pinRelayHeater1, bbbkit::GPIO::PIN pinRelayHeater2, bbbkit::GPIO::PIN pinRelayProximitySensors, bbbkit::GPIO::PIN pinRelayStepperMotorX, bbbkit::GPIO::PIN pinRelayStepperMotorZ) {
    // Initialize relay GPIOs
    this->gpioRelayChiller = new bbbkit::GPIO(pinRelayChiller, bbbkit::GPIO::DIRECTION::OUTPUT);
    this->gpioRelayDrillMotor = new bbbkit::GPIO(pinRelayDrillMotor, bbbkit::GPIO::DIRECTION::OUTPUT);
    this->gpioRelayHeater1 = new bbbkit::GPIO(pinRelayHeater1, bbbkit::GPIO::DIRECTION::OUTPUT);
    this->gpioRelayHeater2 = new bbbkit::GPIO(pinRelayHeater2, bbbkit::GPIO::DIRECTION::OUTPUT);
    this->gpioRelayProximitySensors = new bbbkit::GPIO(pinRelayProximitySensors, bbbkit::GPIO::DIRECTION::OUTPUT);
    this->gpioRelayStepperMotorX = new bbbkit::GPIO(pinRelayStepperMotorX, bbbkit::GPIO::DIRECTION::OUTPUT);
    this->gpioRelayStepperMotorZ = new bbbkit::GPIO(pinRelayStepperMotorZ, bbbkit::GPIO::DIRECTION::OUTPUT);

    // Ensure all relays are off
    this->turnOffAllRelays();
}

PowerController::~PowerController() {
    delete this->gpioRelayChiller;
    delete this->gpioRelayDrillMotor;
    delete this->gpioRelayHeater1;
    delete this->gpioRelayHeater2;
    delete this->gpioRelayProximitySensors;
    delete this->gpioRelayStepperMotorX;
    delete this->gpioRelayStepperMotorZ;
}

PowerController::STATE PowerController::getChillerRelayState() {
    return this->getRelayState(this->gpioRelayChiller);
}

int PowerController::setChillerRelayState(PowerController::STATE state) {
    return this->setRelayState(this->gpioRelayChiller, state);
}

PowerController::STATE PowerController::getDrillMotorRelayState() {
    return this->getRelayState(this->gpioRelayDrillMotor);
}

int PowerController::setDrillMotorRelayState(PowerController::STATE state) {
    return this->setRelayState(this->gpioRelayDrillMotor, state);
}

PowerController::STATE PowerController::getHeaterRelayState() {
    return this->getRelayState(this->gpioRelayHeater1);
}

int PowerController::setHeaterRelayState(PowerController::STATE state) {
    if (this->setRelayState(this->gpioRelayHeater1, state) < 0) {
        return -1;
    }
    if (this->setRelayState(this->gpioRelayHeater2, state) < 0) {
        return -1;
    }
    return 0;
}

PowerController::STATE PowerController::getProximitySensorsRelayState() {
    return this->getRelayState(this->gpioRelayProximitySensors);
}

int PowerController::setProximitySensorsRelayState(PowerController::STATE state) {
    return this->setRelayState(this->gpioRelayProximitySensors, state);
}

PowerController::STATE PowerController::getStepperMotorXRelayState() {
    return this->getRelayState(this->gpioRelayStepperMotorX);
}

int PowerController::setStepperMotorXRelayState(PowerController::STATE state) {
    return this->setRelayState(this->gpioRelayStepperMotorX, state);
}

PowerController::STATE PowerController::getStepperMotorZRelayState() {
    return this->getRelayState(this->gpioRelayStepperMotorZ);
}

int PowerController::setStepperMotorZRelayState(PowerController::STATE state) {
    return this->setRelayState(this->gpioRelayStepperMotorZ, state);
}

int PowerController::turnOffAllRelays() {
    if (this->setChillerRelayState(PowerController::STATE::OFF)) {
        return -1;
    }
    if (this->setDrillMotorRelayState(PowerController::STATE::OFF)) {
        return -1;
    }
    if (this->setHeaterRelayState(PowerController::STATE::OFF)) {
        return -1;
    }
    if (this->setProximitySensorsRelayState(PowerController::STATE::OFF)) {
        return -1;
    }
    if (this->setStepperMotorXRelayState(PowerController::STATE::OFF)) {
        return -1;
    }
    if (this->setStepperMotorZRelayState(PowerController::STATE::OFF)) {
        return -1;
    }
    return 0;
}

PowerController::STATE PowerController::getRelayState(bbbkit::GPIO *gpioRelay) {
    // Get GPIO value
    if (gpioRelay->getValue() == bbbkit::GPIO::VALUE::HIGH) {
        return PowerController::STATE::ON;
    } else {
        return PowerController::STATE::OFF;
    }
}

int PowerController::setRelayState(bbbkit::GPIO *gpioRelay, PowerController::STATE state) {
    // Determine GPIO value for power state
    bbbkit::GPIO::VALUE value = bbbkit::GPIO::VALUE::LOW;
    if (state == PowerController::STATE::ON) {
        value = bbbkit::GPIO::VALUE::HIGH;
    }

    // Set GPIO value
    if (gpioRelay->setValue(value) < 0) {
        std::cout << "PowerController: Error setting relay state." << std::endl;
        return -1;
    }
    return 0;
}

} /* namespace tids */
