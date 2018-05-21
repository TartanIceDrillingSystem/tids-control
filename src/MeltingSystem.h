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

#ifndef MELTINGSYSTEM_H
#define MELTINGSYSTEM_H

#include <atomic>
#include <thread>

#include "DS3218.h"
#include "MLX90614.h"
#include "PowerController.h"

namespace tids {

class MeltingSystem {
private:
    PowerController *powerController;
    DS3218 *capMotor;
    MLX90614 *thermometer;

    std::thread regulateTemperatureThread;
    std::atomic<bool> regulateTemperatureThreadShouldCancel;
public:
    MeltingSystem(PowerController* powerController, DS3218 *capMotor, MLX90614 *thermometer);
    virtual ~MeltingSystem();

    // Open melting chamber cap
    int openCap();

    // Close melting chamber cap
    int closeCap();

    // Start heater and chiller and adjust based on thermometer
    int start();

    // Stop heater and chiller
    int stop();

private:
    // Continuously turn the heater on/off to regulate evaporation temperature
    void regulateTemperature();
};

} /* namespace tids */

#endif /* MELTINGSYSTEM_H */
