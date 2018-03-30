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

#include <iostream>
#include <unistd.h>

#include "../drivers/steppermotor.h"

using namespace std;
using namespace tids;

#define PIN_PLS 44
#define PIN_DIR 65
#define PIN_AWO 26
#define PIN_CS 45
#define PIN_ALM 46
#define PIN_TIM 47
#define STEPS_PER_REVOLUTION 1000

void waitTwoSeconds() {
    cout << "Waiting 2 seconds..." << endl;
    usleep(2000000);
}

int main(int argc, char **argv) {
    cout << "Testing StepperMotor driver:" << endl;

    cout << "Initializing StepperMotor...";
    StepperMotor motor(PIN_PLS, PIN_DIR, PIN_AWO, PIN_CS, PIN_ALM, PIN_TIM,
                       STEPS_PER_REVOLUTION);
    cout << " done!" << endl;

    // Rotate clockwise 1 revolution at 12rpm (5 seconds total)
    cout << "1 revolution CW @ 12rpm (5 seconds)..." << endl;
    motor.setRevolutionsPerMinute(6);
    motor.rotate(360.0f);

    waitTwoSeconds();

    // Rotate clockwise 10 revolutions at 60rpm (10 seconds total)
    cout << "10 revolutions CW @ 60rpm (10 seconds)..." << endl;
    motor.setRevolutionsPerMinute(60);
    motor.rotate(360.0f * 10.0f);

    waitTwoSeconds();  

    // Rotate counterclockwise 10 revolutions at 100rpm (6 seconds total)
    cout << "10 revolutions CCW @ 100rpm (6 seconds)..." << endl;
    motor.setRevolutionsPerMinute(100);
    motor.setDirection(StepperMotor::DIRECTION::COUNTERCLOCKWISE);
    motor.rotate(360.0f * 10.0f);

    waitTwoSeconds();  

    // Motor sleep for 5 seconds

    cout << "Sleeping motor for 5 seconds..." << endl;
    motor.setSleeping(true);
    usleep(5000000);

    // Motor wake for 5 seconds

    cout << "Waking motor for 5 seconds..." << endl;
    motor.setSleeping(false);
    usleep(5000000);

    cout << "StepperMotor test complete!" << endl;
    return 0;
}
