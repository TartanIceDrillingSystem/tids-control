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

#ifndef MMPEU_H
#define MMPEU_H

#include <libbbbkit/GPIO.h>

namespace tids {

class MMPEU {
public:
    enum STATE {
        A = 0,
        B = 1,
        C = 2,
        D = 3,
    };
private:
    // GPIO for channel A
    bbbkit::GPIO *gpioA;
    // GPIO for channel B
    bbbkit::GPIO *gpioB;
    // GPIO for revolution index
    bbbkit::GPIO *gpioIndex;
public:
    MMPEU(bbbkit::GPIO::PIN pinA, bbbkit::GPIO::PIN pinB, bbbkit::GPIO::PIN pinIndex);
    virtual ~MMPEU();

    MMPEU::STATE getState();
    bool isAtIndex();
};

} /* namespace tids */

#endif /* MMPEU_H */
