/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

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

#ifndef DRILLINGSYSTEM_H
#define DRILLINGSYSTEM_H

#include <libbbbkit/DCMotor.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "MMPEU.h"
#include "LTS6NP.h"

namespace tids {

class DrillingSystem {
private:
    bbbkit::DCMotor *motor;
    MMPEU *encoder;
    LTS6NP *currentSensor;

    std::chrono::high_resolution_clock::time_point lastEncoderTriggerTimeS;
    float speedRPM;

    std::thread regulateSpeedThread;
    std::atomic<bool> regulateSpeedThreadShouldCancel;
public:
    DrillingSystem(bbbkit::DCMotor *motor, MMPEU *encoder, LTS6NP *currentSensor);
    virtual ~DrillingSystem();

    // Start drill and automatically adjust speed based on torque
    int start();

    // Stop drill
    int stop();

    // Rotate drill until index location on encoder
    int rotateToIndex();

    // Get drill rotation speed from encoder in RPM
    float getSpeed();

    // Get drill current from current sensor in amps
    float getCurrent();

    // Get drill power from in watts
    float getPower();

    // Get drill torque for speed and current in Nm
    float getTorque();

private:
    // Reset drill speed and encoder trigger
    void resetSpeed();

    // Continuously update drill speed for encoder trigger
    void updateSpeed();

    // Continuously regulate drill speed to control torque
    void regulateSpeed();
};

} /* namespace tids */

#endif /* DRILLINGSYSTEM_H */
