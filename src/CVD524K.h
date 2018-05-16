/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Driver for the Oriental Motor CVD524-K stepper motor driver
    http://www.orientalmotor.com/products/pdfs/opmanuals/HM-60128-7E.pdf
    Default settings for the Oriental Motor PKP569FMN24A stepper motor
    https://catalog.orientalmotor.com/item/pkp-series-5-phase-stepper-motors/pk-series-60mm-5-phase-stepper-motors/pkp569fmn24a

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

#ifndef CVD524K_H
#define CVD524K_H

#include <libbbbkit/StepperMotor.h>

namespace tids {

class CVD524K: public bbbkit::StepperMotor {
private:
    // GPIO pin for step angle switching
    bbbkit::GPIO *gpioCS;
    // GPIO pin for alarm
    bbbkit::GPIO *gpioALM;
    // GPIO pin for position timing output
    bbbkit::GPIO *gpioTIM;
public:
    CVD524K(bbbkit::GPIO::PIN pinPLS, bbbkit::GPIO::PIN pinCW, bbbkit::GPIO::PIN pinAWO,
            bbbkit::GPIO::PIN pinCS, bbbkit::GPIO::PIN pinALM, bbbkit::GPIO::PIN pinTIM,
            bbbkit::StepperMotor::DIRECTION direction=bbbkit::StepperMotor::DIRECTION::CLOCKWISE,
            int stepsPerRevolution=1000, float revolutionsPerMinute=60.0f, int stepFactor=1);
    virtual ~CVD524K();

    bbbkit::GPIO::VALUE getAlarm();
    bbbkit::GPIO::VALUE getTimer();
};

} /* namespace tids */

#endif /* CVD524K_H */
