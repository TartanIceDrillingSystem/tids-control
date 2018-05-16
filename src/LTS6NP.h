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

#ifndef LTS6NP_H
#define LTS6NP_H

#include <libbbbkit/ADC.h>

namespace tids {

class LTS6NP: public bbbkit::ADC {
public:
    LTS6NP(bbbkit::ADC::PIN pin);
    virtual ~LTS6NP();

    // Get sensed current, in amps (averaged over count)
    float getCurrent(int count=1);
};

} /* namespace tids */

#endif /* LTS6NP_H */
