#include "AHT_Manager.h"

AHT_Manager::AHT_Manager(Context *context)

{
    this->context = context;
    this->i2c_in = new TwoWire(0);
    this->i2c_out = new TwoWire(1);

    this->i2c_in->begin(SDA_AHT_IN, SCL_AHT_IN, I2C_CLOCK);
    this->i2c_out->begin(SDA_AHT_OUT, SCL_AHT_OUT, I2C_CLOCK);

    bool aht_in_status = this->aht_in.begin(this->i2c_in);
    bool aht_out_status = this->aht_out.begin(this->i2c_out);

    if (!aht_out_status)
    {
        Serial.println("AHT Out initialization failed");
        this->show_error();
    }
    if (!aht_in_status)
    {
        Serial.println("AHT In initialization failed");
        this->show_error();
    } 
}

AHT_MANAGER::status_t AHT_Manager::read_data()
{
    sensors_event_t hum_in_event, hum_out_event, temp_in_event, temp_out_event;
    int current_tries = MAX_RETRIES;

    for (int i = 0; i < MEAN_CALCULATION_SIZE; i++)
    {
        bool aht_in_read_status = this->aht_in.getEvent(&hum_in_event, &temp_in_event);
        bool aht_out_read_status = this->aht_out.getEvent(&hum_out_event, &temp_out_event);

        if (aht_in_read_status == false || aht_out_read_status == false)
        {
            current_tries--;
            i--;
            if (current_tries <= 0)
            {
                return AHT_MANAGER::ERROR;
            }
            continue;
        }

        this->context->temp_in += temp_in_event.temperature;
        this->context->temp_out += temp_out_event.temperature;
        this->context->hum_in += hum_in_event.relative_humidity;
        this->context->hum_out += hum_out_event.relative_humidity;
        delay(100);
    }

    this->context->hum_in /= MEAN_CALCULATION_SIZE;
    this->context->hum_out /= MEAN_CALCULATION_SIZE;
    this->context->temp_in /= MEAN_CALCULATION_SIZE;
    this->context->temp_out /= MEAN_CALCULATION_SIZE;

    return AHT_MANAGER::OKAY;
}

AHT_Manager::~AHT_Manager()
{
    this->i2c_in->end();
    this->i2c_out->end();
    delete this->i2c_in;
    delete this->i2c_out;
}

void AHT_Manager::execute()
{
    this->context->current_state = GET_AHT_DATA;
    this->context->temp_in = 0;
    this->context->temp_out = 0;
    this->context->hum_in = 0;
    this->context->hum_out = 0;

    status_t status = this->read_data();

}

