#include "Arduino.h"
#include "ControllerInput.h"
#include "strucst.h"

const int CONTROLLER_INPUT_TIMEOUT = 30000;    // The timeout after which a default value is returned
const int MIN_CHANNEL_VALUE = 100;             // The minimum value read from a pin, if lower the default is returned
const int CONTROLLER_INPUT_START_RANGE = 1000; // Readout minimum value range
const int CONTROLLER_INPUT_END_RANGE = 2000;   // Readout maximum value range
const int SMOOTHEN_DIVISION_FACTOR = 2;        // Smoothness of the smoothen function (x / smoothness)

ControllerInput::ControllerInput(PinConfig pinConfig, int minReadValue, int maxReadValue, int standardDefaultValue, int throttleDefaultValue, bool smoothen)
{
    this->throttlePin = pinConfig.throttlePin;
    this->steeringPin = pinConfig.steeringPin;
    this->longitudinalPin = pinConfig.longitudinalPin;
    this->latitudinalPin = pinConfig.latitudinalPin;
    this->minReadValue = minReadValue;
    this->maxReadValue = maxReadValue;
    this->standardDefaultValue = standardDefaultValue;
    this->throttleDefaultvalue = throttleDefaultValue;
    this->smoothenInput = smoothen;

    this->setPinsAsOutput();
}

ControllerInput::~ControllerInput()
{
}

void ControllerInput::setPinsAsOutput()
{
    pinMode(this->throttlePin, INPUT);
    pinMode(this->steeringPin, INPUT);
    pinMode(this->longitudinalPin, INPUT);
    pinMode(this->latitudinalPin, INPUT);
}

/**
 * @brief Reads out a pin and returns the value mapped between the provided min and max.
 *
 * @param pin The pin to read out
 * @param minLimit The minimum value for the readout
 * @param maxLimit The maximum value for the readout
 * @param defaultValue The fallback value if nothing can be read
 * @return int  An value indicating the pins read value
 */
int ControllerInput::getReadoutFromPin(int pin, int minLimit, int maxLimit, int defaultValue)
{
    int ch = pulseIn(pin, HIGH, CONTROLLER_INPUT_TIMEOUT);
    if (ch < MIN_CHANNEL_VALUE)
        return defaultValue;
    return map(ch, CONTROLLER_INPUT_START_RANGE, CONTROLLER_INPUT_END_RANGE, minLimit, maxLimit);
}

/**
 * @brief Smoothens between two values, can be used if input readout is too jerky
 *
 * @param oldValue The old value to smoothen from
 * @param newValue The new value to smoothen to
 * @return int Smoothened value
 */
int ControllerInput::smoothen(int oldValue, int newValue)
{
    if (!smoothenInput)
        return newValue;

    return (oldValue + newValue) / SMOOTHEN_DIVISION_FACTOR;
}

// Helper function for getting the throttle value
int ControllerInput::getThrottleValue()
{
    return this->getReadoutFromPin(this->throttlePin, this->minReadValue, this->maxReadValue, this->throttleDefaultvalue);
}

// Helper function for getting the steering value
int ControllerInput::getSteeringValue()
{
    return this->getReadoutFromPin(this->steeringPin, this->minReadValue, this->maxReadValue, this->standardDefaultValue);
}

// Helper function for getting the longitudinal value
int ControllerInput::getLongitudinalValue()
{
    return this->getReadoutFromPin(this->longitudinalPin, this->minReadValue, this->maxReadValue, this->standardDefaultValue);
}

// Helper function for getting the latitudinal value
int ControllerInput::getLatitudinalValue()
{
    return this->getReadoutFromPin(this->latitudinalPin, this->minReadValue, this->maxReadValue, this->standardDefaultValue);
}

// Helper function for updating te local output values
void ControllerInput::updateInputValues()
{
    this->throttleValue = smoothen(this->throttleValue, this->getThrottleValue());
    this->steeringValue = smoothen(this->steeringValue, this->getSteeringValue());
    this->longitudinalValue = smoothen(this->longitudinalValue, this->getLongitudinalValue());
    this->lateralValue = smoothen(this->lateralValue, this->getLatitudinalValue());
}