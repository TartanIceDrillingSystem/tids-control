/*
    Copyright 2018 CMU Tartan Ice Drilling System (TIDS) Controls
    https://github.com/TartanIceDrillingSystem/tids-control

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "steppermotor.h"
#include <cmath>
#include <iostream>
#include <unistd.h>

using namespace std;

namespace tids {

StepperMotor::StepperMotor(int pin_PLS, int pin_DIR, int pin_AWO, int pin_CS,
                           int pin_ALM, int pin_TIM, int revolutionsPerMinute,
                           int stepsPerRevolution) {

    this->gpio_PLS = new GPIO(pin_PLS, GPIO::OUTPUT);
    this->gpio_DIR = new GPIO(pin_DIR, GPIO::OUTPUT);
    this->gpio_AWO = new GPIO(pin_AWO, GPIO::OUTPUT);
    this->gpio_CS = new GPIO(pin_CS, GPIO::OUTPUT);
    this->gpio_ALM = new GPIO(pin_ALM, GPIO::INPUT);
    this->gpio_TIM = new GPIO(pin_TIM, GPIO::INPUT);

    // Set delay
    this->revolutionsPerMinute = revolutionsPerMinute;
    this->stepsPerRevolution = stepsPerRevolution;
    this->updateStepDelay();

    // Default direction to clockwise
    this->setDirection(StepperMotor::CLOCKWISE);

    // Default step angle to basic
    this->gpio_CS->setValue(GPIO::HIGH);
}

StepperMotor::~StepperMotor() {}

// Getters and setters

int StepperMotor::setDirection(StepperMotor::DIRECTION direction) {
    if (this->direction != direction) {
        this->direction = direction;
        switch (this->direction) {
        case CLOCKWISE:
            this->gpio_DIR->setValue(GPIO::HIGH);
            break;
        case COUNTERCLOCKWISE:
            this->gpio_DIR->setValue(GPIO::LOW);
            break;
        default:
            return -1;
        }
    }
    return 0;
}

int StepperMotor::setRevolutionsPerMinute(float revolutionsPerMinute) {
    this->revolutionsPerMinute = revolutionsPerMinute;
    return this->updateStepDelay();
}

int StepperMotor::setStepsPerRevolution(int stepsPerRevolution) {
    this->stepsPerRevolution = stepsPerRevolution;
    return this->updateStepDelay();
}

int StepperMotor::setSleeping(bool sleeping) {
    if (this->sleeping != sleeping) {
        this->sleeping = sleeping;
        if (this->sleeping) this->gpio_AWO->setValue(GPIO::HIGH);
        else this->gpio_AWO->setValue(GPIO::LOW);
    }
    return 0;
}

GPIO::VALUE StepperMotor::getAlarm() {
    return this->gpio_ALM->getValue();
}

GPIO::VALUE StepperMotor::getTimer() {
    return this->gpio_TIM->getValue();
}

// Stepping

void StepperMotor::step() {
    this->gpio_PLS->setValue(GPIO::HIGH);
    this->gpio_PLS->setValue(GPIO::LOW);
}

void StepperMotor::step(int numberOfSteps) {
    for (int steps = 0; steps < numberOfSteps; steps++) {
        this->step();
        usleep(this->stepDelay);
    }
}

void StepperMotor::rotate(float angle) {
    float stepAngle = 360.0f/(float)(this->stepsPerRevolution);
    int numberOfSteps = round(angle/stepAngle);
    this->step(numberOfSteps);
}

// Threaded stepping

int StepperMotor::threadStep(int numberOfSteps, CallbackFunction_t callbackFunction) {
    this->threadStepCount = numberOfSteps;
    this->threadCallbackFunction = callbackFunction;

    this->threadRunning = true;
    if(pthread_create(&this->thread, NULL, &threadStepInternal, static_cast<void*>(this)) != 0){
        perror("StepperMotor: Failed to create the stepping thread.");
        this->threadRunning = false;
        return -1;
    }
    return 0;
}

int StepperMotor::threadStepForever() {
    return this->threadStep(-1, NULL);
}

// Private

int StepperMotor::updateStepDelay() {
    // Assumes that the step signal takes no time (false but negligible)
    float revolutionsPerMicrosecond = (60.0f * 1000.0f * 1000.0f) / (float)(this->revolutionsPerMinute);
    this->stepDelay = (int)(revolutionsPerMicrosecond / this->stepsPerRevolution);
    return 0;
}

void *threadStepInternal(void *value) {
    StepperMotor *stepper = static_cast<StepperMotor*>(value);
    int steps = 0;
    while(stepper->threadRunning) {
        if (stepper->threadStepCount != -1 && steps >= stepper->threadStepCount) {
            if (stepper->threadCallbackFunction) stepper->threadCallbackFunction(steps);
            break;
        }
        stepper->step();
        steps++;
        usleep(stepper->stepDelay);
    }
    return 0;
}

} /* namespace tids */
