/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Driver for the Melexis MLX90614 infrared thermometer
    https://cdn-shop.adafruit.com/datasheets/MLX90614.pdf

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

#include "MLX90614.h"

#include <cstdint>

namespace tids {

MLX90614::MLX90614(bbbkit::I2C::BUS bus) {
    this->i2c = new bbbkit::I2C(bus, MLX90614_ADDR);
    this->i2c->open();
}

MLX90614::~MLX90614() {
    this->i2c->close();
    delete this->i2c;
}

// Get object temperature in degrees Celsius
float MLX90614::getObjectTemperature() {
    return this->getTemperature(MLX90614_TOBJ1);
}

// Get object temperature in degrees Celsius
float MLX90614::getAmbientTemperature() {
    return this->getTemperature(MLX90614_TA);
}

float MLX90614::getTemperature(char registerAddress) {
    // MLX90614 sends 0:LSB, 1:MSB, 2:PEC
    char *data = this->readRegisters(registerAddress, 3);
    
    // Construct temperature data
    uint16_t rawTemperature = data[1];
    rawTemperature = rawTemperature << 8;
    rawTemperature |= data[0];
    
    // Delete response
    delete[] data;

    // Compute temperature in Celsius
    float temperature = 0.02 * static_cast<float>(rawTemperature) - 273.15;
    return temperature;
}

} /* namespace tids */
