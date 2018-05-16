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

#ifndef MLX90614_H
#define MLX90614_H

#include <libbbbkit/I2C.h>

namespace tids {

// I2C device address
#define MLX90614_ADDR 0x5A

// RAM
// Raw data IR channel 1
#define MLX90614_RAWIR1 0x04
// Raw data IR channel 2
#define MLX90614_RAWIR2 0x05
// Ambient temperature
#define MLX90614_TA 0x06
// Object temperature 1
#define MLX90614_TOBJ1 0x07
// Object temperature 2
#define MLX90614_TOBJ2 0x08

// EEPROM
// Object temperature max
#define MLX90614_TOMAX 0x00
// Object temperature min
#define MLX90614_TOMIN 0x01
// PWM control
#define MLX90614_PWMCTRL 0x02
// Ambient temperature range
#define MLX90614_TARANGE 0x03
// Emissivity correction coefficient
#define MLX90614_EMISSIVITY 0x04
// Config register 1
#define MLX90614_CONFIG 0x05

class MLX90614 {
private:
    bbbkit::I2C *i2c;
public:
    MLX90614(bbbkit::I2C::BUS bus);
    virtual ~MLX90614();

    // Get object temperature in degrees Celsius
    float getObjectTemperature();

    // Get object temperature in degrees Celsius
    float getAmbientTemperature();

private:
    float getTemperature(char register);
};

} /* namespace tids */

#endif /* MLX90614_H */
