/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Driver for the Phidgets i-Snail-VC-10 current sensor
    https://www.phidgets.com/productfiles/3500/3500_0/Documentation/3500_0_i-Snail-VC_Wiring_Diagram.pdf

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

#include "ISNAILVC10.h"

#define ISNAILVC10_CURRENT_MIN 0.0f
#define ISNAILVC10_CURRENT_MAX 10.0f

namespace tids {

ISNAILVC10::ISNAILVC10(bbbkit::ADC::PIN pin) : bbbkit::ADC(pin) {}

ISNAILVC10::~ISNAILVC10() {}

// Get sensed current, in amps (averaged over count)
float ISNAILVC10::getCurrent(int count) {
    // Read ADC ratio
    float ratio = this->readRatio(count);
    // Calculate current based on sensor min and max
    float current = ratio * (ISNAILVC10_CURRENT_MAX - ISNAILVC10_CURRENT_MIN) + ISNAILVC10_CURRENT_MIN;
    return current;
}

} /* namespace tids */
