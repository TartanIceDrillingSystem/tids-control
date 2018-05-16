/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Driver for the TB6600 stepper motor driver
    https://www.dfrobot.com/wiki/index.php/TB6600_Stepper_Motor_Driver_SKU:_DRI0043
    Default settings for the NEMA 23-HS16-0884S
    https://www.omc-stepperonline.com/download/23HS16-0884S.pdf

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

TB6600::TB6600(bbbkit::GPIO::PIN pinPUL, bbbkit::GPIO::PIN pinDIR, bbbkit::GPIO::PIN pinENA,
            bbbkit::StepperMotor::DIRECTION direction=bbbkit::StepperMotor::DIRECTION::CLOCKWISE,
            int stepsPerRevolution, float revolutionsPerMinute, int stepFactor)
            : bbbkit::StepperMotor(pinPUL, pinDIR, pinENA, direction, stepsPerRevolution, revolutionsPerMinute, stepFactor) {}

TB6600::~TB6600() {}

} /* namespace tids */
