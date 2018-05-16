/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Class for leadscrew driven by stepper motor
    https://en.wikipedia.org/wiki/Leadscrew

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

#ifndef STEPPEDLEADSCREW_H
#define STEPPEDLEADSCREW_H

#include <libbbbkit/StepperMotor.h>

namespace tids {

class SteppedLeadscrew {
private:
    bbbkit::StepperMotor *motor;
    float distancePerRevolution;

public:
    SteppedLeadscrew(bbbkit::StepperMotor *motor, float distancePerRevolutionMM);
    virtual ~SteppedLeadscrew();

    // Set distance per revolution
    float getDistancePerRevolution();

    // Get distance per revolution
    int setDistancePerRevolution(float getDistancePerRevolutionMM);

    // Rotate leadscrew to translate by distance, in millimeters
    void move(float distanceMM);
};

} /* namespace tids */

#endif /* STEPPEDLEADSCREW_H */
