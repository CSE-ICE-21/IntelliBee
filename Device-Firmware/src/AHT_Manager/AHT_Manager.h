#ifndef AHT_MANAGER
#define AHT_MANAGER

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include "structs.h"
#include "definitions.h"
#include "configurations.h"

class AHT_Manager : public Manager
{
public:
    explicit AHT_Manager(Context *context);
    ~AHT_Manager();
    void execute() override; // Correctly overrides now.

protected:
    status_t read_data();

private:
    Adafruit_AHTX0 aht_in;
    Adafruit_AHTX0 aht_out;
    TwoWire *i2c_in;
    TwoWire *i2c_out;
};

#endif // AHT_MANAGER