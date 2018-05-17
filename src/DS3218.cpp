/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Driver for the DS3218 servo motor
    https://www.autobotic.com.my/ds3218-servo-control-angle-270-degree-25t-servo-arm

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

#include "DS3218.h"

#include <cmath>

namespace tids {

#define DS3218_FREQUENCY_HZ 100
#define DS3218_DUTY_CYCLE_MIN_NS 500000
#define DS3218_DUTY_CYCLE_MAX_NS 2500000

DS3218::DS3218(bbbkit::PWM::PIN pin, int controlAngleDEG=270 int startAngleDEG=0) : bbbkit::ServoMotor(pin) {
    // Set control angle
    this->controlAngleDEG = controlAngleDEG;

    // Set frequency
    this->setFrequency(DS3218_FREQUENCY_HZ);

    // Set angle (and duty cycle)
    this->setAngle(startAngleDEG);

    // Start PWM
    this->start();
}

DS3218::~DS3218() {
    // Stop PWM
    if (this->isRunning()) {
        this->stop();
    }
}

// Get current angle in degrees
int DS3218::getAngle() {
    return this->angleDEG;
}

// Set current angle in degrees
int DS3218::setAngle(int angleDEG) {
    // Ensure angle is between max and min
    int minAngle = this->getMinAngle();
    int maxAngle = this->getMaxAngle();
    if (angleDEG > maxAngle) {
        angleDEG = maxAngle;
    } else if (angleDEG < minAngle) {
        angleDEG = minAngle;
    }

    // Set angle
    this->angleDEG = angleDEG;

    // Set PWM duty cycle
    return this->setDutyCycle(this->dutyCycleForAngle(this->angleDEG));
}

// Move current angle by amount in degrees
int DS3218::move(int angleDEG) {
    return this->setAngle(this->getAngle() + angleDEG);
}

// Get current angle as a percent, where the midpoint is 50%
float DS3218::getAngleAsPercent() {
    return this->percentForAngle(this->getAngle());
}

// Set current angle as a percent, where the midpoint is 50%
int DS3218::setAngleAsPercent(float anglePercent) {
    return this->setAngle(this->angleForPercent(anglePercent));
}

// Midpoint angle
int DS3218::getNeutralAngle() {
    return 0;
}

// Maximum angle through clockwise rotation
int DS3218::getMaxAngle() {
    return (this->controlAngleDEG / 2);
}

// Maximum angle through counterclockwise rotation
int DS3218::getMinAngle() {
    return -(this->controlAngleDEG / 2);
}

// Convert angle to percent, where the midpoint is 50%
float percentForAngle(int angleDEG) {
    float minAngle = static_cast<float>(this->getMinAngle());
    float maxAngle = static_cast<float>(this->getMaxAngle());
    float angle = static_cast<float>(angleDEG);

    percent = 100.0f * (angle - minAngle) / (maxAngle - minAngle);
    return percent;
}

// Convert percent to angle, where the midpoint is 50%
int angleForPercent(float anglePercent) {
    float ratio = anglePercent / 100.0f;
    float minAngle = static_cast<float>(this->getMinAngle());
    float maxAngle = static_cast<float>(this->getMaxAngle());

    float angle = anglePercent * (maxAngle - minAngle) + minAngle;
    return static_cast<int>(angle);
}

// PWM duty cycle to set DS3218 to angle in nanoseconds
int DS3218::dutyCycleForAngle(int angleDEG) {
    float ratio = this->percentForAngle(angleDEG) / 100.0f;
    float dutyCycleMin = static_cast<float>(DS3218_DUTY_CYCLE_MIN_NS);
    float dutyCycleMax = static_cast<float>(DS3218_DUTY_CYCLE_MAX_NS);

    float dutyCycleFloat = ratio * (dutyCycleMax - dutyCycleMin) + dutyCycleMin;
    int dutyCycle = static_cast<int>(round(dutyCycleFloat));
    return dutyCycle;
}

} /* namespace tids */
