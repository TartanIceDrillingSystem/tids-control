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

PowerController::PowerController(bbbkit::GPIO::PIN pinRelayChiller, bbbkit::GPIO::PIN pinRelayDrillMotor, bbbkit::GPIO::PIN pinRelayHeater1, bbbkit::GPIO::PIN pinRelayHeater2, bbbkit::GPIO::PIN pinRelayProximitySensors, bbbkit::GPIO::PIN pinRelayMotorX, bbbkit::GPIO::PIN pinRelayMotorZ, bbbkit::GPIO::PIN pinRelay24V); {
    // Initialize relay GPIOs
    this->gpioRelayChiller = new bbbkit::GPIO(pinRelayChiller, bbbkit::GPIO::DIRECTION::OUTPUT, bbbkit::GPIO::VALUE::LOW);
    this->gpioRelayDrillMotor = new bbbkit::GPIO(pinRelayDrillMotor, bbbkit::GPIO::DIRECTION::OUTPUT, bbbkit::GPIO::VALUE::LOW);
    this->gpioRelayHeater1 = new bbbkit::GPIO(pinRelayHeater1, bbbkit::GPIO::DIRECTION::OUTPUT, bbbkit::GPIO::VALUE::LOW);
    this->gpioRelayHeater2 = new bbbkit::GPIO(pinRelayHeater2, bbbkit::GPIO::DIRECTION::OUTPUT, bbbkit::GPIO::VALUE::LOW);
    this->gpioRelayProximitySensors = new bbbkit::GPIO(pinRelayProximitySensors, bbbkit::GPIO::DIRECTION::OUTPUT, bbbkit::GPIO::VALUE::LOW);
    this->gpioRelayMotorX = new bbbkit::GPIO(pinRelayMotorX, bbbkit::GPIO::DIRECTION::OUTPUT, bbbkit::GPIO::VALUE::LOW);
    this->gpioRelayMotorZ = new bbbkit::GPIO(pinRelayMotorZ, bbbkit::GPIO::DIRECTION::OUTPUT, bbbkit::GPIO::VALUE::LOW);
    this->gpioRelay24V = new bbbkit::GPIO(pinRelay24V, bbbkit::GPIO::DIRECTION::OUTPUT, bbbkit::GPIO::VALUE::LOW);

    // Ensure all relays are off
    this->turnOffAllRelays();
}

PowerController::~PowerController() {
    // Ensure all relays are off
    this->turnOffAllRelays();

    delete this->gpioRelayChiller;
    delete this->gpioRelayDrillMotor;
    delete this->gpioRelayHeater1;
    delete this->gpioRelayHeater2;
    delete this->gpioRelayProximitySensors;
    delete this->gpioRelayMotorX;
    delete this->gpioRelayMotorZ;
    delete this->gpioRelay24V;
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

PowerController::STATE PowerController::getMotorXRelayState() {
    return this->getRelayState(this->gpioRelayMotorX);
}

int PowerController::setMotorXRelayState(PowerController::STATE state) {
    return this->setRelayState(this->gpioRelayMotorX, state);
}

PowerController::STATE PowerController::getMotorZRelayState() {
    return this->getRelayState(this->gpioRelayMotorZ);
}

int PowerController::setMotorZRelayState(PowerController::STATE state) {
    return this->setRelayState(this->gpioRelayMotorZ, state);
}

PowerController::STATE PowerController::get24VRelayState() {
    return this->getRelayState(this->gpioRelay24V);
}

int PowerController::set24VRelayState(PowerController::STATE state) {
    return this->setRelayState(this->gpioRelay24V, state);
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
    if (this->setMotorXRelayState(PowerController::STATE::OFF)) {
        return -1;
    }
    if (this->setMotorZRelayState(PowerController::STATE::OFF)) {
        return -1;
    }
    if (this->set24VRelayState(PowerController::STATE::OFF)) {
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
