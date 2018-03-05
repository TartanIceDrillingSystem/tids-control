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

#ifndef LIBRARY_STEPPERMOTOR_H_
#define LIBRARY_STEPPERMOTOR_H_

#include "../library/gpio.h"

namespace tids {

class StepperMotor {

public:
    enum DIRECTION { CLOCKWISE, COUNTERCLOCKWISE };

private:
    // PLS
    GPIO *gpio_PLS;
    // DIR
    GPIO *gpio_DIR;
    // AWO
    GPIO *gpio_AWO;
    // CS
    GPIO *gpio_CS;
    // ALM
    GPIO *gpio_ALM;
    // TIM
    GPIO *gpio_TIM;

    DIRECTION direction;
    float revolutionsPerMinute;
    int stepsPerRevolution;
    bool sleeping;
    // Delay between steps, in microseconds
    int stepDelay;

    pthread_t thread;
    bool threadRunning;
    CallbackFunction_t threadCallbackFunction;
    int threadStepCount;

public:
    StepperMotor(int pin_PLS, int pin_DIR, int pin_AWO, int pin_CS,
                 int pin_ALM, int pin_TIM, int revolutionsPerMinute = 60,
                 int stepsPerRevolution = 1000);
    virtual ~StepperMotor();
    
    // Getters and setters
    
    virtual DIRECTION getDirection() { return this->direction; }
    virtual int setDirection(DIRECTION direction);

    virtual float getRevolutionsPerMinute() { return this->revolutionsPerMinute; }
    virtual int setRevolutionsPerMinute(float revolutionsPerMinute);
    
    virtual int getStepsPerRevolution() { return this->stepsPerRevolution; }
    virtual int setStepsPerRevolution(int stepsPerRevolution);
    
    virtual bool getSleeping() { return this->sleeping; }
    virtual int setSleeping(bool sleeping);

    virtual GPIO::VALUE getAlarm();
    virtual GPIO::VALUE getTimer();

    // Stepping

    virtual void step();
    virtual void step(int numberOfSteps);
    virtual void rotate(float degrees);

    // Threaded stepping

    virtual int threadStep(int numberOfSteps, CallbackFunction_t callbackFunction);
    virtual int threadStepForever();
    virtual void stopThreadStep() { this->threadRunning = false; }

private:
    virtual int updateStepDelay();
    friend void *threadStepInternal(void *value);
};

void *threadStepInternal(void *value);

} /* namespace tids */

#endif /* LIBRARY_STEPPERMOTOR_H_ */
