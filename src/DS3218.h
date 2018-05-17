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

#ifndef DS3218_H
#define DS3218_H

#include <libbbbkit/ServoMotor.h>

namespace tids {

class DS3218: public bbbkit::ServoMotor {
private:
    // The DS3218 has 270 and 180 degree variants
    int controlAngleDEG;
    // Current angle of DS3218
    // 0 degrees is defined as the midpoint for the DS3218
    // the 270-degree variant can rotate from -135 to +135 degrees
    // the 180-degree variant can rotate from -90 to +90 degrees
    int angleDEG;

public:
    DS3218(bbbkit::PWM::PIN pin, int controlAngleDEG=270 int startAngleDEG=0);
    virtual ~DS3218();

    // Get current angle in degrees
    int getAngle();

    // Set current angle in degrees
    int setAngle(int angleDEG);

    // Move current angle by amount in degrees
    int move(int angleDEG);

    // Get current angle as a percent, where the midpoint is 50%
    float getAngleAsPercent();

    // Set current angle as a percent, where the midpoint is 50%
    int setAngleAsPercent(float anglePercent);

    // Midpoint angle
    int getNeutralAngle();

    // Maximum angle through clockwise rotation
    int getMaxAngle();
    
    // Maximum angle through counterclockwise rotation
    int getMinAngle();

private:
    // Convert angle to percent, where the midpoint is 50%
    float percentForAngle(int angleDEG);

    // Convert percent to angle, where the midpoint is 50%
    int angleForPercent(float anglePercent);

    // PWM duty cycle to set DS3218 to angle in nanoseconds
    int dutyCycleForAngle(int angleDEG);
};

} /* namespace tids */

#endif /* TB6600_H */