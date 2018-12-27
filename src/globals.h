#ifndef GLOBALS_H
#define GLOBALS_H

#include "components.h"

#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>

#include <utility>
#include <vector>

namespace Config {

extern int const relayPin;
extern int const gpioPin;

extern std::vector<std::pair<char const*, char const*>> const wifiCredentials;

#define MQTT_TOPIC_BASE "/homeautomation/bad/light/"
extern char const* mqttTopicLightSet;
extern char const* mqttTopicLightState;
extern char const* mqttClientId;
extern char const* mqttBroker;
extern int const mqttPort;
extern char const* mqttUser;
extern char const* mqttPassword;

extern uint16_t const otaPort;
extern char const* otaHostname;

} // namespace Config

namespace Globals {

extern ESP8266WiFiMulti wifiMulti;
extern PubSubClient client;
extern Components::Timer motionTimer;
extern Components::FSM fsm;
extern Components::Light light;

} // namespace Globals

#endif /* GLOBALS_H */
