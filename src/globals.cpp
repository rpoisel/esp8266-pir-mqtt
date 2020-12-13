#include "globals.h"

using namespace Components;

namespace Config {

int const relayPin = 12;
int const gpioPin = 14;
int const ledPin = 13;

std::vector<std::pair<char const*, char const*>> const wifiCredentials = {
    // {"SSID", "PASSWORD"},
};

#define MQTT_TOPIC_BASE "/homeautomation/bad/light/"
char const* mqttTopicLightSet = MQTT_TOPIC_BASE "set";
char const* mqttTopicLightState = MQTT_TOPIC_BASE "state";
char const* mqttClientId = "client-linuxmagazin";
char const* mqttBroker = "192.168.88.241";
int const mqttPort = 1883;
char const* mqttUser = nullptr;
char const* mqttPassword = nullptr;

uint16_t const otaPort = 8266;
char const* otaHostname = "esp_bad_eg";

}

namespace Globals {

ESP8266WiFiMulti wifiMulti;
WiFiClient wificlient;
PubSubClient client(wificlient);

Timer motionTimer(3 /* min */ * 60 /* sec/min */ * 1000 /* ms/sec */);
FSM fsm;
Light light(Config::relayPin);

} // namespace Globals
