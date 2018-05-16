/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Driver for the LJ12A3-4-Z/BY inductive proximity sensor
    https://www.reprap.me/lj12a3-4-z-by-inductive-proximity-sensor.html

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

#include "LJ12A34ZBY.h"

namespace tids {

LJ12A34ZBY::LJ12A34ZBY(bbbkit::GPIO::PIN pin) {
    this->gpio = new bbbkit::GPIO(pin, bbbkit::GPIO::DIRECTION::INPUT);
}

LJ12A34ZBY::~LJ12A34ZBY() {
    delete this->gpio;
}

bool LJ12A34ZBY::isTriggered() {
    return (this->gpio->getValue() == bbbkit::GPIO::VALUE::HIGH);
}

} /* namespace tids */
