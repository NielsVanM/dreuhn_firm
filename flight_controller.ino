/**
 * @brief
 * FlightController firmware for dreuhn
 */

#include "src/ControllerInput.h"
#include "src/strucst.h"

/**
 * @brief Arduino Uno Wifi Pin Config
 *
 */
PinConfig pinConfig = PinConfig{
    A2,
    A3,
    A4,
    A5,
};

/**
 * @brief Wemos D1 Pin Config
 *
 */
// PinConfig pinConfig = PinConfig{
//     D2,
//     D3,
//     D4,
//     D5,
// };

ControllerInput controllerInput = ControllerInput(pinConfig, -100, 100, 0, -100, false);
int iterator = 0;

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    controllerInput.updateInputValues();
    iterator++;
    Serial.print(controllerInput.throttleValue);
    Serial.print(", ");
    Serial.print(controllerInput.steeringValue);
    Serial.print(", ");
    Serial.print(controllerInput.longitudinalValue);
    Serial.print(", ");
    Serial.println(controllerInput.lateralValue);
}