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

#include "PositioningAxis.h"

namespace tids {

#define SPEED_DEFAULT 10.0f

#define SENSOR_BUFFER_MM 30.0f
#define SENSOR_BUFFER_MOVE_MM 3.0f
#define SENSOR_POSITION_OVERRIDE_MM 0.0f

PositioningAxis::PositioningAxis(float lengthMM, float pitchMM, bbbkit::StepperMotor *motor, LJ12A34ZBY *homeSensor, LJ12A34ZBY *endSensor) {
    // Mark position as uncalibrated
    this->positionMM = -1;
    // Set length
    this->lengthMM = lengthMM;
    // Initialize speed
    this->speed = SPEED_DEFAULT;
    // Initialize stepped leadscrew
    this->leadscrew = new SteppedLeadscrew(motor, pitchMM);
    // Set proximity sensors
    this->homeSensor = homeSensor;
    this->endSensor = endSensor;
}

PositioningAxis::~PositioningAxis() {
    delete this->leadscrew;
}

// Get current position in millimeters
float PositioningAxis::getPosition() {
    return this->positionMM;
}

// Get speed in millimeters per second
float getSpeed() {
    return this->leadscrew->getSpeed();
}

// Set speed in millimeters per second
int setSpeed(float millimetersPerSecond) {
    return this->leadscrew->setSpeed(this->speed);
}

// Move to position at positionMM millimeters
int PositioningAxis::moveTo(float positionMM) {
    float homeBufferPositionMM = SENSOR_BUFFER_MM;
    float endBufferPositionMM = this->lengthMM - SENSOR_BUFFER_MM;

    // Speed must be nonzero to move
    if (this->getSpeed() <= 0.0f) {
        return -1;
    }

    // Target position is smaller and in home sensor buffer zone
    if (positionMM < this->positionMM && positionMM < homeBufferPositionMM) {
        // Stop if home sensor is already triggered
        if (this->homeSensor->isTriggered()) {
            return 0;
        }

        // Move to home sensor buffer start if necessary
        if (this->positionMM > homeBufferPositionMM) {
            // Move to the buffer position
            this->leadscrew->move(homeBufferPositionMM - this->positionMM);
            // Update current position
            this->positionMM = homeBufferPositionMM;
        }

        // Slowly move until the position is reached or sensor is activated
        while (this->positionMM > positionMM) {
            if (this->homeSensor->isTriggered()) {
                // Move a little extra to compensate for sensor imperfection
                this->leadscrew->move(-SENSOR_POSITION_OVERRIDE_MM);
                // Reset current position
                this->positionMM = 0.0f;
                return 0;
            } else {
                // Move by a small amount and check again
                this->leadscrew->move(-SENSOR_BUFFER_MOVE_MM);
                // Update current position
                this->positionMM -= SENSOR_BUFFER_MOVE_MM;
            }
        }
    }

    // Target position is larger and in end sensor buffer zone
    else if (positionMM > this->positionMM && positionMM > endBufferPositionMM) {
        // Stop if end sensor is already triggered
        if (this->endSensor != NULL && this->endSensor->isTriggered()) {
            return 0;
        }

        // Move to end sensor buffer start if necessary
        if (this-> positionMM < endBufferPositionMM) {
            // Move to the end buffer position
            this->leadscrew->move(endBufferPositionMM - this->positionMM);
            // Update current position
            this->positionMM = endBufferPositionMM;
        }

        // Slowly move until the position is reached or end sensor is activated
        while (this->positionMM < positionMM && this->positionMM < this->lengthMM) {
            if (this->endSensor != NULL && this->endSensor->isTriggered()) {
                // Move a little extra to compensate for sensor imperfection
                this->leadscrew->move(SENSOR_POSITION_OVERRIDE_MM);
                // Update current position
                this->positionMM += SENSOR_POSITION_OVERRIDE_MM;
                return 0;
            } else {
                // Move by a small amount and check again
                this->leadscrew->move(SENSOR_BUFFER_MOVE_MM);
                // Update current position
                this->positionMM += SENSOR_BUFFER_MOVE_MM;
            }
        }
    }

    // Target position does not enter a sensor buffer zone
    else {
        // Move to target position
        this->leadscrew->move(positionMM - this->positionMM);
        // Update current position
        this->positionMM = positionMM;
        return 0;
    }
}

// Move to home position (position 0) based on this->homeSensor
int PositioningAxis::moveToHome() {
    return this->moveTo(0.0f);
}

// Move to end position (position this->lengthMM) based on this->endSensor
int PositioningAxis::moveToEnd() {
    return this->moveTo(this->lengthMM);
}

// Move by positionMM millimeters
int PositioningAxis::moveBy(float positionMM) {
    return this->moveTo(this->getPosition() + this->positionMM);
}

} /* namespace tids */
