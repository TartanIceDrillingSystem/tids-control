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

#include <iostream>
#include <unistd.h>

#include <libbbbkit/GPIO.h>
#include <libbbbkit/PWM.h>
#include <libbbbkit/StepperMotor.h>

int testStepperMotor() {
    std::cout << "Testing StepperMotor driver:" << std::endl;

    std::cout << "Initializing StepperMotor..." << std::endl;
    bbbkit::StepperMotor *motor = new bbbkit::StepperMotor(bbbkit::GPIO::PIN::GPIO_67, bbbkit::GPIO::PIN::GPIO_68, bbbkit::GPIO::PIN::GPIO_26);

    // Parameters for Nema 23 (23HS30-2804S)
    motor->setStepsPerRevolution(200);
    motor->setStepFactor(32);
    motor->setDirection(bbbkit::StepperMotor::DIRECTION::CLOCKWISE);

    // Rotate clockwise 1 revolution at 12rpm (5 seconds total)
    std::cout << "1 revolution CW @ 15rpm (4 seconds)..." << std::endl;
    motor->setRevolutionsPerMinute(15.0);
    motor->rotate(360.0f);

    std::cout << "Waiting 2 seconds..." << std::endl;
    usleep(2000000);

    // Rotate clockwise 10 revolutions at 60rpm (10 seconds total)
    std::cout << "10 revolutions CW @ 60rpm (10 seconds)..." << std::endl;
    motor->setRevolutionsPerMinute(500.0);
    motor->rotate(360.0 * 10.0);

    std::cout << "Waiting 2 seconds..." << std::endl;
    usleep(2000000);

    // Rotate counterclockwise 10 revolutions at 100rpm (6 seconds total)
    std::cout << "10 revolutions CCW @ 100rpm (6 seconds)..." << std::endl;
    motor->setRevolutionsPerMinute(100.0);
    motor->setDirection(bbbkit::StepperMotor::DIRECTION::COUNTERCLOCKWISE);
    motor->rotate(360.0 * 10.0);

    std::cout << "Waiting 2 seconds..." << std::endl;
    usleep(2000000);

    // Motor sleep for 5 seconds

    std::cout << "Sleeping motor for 5 seconds..." << std::endl;
    motor->setIsSleeping(true);
    usleep(5000000);

    // Motor wake for 5 seconds

    std::cout << "Waking motor for 5 seconds..." << std::endl;
    motor->setIsSleeping(false);
    usleep(5000000);

    std::cout << "StepperMotor test complete!" << std::endl;

    delete motor;

    return 0;
}

int testPWM() {
    std::cout << "Testing PWM:" << std::endl;

    bbbkit::PWM *pwm = new bbbkit::PWM(bbbkit::PWM::PIN::P8_13);

    std::cout << "Setting frequency..." << std::endl;
    pwm->setFrequency(1000000); // 1 MHz

    std::cout << "Starting..." << std::endl;
    pwm->start();

    std::cout << "Running for 5 seconds @ 50%..." << std::endl;
    pwm->setDutyCycleAsPercent(50.0f); // 50%
    usleep(5000000);

    std::cout << "Running for 5 seconds @ 75%..." << std::endl;
    pwm->setDutyCycleAsPercent(75.0f); // 75%
    usleep(5000000);

    std::cout << "Stopping..." << std::endl;
    pwm->stop();
    delete pwm;

    return 0;
}

int main(int argc, char **argv) {
    //testStepperMotor();
    testPWM();
    return 0;
}

