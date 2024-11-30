#ifndef GSM_MANAGER

#include <Arduino.h>

#define TINY_GSM_MODEM_SIM900

#include <TinyGsmClient.h>
#include <PubSubClient.h>

#include "structs.h"
#include "definitions.h"
#include "configurations.h"

class GSM_Manager : public Manager
{
public:
    #ifdef DUMP_AT_COMMANDS
    #include <StreamDebugger.h>
        StreamDebugger debugger(SerialAT, SerialMon);
        TinyGsm modem(debugger);
    #else
        TinyGsm *modem;
    #endif
    explicit GSM_Manager(Context *context);
    ~GSM_Manager();

private:
    PubSubClient *mqtt;
    TinyGsmClient *client;
    char *ntpServer;
    char *apn;
    void connect();
    void publish(const char *topic, const char *message);
    void subscribe(const char *topic);
    void loop();
};

#endif // GSM_MANAGER