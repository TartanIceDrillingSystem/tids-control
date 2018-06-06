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

#include "HX711.h"

#include <chrono>
#include <cstdint>
#include <thread>

// Length of HX711 data to read, in bits
#define HX711_DATA_LENGTH 24

namespace tids {

HX711::HX711(bbbkit::GPIO::PIN pinDOUT, bbbkit::GPIO::PIN pinPD_SCK, float scale, long offset, GAIN gain) {
    this->gpioDOUT = new bbbkit::GPIO(pinDOUT, bbbkit::GPIO::DIRECTION::INPUT);
    this->gpioPD_SCK = new bbbkit::GPIO(pinPD_SCK, bbbkit::GPIO::DIRECTION::OUTPUT);

    this->setScale(scale);
    this->setOffset(offset);
    this->setGain(gain);
}

HX711::~HX711() {
    delete this->gpioDOUT;
    delete this->gpioPD_SCK;
}

// Check if HX711 has data ready to read
bool HX711::isReady() {
    return this->gpioDOUT->getValue() == bbbkit::GPIO::VALUE::LOW;
}

// Get gain factor
HX711::GAIN HX711::getGain() {
    return this->gain;
}

// Set new gain factor after the next read
int HX711::setGain(GAIN gain) {
    this->gain = gain;
    // Perform a read in order to set the new gain factor
    this->readRaw();
    return 0;
}

// Read raw data value
long HX711::readRaw() {
    // Wait until chip is ready
    while (!this->isReady()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    uint32_t data = 0;
    for (int bitIndex = 0; bitIndex < HX711_DATA_LENGTH; bitIndex++) {
        // Read data bit by switching clock pin
        this->gpioPD_SCK->setValue(bbbkit::GPIO::VALUE::HIGH);
        uint32_t bit = static_cast<uint32_t>(this->gpioDOUT->getValue());
        this->gpioPD_SCK->setValue(bbbkit::GPIO::VALUE::LOW);

        // Construct 24-bit data
        data |= (bit << (HX711_DATA_LENGTH - 1 - bitIndex));
    }

    // Write the gain by switching clock pin
    int gainWriteClockTicks = static_cast<int>(this->gain);
    for (int gainIndex = 0; gainIndex < gainWriteClockTicks; gainIndex++) {
        this->gpioPD_SCK->setValue(bbbkit::GPIO::VALUE::HIGH);
        this->gpioPD_SCK->setValue(bbbkit::GPIO::VALUE::LOW);
    }

    // Pad 32-bits for signed data
    if (data & (0x1 << (HX711_DATA_LENGTH - 1))) {
        data |= ((uint32_t)0xFF << HX711_DATA_LENGTH);
    }

    // Cast from uint32 to int32 to long in order to preserve signed value for any size of long
    return static_cast<long>(static_cast<int32_t>(data));
}

// Read raw data value (averaged over count)
long HX711::readRaw(int count) {
    if (count < 1) {
        return 0;
    }

    // Sum raw data
    long long readSum = 0;
    for (int i = 0; i < count; i++) {
        long data = this->readRaw();
        readSum += data;
    }

    // Average over count
    return static_cast<long>(readSum / count);
}

// Read weight value for the specified offset and scale (averaged over count)
float HX711::readWeight(int count) {
    return static_cast<float>(this->readRaw(count) - this->offset) / this->getScale();
}

// Get offset
long HX711::getOffset() {
    return this->offset;
}

// Set offset
int HX711::setOffset(long offset) {
    this->offset = offset;
    return 0;
}

// Get scale
float HX711::getScale() {
    return this->scale;
}

// Set scale
int HX711::setScale(float scale) {
    this->scale = scale;
    return 0;
}

// Zero HX711 by setting the current weight as the offset (averaged over count)
int HX711::tare(int count) {
    long rawAvg = this->readRaw(count);
    return this->setOffset(rawAvg);
}

// Power down HX711
void HX711::powerDown() {
    // Hold clock pin high
    this->gpioPD_SCK->setValue(bbbkit::GPIO::VALUE::LOW);
    this->gpioPD_SCK->setValue(bbbkit::GPIO::VALUE::HIGH);
}

// Wake up HX711
void HX711::powerUp() {
    // Reset clock pin low
    this->gpioPD_SCK->setValue(bbbkit::GPIO::VALUE::LOW);
}

} /* namespace tids */
