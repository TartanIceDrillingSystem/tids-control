/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Class for power controller backed by relay
    https://en.wikipedia.org/wiki/Solid-state_relay

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

#ifndef POWERCONTROLLER_H
#define POWERCONTROLLER_H

#include <libbbbkit/GPIO.h>

namespace tids {

class PowerController {
public:
    enum STATE {
        OFF = 0,
        ON = 1,
    };
private:
    // Relay controlling power to chiller (120V AC)
    bbbkit::GPIO *gpioRelayChiller;
    
    // Relay controlling power to drill motor (90V DC)
    bbbkit::GPIO *gpioRelayDrillMotor;
    
    // Relays controlling power to proximity sensors (30V DC)
    // Divided into two relays to keep current per relay below max
    bbbkit::GPIO *gpioRelayHeater1;
    bbbkit::GPIO *gpioRelayHeater2;

    // Relay controlling power to proximity sensors (12V DC)
    bbbkit::GPIO *gpioRelayProximitySensors;

    // Relay controlling power to x-axis motor (24V DC)
    bbbkit::GPIO *gpioRelayMotorX;
    
    // Relay controlling power to z-axis motor (24V DC)
    bbbkit::GPIO *gpioRelayMotorZ;

    // Relay controlling power to 24V buck convertor that supplies power to x-axis and z-axis motors
    bbbkit::GPIO *gpioRelay24V;
    
public:
    PowerController(bbbkit::GPIO::PIN pinRelayChiller, bbbkit::GPIO::PIN pinRelayDrillMotor, bbbkit::GPIO::PIN pinRelayHeater1, bbbkit::GPIO::PIN pinRelayHeater2, bbbkit::GPIO::PIN pinRelayProximitySensors, bbbkit::GPIO::PIN pinRelayMotorX, bbbkit::GPIO::PIN pinRelayMotorZ, bbbkit::GPIO::PIN pinRelay24V);
    virtual ~PowerController();

    PowerController::STATE getChillerRelayState();
    int setChillerRelayState(PowerController::STATE state);

    PowerController::STATE getDrillMotorRelayState();
    int setDrillMotorRelayState(PowerController::STATE state);

    PowerController::STATE getHeaterRelayState();
    int setHeaterRelayState(PowerController::STATE state);

    PowerController::STATE getProximitySensorsRelayState();
    int setProximitySensorsRelayState(PowerController::STATE state);

    PowerController::STATE getMotorXRelayState();
    int setMotorXRelayState(PowerController::STATE state);

    PowerController::STATE getMotorZRelayState();
    int setMotorZRelayState(PowerController::STATE state);

    PowerController::STATE get24VRelayState();
    int set24VRelayState(PowerController::STATE state);

    int turnOffAllRelays();

private:
    PowerController::STATE getRelayState(bbbkit::GPIO *gpioRelay);
    int setRelayState(bbbkit::GPIO *gpioRelay, PowerController::STATE state);
};

} /* namespace tids */

#endif /* POWERCONTROLLER_H */
