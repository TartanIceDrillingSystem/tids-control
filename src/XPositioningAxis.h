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

#ifndef XPOSITIONINGAXIS_H
#define XPOSITIONINGAXIS_H

#include "CVD524K.h"
#include "LJ12A34ZBY.h"
#include "SteppedLeadscrew.h"

namespace tids {

class XPositioningAxis {
private:
    // Length of the axis in millimeters
    float lengthMM;

    // Current position on the axis in millimeters
    float positionMM;

    // Axis leadscrew with stepper motor
    SteppedLeadscrew *leadscrew;

    // Proximity sensor marking home location (position 0)
    LJ12A34ZBY *homeSensor;

public:
    XPositioningAxis(float lengthMM, float pitchMM, CVD524K *motor, LJ12A34ZBY *homeSensor);
    virtual ~XPositioningAxis();

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

    // Move by positionMM millimeters
    int moveBy(float positionMM);

    // If this->homeSensor is active
    bool isAtHome();
};

} /* namespace tids */

#endif /* XPOSITIONINGAXIS_H */
