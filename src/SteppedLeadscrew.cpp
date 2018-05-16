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

namespace tids {

SteppedLeadscrew::SteppedLeadscrew(StepperMotor *motor, float distancePerRevolution) {
    this->motor = motor;
    this->distancePerRevolution = distancePerRevolution;
}

SteppedLeadscrew::~SteppedLeadscrew() {}

// Set distance per revolution
float SteppedLeadscrew::getDistancePerRevolution {
    return this->distancePerRevolution;
}

// Get distance per revolution
int SteppedLeadscrew::setDistancePerRevolution(float distancePerRevolution) {
    this->distancePerRevolution = distancePerRevolution;
    return 0;
}

// Rotate leadscrew to translate by distance, in millimeters
int SteppedLeadscrew::move(float distanceMM) {
    // Set rotation direction on stepper motor
    bbbkit::StepperMotor::DIRECTION rotationDirection = bbbkit::StepperMotor::DIRECTION::CLOCKWISE;
    if (distanceMM < 0) {
        this->motor->setDirection(bbbkit::StepperMotor::DIRECTION::COUNTERCLOCKWISE);
    }

    float revolutions = distanceMM / this->distancePerRevolution;
    float angleDEG = revolutions * 360.0f;
    return this->motor->rotate(angleDEG);
}

} /* namespace tids */
