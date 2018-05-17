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

#include "TIDSControl.h"

#include <iostream>
#include <unistd.h>

TIDSControl::TIDSControl() {
    this->powerController = new PowerController(TIDS_POWERCONTROLLER_PIN_RELAYCHILLER_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYDRILLMOTOR_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYHEATER1_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYHEATER2_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYPROXIMITYSENSORS_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYSTEPEPRMOTORX_GPIO,
                                                TIDS_POWERCONTROLLER_PIN_RELAYSTEPPERMOTORZ_GPIO);
    this->currentSensor = new ISNAILVC10(TIDS_CURRENTSENSOR_PIN_ADC);

    this->drillMotor = new bbbkit::DCMotor(TIDS_DRILLMOTOR_PIN_PWM, 1000, 0.0);

    this->drillEncoder = new MMPEU(TIDS_DRILLENCODER_PIN_A_GPIO,
                                    TIDS_DRILLENCODER_PIN_B_GPIO,
                                    TIDS_DRILLENCODER_PIN_INDEX_GPIO);

    this->drillCurrentSensor = new LTS6NP(TIDS_DRILLCURRENTSENSOR_PIN_ADC);

    this->drillLoadCell = new HX711(TIDS_DRILLLOADCELL_PIN_DOUT_GPIO,
        TIDS_DRILLLOADCELL_PIN_PD_SCK_GPIO,
        -56500.0f);

    this->stepperMotorX = new CVD525K(TIDS_STEPPERMOTORX_PIN_PLS_GPIO,
                                        TIDS_STEPPERMOTORX_PIN_DIR_GPIO,
                                        TIDS_STEPPERMOTORX_PIN_AWO_GPIO,
                                        TIDS_STEPPERMOTORX_PIN_CS_GPIO,
                                        TIDS_STEPPERMOTORX_PIN_ALM_GPIO,
                                        TIDS_STEPPERMOTORX_PIN_TIM_GPIO);

    this->proximitySensorXHome = new LJ12A34ZBY(TIDS_PROXIMITYSENSORXHOME_PIN_GPIO);

    this->axisX = new SteppedLeadscrew(this->stepperMotorX, 3.0);

    this->stepperMotorZ = new TB6600(TIDS_STEPPERMOTORZ_PIN_PUL_GPIO,
                                        TIDS_STEPPERMOTORZ_PIN_DIR_GPIO,
                                        TIDS_STEPPERMOTORZ_PIN_ENA_GPIO);

    this->proximitySensorZHome = new LJ12A34ZBY(TIDS_PROXIMITYSENSORZHOME_PIN_GPIO);

    this->proximitySensorZBottom = new LJ12A34ZBY(TIDS_PROXIMITYSENSORZBOTTOM_PIN_GPIO);

    this->axisZ = new SteppedLeadscrew(this->stepperMotorZ, 4.0);

    this->headerCapMotor = new DS3218(TIDS_HEATERCAPMOTOR_PIN_PWM);

    this->heaterThermometer = new MLX90614(TIDS_HEATERTHERMOMETER_BUS_I2C);
}

TIDSControl::~TIDSControl() {
    delete this->powerController;
    delete this->currentSensor;

    delete this->drillMotor;
    delete this->drillEncoder;
    delete this->drillCurrentSensor;
    delete this->drillLoadCell;

    delete this->stepperMotorX;
    delete this->proximitySensorXHome;
    delete this->axisX;

    delete this->stepperMotorZ;
    delete this->proximitySensorZHome;
    delete this->proximitySensorZBottom;
    delete this->axisZ;

    delete this->heaterThermometer;
}

int TIDSControl::testPowerController() {
    this->powerController->turnOffAllRelays();
    return 0;
}

int TIDSControl::testCurrentSensor() {
    return 0;
}

int TIDSControl::testDrillMotor() {
    return 0;
}

int TIDSControl::testDrillMotorAndEncoder() {
    return 0;
}

int TIDSControl::testDrillCurrentSensor() {
    return 0;
}

int TIDSControl::testDrillLoadCell() {
    return 0;
}

int TIDSControl::testAxisX() {
    return 0;
}

int TIDSControl::testAxisZ() {
    return 0;
}

int TIDSControl::testHeater() {
    this->powerController->turnOffAllRelays();
    this->powerController->setHeaterRelayState(PowerController::STATE::ON);
    usleep(10 * 000000);
    this->powerController->setHeaterRelayState(PowerController::STATE::OFF);
    return 0;
}

int TIDSControl::testHeaterCapMotor() {
    return 0;
}

int TIDSControl::testHeaterThermometer() {
    return 0;
}
