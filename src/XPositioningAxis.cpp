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

#include "XPositioningAxis.h"

namespace tids {

// Default leadscrew speed in mm/s
#define SPEED_DEFAULT 10.0f

// Sensor buffer area extending from the home location
#define SENSOR_BUFFER_MM 30.0f

// Distance to move by between home sensor checks in buffer
#define SENSOR_BUFFER_MOVE_MM 3.0f

// Additional distance to move after home sensor is triggered
#define SENSOR_POSITION_OVERRIDE_MM 0.0f

XPositioningAxis::XPositioningAxis(float lengthMM, float pitchMM, CVD524K *motor, LJ12A34ZBY *homeSensor) {
    // Mark position as uncalibrated
    this->positionMM = length + 1;
    // Set length
    this->lengthMM = lengthMM;
    // Initialize stepped leadscrew
    this->leadscrew = new SteppedLeadscrew(motor, pitchMM);
    // Set proximity sensor
    this->homeSensor = homeSensor;
    // Set speed on leadscrew
    this->setSpeed(SPEED_DEFAULT);
}

XPositioningAxis::~XPositioningAxis() {
    delete this->leadscrew;
}

// Get current position in millimeters
float XPositioningAxis::getPosition() {
    return this->positionMM;
}

// Get speed in millimeters per second
float XPositioningAxis::getSpeed() {
    return this->leadscrew->getSpeed();
}

// Set speed in millimeters per second
int XPositioningAxis::setSpeed(float millimetersPerSecond) {
    return this->leadscrew->setSpeed(this->speed);
}

// Move to position at positionMM millimeters
int XPositioningAxis::moveTo(float positionMM) {
    float homeBufferPositionMM = SENSOR_BUFFER_MM;

    // Speed must be nonzero to move
    if (this->getSpeed() <= 0.0f) {
        return -1;
    }

    // Target position is smaller and in home sensor buffer zone
    if (positionMM < this->positionMM && positionMM < homeBufferPositionMM) {
        // Stop if home sensor is already triggered
        if (this->isAtHome()) {
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
            if (this->isAtHome()) {
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

    // Target position does not enter sensor buffer zone
    else {
        // Move to target position
        this->leadscrew->move(positionMM - this->positionMM);
        // Update current position
        this->positionMM = positionMM;
        return 0;
    }
}

// Move to home position (position 0) based on this->homeSensor
int XPositioningAxis::moveToHome() {
    return this->moveTo(0.0f);
}

// Move by positionMM millimeters
int XPositioningAxis::moveBy(float positionMM) {
    return this->moveTo(this->getPosition() + this->positionMM);
}

// If this->homeSensor is active
bool XPositioningAxis::isAtHome() {
    return this->homeSensor->isTriggered();
}

} /* namespace tids */
