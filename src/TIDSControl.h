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

#ifndef TIDSCONTROL_H
#define TIDSCONTROL_H

#include <libbbbkit/DCMotor.h>

#include "CVD524K.h"
#include "DrillingSystem.h"
#include "DS3218.h"
#include "HX711.h"
#include "ISNAILVC10.h"
#include "L298N.h"
#include "LJ12A34ZBY.h"
#include "LTS6NP.h"
#include "MeltingSystem.h"
#include "MLX90614.h"
#include "MMPEU.h"
#include "PowerController.h"
#include "SteppedLeadscrew.h"
#include "TelemetrySystem.h"
#include "XPositioningAxis.h"
#include "ZPositioningAxis.h"

namespace tids {

#define TIDS_POWERCONTROLLER_PIN_RELAYCHILLER_GPIO bbbkit::GPIO::PIN::P8_42 // Relay 1
#define TIDS_POWERCONTROLLER_PIN_RELAYHEATER1_GPIO bbbkit::GPIO::PIN::P8_40 // Relay 2
#define TIDS_POWERCONTROLLER_PIN_RELAYHEATER2_GPIO bbbkit::GPIO::PIN::P8_38 // Relay 3
#define TIDS_POWERCONTROLLER_PIN_RELAYDRILLMOTOR_GPIO bbbkit::GPIO::PIN::P8_36 // Relay 4
#define TIDS_POWERCONTROLLER_PIN_RELAYMOTORZ_GPIO bbbkit::GPIO::PIN::P8_34 //Relay 5
#define TIDS_POWERCONTROLLER_PIN_RELAYPROXIMITYSENSORS_GPIO bbbkit::GPIO::PIN::P8_32 // Relay 6
#define TIDS_POWERCONTROLLER_PIN_RELAYMOTORX_GPIO bbbkit::GPIO::PIN::P8_30 // Relay 7

#define TIDS_CURRENTSENSOR_PIN_ADC bbbkit::ADC::PIN::P9_39

#define TIDS_LOADCELL_PIN_DOUT_GPIO bbbkit::GPIO::PIN::P8_27
#define TIDS_LOADCELL_PIN_PD_SCK_GPIO bbbkit::GPIO::PIN::P8_29

#define TIDS_DRILLMOTOR_PIN_PWM bbbkit::PWM::PIN::P9_14

#define TIDS_DRILLENCODER_PIN_A_GPIO bbbkit::GPIO::PIN::P8_8
#define TIDS_DRILLENCODER_PIN_B_GPIO bbbkit::GPIO::PIN::P8_7
#define TIDS_DRILLENCODER_PIN_INDEX_GPIO bbbkit::GPIO::PIN::P8_9

#define TIDS_DRILLCURRENTSENSOR_PIN_ADC bbbkit::ADC::PIN::P9_40

#define TIDS_MOTORX_PIN_PLS_GPIO bbbkit::GPIO::PIN::P8_31
#define TIDS_MOTORX_PIN_DIR_GPIO bbbkit::GPIO::PIN::P8_33
#define TIDS_MOTORX_PIN_AWO_GPIO bbbkit::GPIO::PIN::P8_35
#define TIDS_MOTORX_PIN_CS_GPIO bbbkit::GPIO::PIN::P8_37
#define TIDS_MOTORX_PIN_ALM_GPIO bbbkit::GPIO::PIN::P8_39
#define TIDS_MOTORX_PIN_TIM_GPIO bbbkit::GPIO::PIN::P8_41

#define TIDS_PROXIMITYSENSORXHOME_PIN_GPIO bbbkit::GPIO::PIN::P9_23

#define TIDS_MOTORZ_PIN_ENA_PWM bbbkit::PWM::PIN::P8_13
#define TIDS_MOTORZ_PIN_IN1_GPIO bbbkit::GPIO::PIN::P8_14
#define TIDS_MOTORZ_PIN_IN2_GPIO bbbkit::GPIO::PIN::P8_16

#define TIDS_PROXIMITYSENSORZHOME_PIN_GPIO bbbkit::GPIO::PIN::P9_24
#define TIDS_PROXIMITYSENSORZBOTTOM_PIN_GPIO bbbkit::GPIO::PIN::P9_25

#define TIDS_HEATERCAPMOTOR_PIN_PWM bbbkit::PWM::PIN::P9_16

#define TIDS_HEATERTHERMOMETER_BUS_I2C bbbkit::I2C::BUS::I2C_2 // SCL: P9_19, SDA: P9_20

class TIDSControl {
private:
    PowerController *powerController;

    ISNAILVC10 *currentSensor;
    HX711 *loadCell;
    TelemetrySystem *telemetrySystem;

    DrillingSystem *drillingSystem;
    bbbkit::DCMotor *drillMotor;
    MMPEU *drillEncoder;
    LTS6NP *drillCurrentSensor;

    XPositioningAxis *xAxis;
    CVD524K *xAxisMotor;
    LJ12A34ZBY *proximitySensorXHome;

    ZPositioningAxis *zaxis;
    L298N *zAxisMotor;
    LJ12A34ZBY *proximitySensorZHome;
    LJ12A34ZBY *proximitySensorZBottom;

    MeltingSystem *meltingSystem;
    DS3218 *heaterCapMotor;
    MLX90614 *heaterThermometer;
public:
    TIDSControl();
    virtual ~TIDSControl();

    void run();

    float getCurrent();

    float getDrillCurrent();
    float getDrillSpeed();
    float getDrillTorque();

    float getAxisXLocation();

    float getAxisZLocation();

    float getHeaterTemperature();

    int testPowerController();
    int testCurrentSensor();
    int testLoadCell();
    int testDrillMotor();
    int testDrillMotorAndEncoder();
    int testDrillCurrentSensor();
    int testAxisX();
    int testAxisZ();
    int testHeater();
    int testHeaterCapMotor();
    int testHeaterThermometer();
};

} /* namespace tids */

#endif /* TIDSCONTROL_H */
