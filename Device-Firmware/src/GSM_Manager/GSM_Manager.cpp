#include "GSM_Manager.h"

GSM_Manager::GSM_Manager(Context *context)
{
    this->context = context;
    this->modem = new TinyGsm(Serial2);
    this->client = new TinyGsmClient(*this->modem);
    this->mqtt = new PubSubClient(*this->client);
    this->apn = "mobitel";
    this->ntpServer = "pool.ntp.org";
}

GSM_Manager::~GSM_Manager()
{
    delete this->modem;
    delete this->client;
    delete this->mqtt;
}

void GSM_Manager::publish(const char *topic, const char *message)
{
    this->mqtt->publish(topic, message);
}

void GSM_Manager::subscribe(const char *topic)
{
    this->mqtt->subscribe(topic);
}

void MQTTCallback(char *topic, byte *payload, unsigned int length)
{
    SerialMon.print("Message arrived [");
    SerialMon.print(topic);
    SerialMon.print("]: ");
    for (int i = 0; i < length; i++)
    {
        SerialMon.print((char)payload[i]);
    }
    SerialMon.println();
}

void GSM_Manager::connect()
{
    SerialMon.println("Wait...");
    this->modem->setBaud(115200);
    delay(6000);
    SerialMon.println("Initializing modem...");
    this->modem->restart();
    String modemInfo = this->modem->getModemInfo();
    SerialMon.println(modemInfo);
    SerialMon.print("Waiting for network...");
    if (!this->modem->waitForNetwork())
    {
        SerialMon.println(" failed waiting for network");
        delay(10000);
    }
    SerialMon.println(" successfully waited for network");

    if (this->modem->isNetworkConnected())
    {
        SerialMon.println("Network connected succesfully!");
    }
    else
    {
        SerialMon.println("Network connection failed!");
    }

    delay(1000);

    SerialMon.print(F("Connecting to "));
    SerialMon.print(this->apn);
    if (!this->modem->gprsConnect(this->apn))
    {
        SerialMon.println(" fail");
        delay(10000);
        return;
    }

    SerialMon.println(" success");

    if (this->modem->isGprsConnected())
    {
        SerialMon.println("GPRS connected");
    }

    configTime(0, 19800, this->ntpServer);
    delay(5000);
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo))
    {
        Serial.println("Failed to get local Time in sync");
    }

    this->mqtt->setServer(MQTT_SERVER, MQTT_PORT);
    this->mqtt->setCallback(MQTTCallback);
}
