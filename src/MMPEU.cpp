/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Driver for the MMP EU series optical encoder
    http://www.midwestmotion.com/encoders/eu.pdf

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

namespace tids {

    MMPEU::MMPEU(bbbkit::GPIO::PIN pinA, bbbkit::GPIO::PIN pinB, bbbkit::GPIO::PIN pinIndex) {
        this->gpioA = new bbbkit::GPIO(pinA, bbbkit::GPIO::DIRECTION::INPUT);
        this->gpioB = new bbbkit::GPIO(pinB, bbbkit::GPIO::DIRECTION::INPUT);
        this->gpioIndex = new bbbkit::GPIO(pinIndex, bbbkit::GPIO::DIRECTION::INPUT);
    }

    MMPEU::~MMPEU() {
        delete this->gpioA;
        delete this->gpioB;
        delete this->gpioIndex;
    }

    MMPEU::STATE MMPEU::getState() {
        if (this->gpioA->getValue() == bbbkit::GPIO::VALUE::LOW) {
            if (this->gpioB->getValue() == bbbkit::GPIO::VALUE::LOW) {
                return STATE::A; // 00
            } else {
                return STATE::D; // 01
            }
        } else {
            if (this->gpioB->getValue() == bbbkit::GPIO::VALUE::LOW) {
                return STATE::B; // 10
            } else {
                return STATE::C; // 11
            }
        }
    }

    bool MMPEU::isAtIndex() {
        return (this->gpioIndex->getValue() == bbbkit::GPIO::VALUE::HIGH);
    }

} /* namespace tids */
