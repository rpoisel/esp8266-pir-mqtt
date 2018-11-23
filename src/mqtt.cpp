#include "mqtt.h"
#include "globals.h"
#include "states.h"
#include "logging.h"

using namespace Config;
using namespace Globals;
using namespace States;
using namespace Logging;

static void mqttReconnect();
static void mqttCallback(char* topic, byte* payload, unsigned int length);
static void printMqttMsg(char* topic, byte* payload, unsigned int length);

static void mqttCallback(char* topic, byte* payload, unsigned int length)
{
  printMqttMsg(topic, payload, length);

  if (length == 0)
  {
    return;
  }

  if (strcmp(topic, mqttTopicLightSet))
  {
    loggingPrintln("MQTT: Unknown topic");
    return;
  }

  if (payload[0] != '0' && payload[0] != '1')
  {
    loggingPrintln("MQTT: Unknown value");
    return;
  }

  fsm.event(EVENT_MQTT);
}

void setupMQTT()
{
  client.setServer(mqttBroker, mqttPort);
  client.setCallback(mqttCallback);
}

void loopMQTT()
{
  if (client.connected())
  {
    client.loop();
  }
  else
  {
    mqttReconnect();
  }
}

static void mqttReconnect()
{
  if (!client.connected())
  {
    loggingPrint("Attempting MQTT connection...");
    boolean success;
    if (mqttUser && mqttPassword)
    {
      success = client.connect(mqttClientId, mqttUser, mqttPassword);
    }
    else
    {
      success = client.connect(mqttClientId);
    }
    if (success)
    {
      loggingPrintln("connected");
      client.subscribe(mqttTopicLightSet);
    }
    else
    {
      loggingPrint("failed, rc=");
      loggingPrintln(client.state());
    }
  }
}

static void printMqttMsg(char* topic, byte* payload, unsigned int length)
{
  loggingPrint("MQTT message: '");
  for (unsigned int cnt = 0; cnt < length; cnt++)
  {
    loggingPrint((char)payload[cnt]);
  }
  loggingPrint("' on topic '");
  loggingPrint(topic);
  loggingPrintln("'");
}
