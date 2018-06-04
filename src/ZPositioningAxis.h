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

#ifndef ZPOSITIONINGAXIS_H
#define ZPOSITIONINGAXIS_H

#include "L298N.h"
#include "LJ12A34ZBY.h"

namespace tids {

class ZPositioningAxis {
private:
    // Length of the axis in millimeters
    float lengthMM;

    // DC motor
    L298N *motor;

    // Proximity sensor marking home location (position 0)
    LJ12A34ZBY *homeSensor;

    // Proximity sensor at end location (position this->lengthMM)
    LJ12A34ZBY *endSensor;
public:
    ZPositioningAxis(float lengthMM, float pitchMM, L298N *motor, LJ12A34ZBY *homeSensor, LJ12A34ZBY *endSensor);
    virtual ~ZPositioningAxis();

    // Start moving to home position
    int startMovingtoHome();

    // Start moving to end position
    int startMovingToEnd();

    // Stop moving
    int stop();

    // Move to home position (position 0) based on this->homeSensor
    int moveToHome();

    // Move to end position (position this->lengthMM) based on this->endSensor
    int moveToEnd();

    // If this->homeSensor is active
    bool isAtHome();

    // If this->endSensor is active
    bool isAtEnd();
};

} /* namespace tids */

#endif /* ZPOSITIONINGAXIS_H */
