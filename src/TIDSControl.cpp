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

#include <bbbkit/GPIO.h>
#include <bbbkit/StepperMotor.h>

namespace tids {

int testStepperMotor() {
    std::cout << "Testing StepperMotor driver:" << std::endl;

    std::cout << "Initializing StepperMotor...";
    bbbkit::GPIO gpioPLS(bbbkit::GPIO::PIN::GPIO_44);
    bbbkit::GPIO gpioDIR(bbbkit::GPIO::PIN::GPIO_65);
    bbbkit::GPIO gpioAWO(bbbkit::GPIO::PIN::GPIO_26);
    bbbkit::GPIO gpioCS(bbbkit::GPIO::PIN::GPIO_45);
    bbbkit::GPIO gpioALM(bbbkit::GPIO::PIN::GPIO_46);
    bbbkit::GPIO gpioTIM(bbbkit::GPIO::PIN::GPIO_47);
    StepperMotor motor(gpioPLS, gpioDIR, gpioAWO, gpioCS, gpioALM, gpioTIM,
                       1000);
    std::cout << " done!" << std::endl;

    // Rotate clockwise 1 revolution at 12rpm (5 seconds total)
    std::cout << "1 revolution CW @ 12rpm (5 seconds)..." << std::endl;
    motor.setRevolutionsPerMinute(6);
    motor.rotate(360.0f);

    std::cout << "Waiting 2 seconds..." << std::endl;
    usleep(2000000);

    // Rotate clockwise 10 revolutions at 60rpm (10 seconds total)
    std::cout << "10 revolutions CW @ 60rpm (10 seconds)..." << std::endl;
    motor.setRevolutionsPerMinute(60);
    motor.rotate(360.0f * 10.0f);

    std::cout << "Waiting 2 seconds..." << std::endl;
    usleep(2000000); 

    // Rotate counterclockwise 10 revolutions at 100rpm (6 seconds total)
    std::cout << "10 revolutions CCW @ 100rpm (6 seconds)..." << std::endl;
    motor.setRevolutionsPerMinute(100);
    motor.setDirection(StepperMotor::DIRECTION::COUNTERCLOCKWISE);
    motor.rotate(360.0f * 10.0f);

    std::cout << "Waiting 2 seconds..." << std::endl;
    usleep(2000000);  

    // Motor sleep for 5 seconds

    std::cout << "Sleeping motor for 5 seconds..." << std::endl;
    motor.setSleeping(true);
    usleep(5000000);

    // Motor wake for 5 seconds

    std::cout << "Waking motor for 5 seconds..." << std::endl;
    motor.setSleeping(false);
    usleep(5000000);

    std::cout << "StepperMotor test complete!" << std::endl;

    return 0;
}

int main(int argc, char **argv) {
    testStepperMotor();
    return 0;
}

} /* namespace tids */

