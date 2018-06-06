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

#include "DrillingSystem.h"

#include <algorithm>

namespace tids {

#define ENCODER_COUNTS_PER_REVOLUTION 1024.0
#define ENCODER_PULSES_PER_REVOLUTION (ENCODER_COUNTS_PER_REVOLUTION / 4)

#define DRILL_VOLTAGE 90.0f

#define SPEED_MIN_PERCENT 5.0f
#define SPEED_MAX_PERCENT 50.0f
#define SPEED_DELTA_PERCENT 5.0f

#define TORQUE_MIN_NM 8.0f    
#define TORQUE_MAX_NM 10.0f

const double PI = 3.14159265358979323846;

DrillingSystem::DrillingSystem(bbbkit::DCMotor *motor, MMPEU *encoder, LTS6NP *currentSensor) {
    this->motor = motor;
    this->encoder = encoder;
    this->currentSensor = currentSensor;
    this->regulateSpeedThreadShouldCancel = true;

    // Reset encoder trigger time and speed
    this->resetSpeed();
}

DrillingSystem::~DrillingSystem() {
    this->stop();
}

// Start drill and automatically adjust speed based on torque
int DrillingSystem::start() {
    // Return if the speed regulation thread already exists
    if (!this->regulateSpeedThreadShouldCancel) {
        return -1;
    }

    // Start encoder trigger thread to calculate speed
    this->encoder->getGPIOA()->setEdgeType(bbbkit::GPIO::EDGE::RISING);
    this->encoder->getGPIOA()->waitForEdgeThread((bbbkit::CallbackFunction_t)&DrillingSystem::updateSpeed);

    // Start drill at minimum speed
    this->motor->setSpeedPercent(SPEED_MIN_PERCENT);
    this->motor->start();

    // Reset cancellation token
    this->regulateSpeedThreadShouldCancel = false;
    // Start speed regulation on new thread
    this->regulateSpeedThread = std::thread(&DrillingSystem::regulateSpeed, this);

    return 0;
}

// Stop drill
int DrillingSystem::stop() {
    // Cancel and join speed regulation thread
    this->regulateSpeedThreadShouldCancel = true;
    this->regulateSpeedThread.join();

    // Stop encoder trigger thread
    this->encoder->getGPIOA()->stopWaitForEdgeThread();

    // Stop drill
    this->motor->stop();

    // Reset encoder trigger time and speed
    this->resetSpeed();

    return 0;
}

// Rotate drill until index location on encoder
int DrillingSystem::rotateToIndex() {
    // Start dtill at minimum speed
    this->motor->setSpeedPercent(SPEED_MIN_PERCENT);
    this->motor->start();

    // Run until encoder notifies index location
    while (!encoder->isAtIndex()) {}

    // Stop drill
    this->motor->stop();

    return 0;
}

// Get drill rotation speed from encoder in RPM
float DrillingSystem::getSpeed() {
    return this->speedRPM;
}

// Get drill current from current sensor in amps
float DrillingSystem::getCurrent() {
    return this->currentSensor->getCurrent();
}

// Get drill power from in watts
float DrillingSystem::getPower() {
    return DRILL_VOLTAGE * this->getCurrent();
}

// Get drill torque for speed and current in Nm
float DrillingSystem::getTorque() {
    float speedRPM = this->getSpeed();
    float powerW = this->getPower();

    float torqueNM = powerW / (speedRPM * PI / 30.0);
    return torqueNM;
}

// Reset drill speed and encoder trigger
void DrillingSystem::resetSpeed() {
    this->lastEncoderTriggerTimeS = std::chrono::high_resolution_clock::time_point::min();
    this->speedRPM = 0.0f;
}

// Continuously update drill speed for encoder trigger
void DrillingSystem::updateSpeed() {
    std::chrono::high_resolution_clock::time_point encoderTriggerTimeS = std::chrono::high_resolution_clock::now();

    // Update speed if there is a recorded last trigger time
    bool firstSpeedUpdate = (this->lastEncoderTriggerTimeS == std::chrono::high_resolution_clock::time_point::min());
    if (!firstSpeedUpdate) {
        // Calculate time difference between pulses
        //double pulseTimeDifferenceS = std::chrono::duration_cast<std::chrono::duration::seconds>(encoderTriggerTimeS - lastEncoderTriggerTimeS);
	std::chrono::duration<double> difference = encoderTriggerTimeS - lastEncoderTriggerTimeS;
	double pulseTimeDifferenceS = difference.count();
        // Calculate seconds per revolution based on encoder pulses per revolution
        double secondsPerRevolution = pulseTimeDifferenceS * ENCODER_PULSES_PER_REVOLUTION;
        // Convert to revolutions per minute
        double revolutionsPerMinute = 1.0 / (secondsPerRevolution) * 60.0;
        // Update speed
        this->speedRPM = revolutionsPerMinute;
    }

    // Set current trigger time as last trigger time
    this->lastEncoderTriggerTimeS = encoderTriggerTimeS;
}

// Continuously regulate drill speed to control torque
void DrillingSystem::regulateSpeed() {
    // Run until cancellation token
    while (!this->regulateSpeedThreadShouldCancel) {
        // Get current torque
        float torqueNM = this->getTorque();
        
        // Get current speed percentage
        float newSpeedPercent = this->motor->getSpeedPercent();

        // Increase or decrease speed as necessary (within min and max)
        if (torqueNM < TORQUE_MIN_NM) {
            newSpeedPercent = std::min(newSpeedPercent + SPEED_DELTA_PERCENT, SPEED_MAX_PERCENT);
        } else if (torqueNM > TORQUE_MAX_NM) {
            newSpeedPercent = std::max(newSpeedPercent - SPEED_DELTA_PERCENT, SPEED_MIN_PERCENT);
        }

        // Set new speed percentage
        this->motor->setSpeedPercent(newSpeedPercent);

        // Repeat every 0.1 seconds
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

} /* namespace tids */
