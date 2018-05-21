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

#ifndef POSITIONINGAXIS_H
#define POSITIONINGAXIS_H

#include <libbbbkit/StepperMotor.h>

#include "LJ12A34ZBY.h"
#include "SteppedLeadscrew.h"

namespace tids {

class PositioningAxis {
private:
    // Length of the axis in millimeters
    float lengthMM;

    // Current position on the axis in millimeters
    float positionMM;

    // Axis leadscrew with stepper motor
    SteppedLeadscrew *leadscrew;

    // Proximity sensor marking home location (position 0)
    LJ12A34ZBY *homeSensor;

    // Proximity sensor at end location (position this->lengthMM)
    LJ12A34ZBY *endSensor;
public:
    PositioningAxis(float lengthMM, float pitchMM, bbbkit::StepperMotor *motor, LJ12A34ZBY *homeSensor, LJ12A34ZBY *endSensor=NULL);
    virtual ~PositioningAxis();

    // Get current position in millimeters
    float getPosition();

    // Get speed in millimeters per second
    float getSpeed();

    // Set speed in millimeters per second
    int setSpeed(float millimetersPerSecond)

    // Move to position at positionMM millimeters
    int moveTo(float positionMM);

    // Move to home position (position 0) based on this->homeSensor
    int moveToHome();

    // Move to end position (position this->lengthMM) based on this->endSensor
    int moveToEnd();

    // Move by positionMM millimeters
    int moveBy(float positionMM);
};

} /* namespace tids */

#endif /* POSITIONINGAXIS_H */
