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

#ifndef LIBRARY_GPIO_H_
#define LIBRARY_GPIO_H_

#include <fstream>
#include <string>

using std::ofstream;
using std::string;

#define GPIO_PATH "/sys/class/gpio/"

namespace tids {

typedef int (*CallbackFunction_t)(int);

class GPIO {
public:
    enum DIRECTION{ INPUT, OUTPUT };
    enum VALUE{ LOW=0, HIGH=1 };
    enum EDGE{ NONE=0, RISING=1, FALLING=2, BOTH=3 };

private:
    int pin;
    string path;
    string name;
    int debounce;
    CallbackFunction_t callbackFunction;
    pthread_t thread;
    bool threadRunning;
    ofstream stream;

public:
    GPIO(int pin, GPIO::VALUE activeState=HIGH);
    virtual ~GPIO();

    // Getters and setters
    virtual int pin() { return pin; }
    virtual string path() { return path; }
    virtual string name() { return name; }
    
    virtual int debounce() { return debounce; }
    virtual int setDebounce(int debounce) { this->debounce = debounce; }

    virtual GPIO::DIRECTION direction();
    virtual int setDirection(GPIO::DIRECTION direction);

    virtual GPIO::VALUE activeState();
    virtual int setActiveState(GPIO::VALUE value);

    // General input
    virtual GPIO::VALUE value();

    // General output
    virtual int setValue(GPIO::VALUE value);

    // Edge input
    virtual GPIO::EDGE edgeType();
    virtual int setEdgeType(GPIO::EDGE edgeType);
    virtual int waitForEdge();
    virtual int waitForEdgeThread(CallbackFunction_t callbackFunction);
    virtual int waitForEdgeThreadCancel() { this->threadRunning = false; }

    // Stream output
    virtual int streamOpen();
    virtual int streamSetValue(GPIO::VALUE value);
    virtual int streamClose();

private:
    int exportPin();
    int unexportPin();
    friend void *threadEdgePoll(void *value);
};

void *threadEdgePoll(void *value);

} /* namespace tids */

#endif /* LIBRARY_GPIO_H_ */
