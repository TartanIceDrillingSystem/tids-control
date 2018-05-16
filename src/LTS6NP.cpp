/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Driver for the LEM LTS 6-NP current sensor
    https://www.lem.com/sites/default/files/products_datasheets/lts_6-np.pdf

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

#include "LTS6NP.h"

// LTS 6-NP outputs a minimum of 0.5V and a maximum of 4.5V, which is stepped down through a 5V->1.8V voltage divider to a minimum of .18V and a maximum of 1.62V
#define ADC_CURRENT_MIN 180
#define ADC_CURRENT_MAX 1620

#define LTS6NP_CURRENT_MIN -19.2f
#define LTS6NP_CURRENT_MAX 19.2f

namespace tids {

LTS6NP::LTS6NP(bbbkit::ADC::PIN pin) : bbbkit::ADC(pin, ADC_CURRENT_MIN, ADC_CURRENT_MAX) {}

LTS6NP::~LTS6NP() {}

// Get sensed current, in amps (averaged over count)
float LTS6NP::getCurrent(int count) {
    // Read ADC ratio
    float ratio = this->readRatio(count);
    // Calculate current based on sensor min and max
    float current = ratio * (LTS6NP_CURRENT_MAX - LTS6NP_CURRENT_MIN) + LTS6NP_CURRENT_MIN;
    return current;
}

} /* namespace tids */
