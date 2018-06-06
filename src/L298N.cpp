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

#include "L298N.h"

namespace tids {

L298N::L298N(bbbkit::PWM::PIN pinENA, bbbkit::GPIO::PIN pinIN1, bbbkit::GPIO::PIN pinIN2, int dutyCyclePeriodNS, float speedPercent, L298N::DIRECTION direction) : bbbkit::DCMotor(pinENA, dutyCyclePeriodNS, speedPercent) {
    
    // Initialize GPIOs
    this->gpioIN1 = new bbbkit::GPIO(pinIN1, bbbkit::GPIO::DIRECTION::OUTPUT);
    this->gpioIN2 = new bbbkit::GPIO(pinIN2, bbbkit::GPIO::DIRECTION::OUTPUT);

    // Set direction
    this->setDirection(direction);
}

L298N::~L298N() {
    delete this->gpioIN1;
    delete this->gpioIN2;
}

L298N::DIRECTION L298N::getDirection() {
    return this->direction;
}

int L298N::setDirection(L298N::DIRECTION direction) {
    this->direction = direction;

    // Set direction with GPIOs
    if (this->direction == L298N::DIRECTION::CLOCKWISE){
        this->gpioIN1->setValue(bbbkit::GPIO::VALUE::HIGH);
        this->gpioIN2->setValue(bbbkit::GPIO::VALUE::LOW);
    } else {
        this->gpioIN1->setValue(bbbkit::GPIO::VALUE::LOW);
        this->gpioIN2->setValue(bbbkit::GPIO::VALUE::HIGH);
    }

    return 0;
}

} /* namespace tids */
