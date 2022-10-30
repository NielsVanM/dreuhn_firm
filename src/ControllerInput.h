#ifndef ControllerInput_H
#define ControllerInput_H

class ControllerInput
{
private:
    // Pins regarding the input from an FS-IA6 receiver
    int throttlePin;
    int steeringPin;
    int longitudinalPin;
    int latitudinalPin;
    bool smoothenInput;

    // The range of values to parse input to
    int minReadValue;
    int maxReadValue;
    int standardDefaultValue;
    int throttleDefaultvalue;

    void setPinsAsOutput();
    int getReadoutFromPin(int pin, int minValue, int minLimit, int maxLimit);
    int smoothen(int oldValue, int newValue);

public:
    int throttleValue;
    int steeringValue;
    int longitudinalValue;
    int lateralValue;

    ControllerInput(PinConfig pinConfig, int minReadValue, int maxReadValue, int standardDefaultValue, int throttleDefaultValue, bool smoothen);
    ~ControllerInput();

    int getThrottleValue();
    int getSteeringValue();
    int getLongitudinalValue();
    int getLatitudinalValue();

    void updateInputValues();
};

#endif