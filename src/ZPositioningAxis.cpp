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

#include "ZPositioningAxis.h"

#include <chrono>
#include <thread>

namespace tids {

#define MOTOR_SPEED_PERCENT 30.0f

#define MOTOR_ROTATION_DIRECTION_HOME L298N::DIRECTION::COUNTERCLOCKWISE
#define MOTOR_ROTATION_DIRECTION_END L298N::DIRECTION::CLOCKWISE

ZPositioningAxis::ZPositioningAxis(float lengthMM, float pitchMM, L298N *motor, LJ12A34ZBY *homeSensor, LJ12A34ZBY *endSensor) {
    // Set length
    this->lengthMM = lengthMM;
    // Set motor and initialize speed
    this->motor = motor;
    this->motor->setSpeedPercent(MOTOR_SPEED_PERCENT);
    // Set proximity sensors
    this->homeSensor = homeSensor;
    this->endSensor = endSensor;
}

ZPositioningAxis::~ZPositioningAxis() {}

// Start moving to home position
int ZPositioningAxis::startMovingToHome() {
    // Return if already at home position
    if (this->isAtHome()) {
        return -1;
    }

    // Set motor direction to move to home
    this->motor->setDirection(MOTOR_ROTATION_DIRECTION_HOME);

    // Start moving
    this->motor->start();
    return 0;
}

// Start moving to end position
int ZPositioningAxis::startMovingToEnd() {
    // Return if already at end position
    if (this->isAtEnd()) {
        return -1;
    }

    // Set motor direction to move to end
    this->motor->setDirection(MOTOR_ROTATION_DIRECTION_END);

    // Start moving
    this->motor->start();
    return 0;
}

// Stop moving
int ZPositioningAxis::stop() {
    if (this->motor->isRunning()) {
        return this->motor->stop();
    }
    return -1;
}

// Move to home position (position 0) based on this->homeSensor
int ZPositioningAxis::moveToHome() {
    this->startMovingToHome();
    // Loop until home reached (check every 10 milliseconds)
    while (!this->isAtHome()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return this->stop();
}

// Move to end position (position this->lengthMM) based on this->endSensor
int ZPositioningAxis::moveToEnd() {
    this->startMovingToEnd();
    // Loop until end reached (check every 10 milliseconds)
    while (!this->isAtEnd()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return this->stop();
}

// If this->homeSensor is active
bool ZPositioningAxis::isAtHome() {
    return this->homeSensor->isTriggered();
}

// If this->endSensor is active
bool ZPositioningAxis::isAtEnd() {
    return this->endSensor->isTriggered();
}

} /* namespace tids */
