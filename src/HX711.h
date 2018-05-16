/*
    Tartan Ice Drilling System (TIDS) for autonomous martian ice extraction.
    Copyright (C) 2018 Devin Gund (https://dgund.com)

    Driver for the HX711 Load Cell Amplifier
    http://cdn.sparkfun.com/datasheets/Sensors/ForceFlex/hx711_english.pdf

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

#ifndef HX711_H
#define HX711_H

#include <libbbbkit/GPIO.h>

namespace tids {

class HX711 {
public:
    enum GAIN {
        A_128 = 1,
        B_32 = 2,
        A_64 = 3,
    };
private:
    bbbkit::GPIO *gpioDOUT;
    bbbkit::GPIO *gpioPD_SCK;
    GAIN gain;
    long offset;
    float scale;
public:
    HX711(bbbkit::GPIO::PIN pinDOUT, bbbkit::GPIO::PIN pinPD_SCK, GAIN gain=A_128, long offset=0, float scale=1.0f);
    virtual ~HX711();

    // Check if HX711 has data ready to read
    bool isReady();

    // Get gain factor
    int getGain();

    // Set new gain factor after the next read
    int setGain(GAIN gain);

    // Read raw data value
    long readRaw();

    // Read raw data value (averaged over count)
    long readRaw(int count);

    // Read weight value for the specified offset and scale (averaged over count)
    float readWeight(int count=1);

    // Get offset
    long getOffset();

    // Set offset
    int setOffset(long offset);

    // Get scale
    float getScale();

    // Set scale
    int setScale(float scale);

    // Zero HX711 by setting the current weight as the offset (averaged over count)
    int tare(int count=1);

    // Power down HX711
    void powerDown();

    // Wake up HX711
    void powerUp();
};

} /* namespace tids */

#endif /* HX711_H */
