#ifndef MQTT_H
#define MQTT_H

#include "globals.h"

void setupMQTT();
void loopMQTT();
#if DEBUG==1
void printMqttMsg(char* topic, byte* payload, unsigned int length);
#endif

#endif /* MQTT_H */
