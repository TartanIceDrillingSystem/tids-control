/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Driver for the L298N motor driver
    https://www.sparkfun.com/datasheets/Robotics/L298_H_Bridge.pdf
    Default settings for the MMP TM36-268F-24V GP32-169 DC motor
    http://www.midwestmotion.com/gearmotors.htm

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

#ifndef L298N_H
#define L298N_H

#include <libbbbkit/DCMotor.h>
#include <libbbbkit/GPIO.h>

namespace tids {

class L298N: public bbbkit::DCMotor {
public:
    enum DIRECTION { CLOCKWISE, COUNTERCLOCKWISE };

private:
    // GPIO pins for controlling motor direction
    bbbkit::GPIO *gpioIN1;
    bbbkit::GPIO *gpioIN2;

    L298N::DIRECTION direction;

public:
    L298N(bbbkit::PWM::PIN pinENA, bbbkit::GPIO::PIN pinIN1, bbbkit::GPIO::PIN pinIN2, int dutyCyclePeriodNS=1000, float speedPercent=0.0, L298N::DIRECTION direction=L298N::DIRECTION::CLOCKWISE);
    virtual ~L298N();

    L298N::DIRECTION getDirection();
    int setDirection(L298N::DIRECTION direction);
};

} /* namespace tids */

#endif /* L298N_H */
