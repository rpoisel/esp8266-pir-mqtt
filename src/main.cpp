#include "components.h"

#include <ArduinoOTA.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>

#include <utility>

using namespace Components;

namespace {

// configuration
static int const gpioPin = 14;
static int const relayPin = 12;
static int const ledPin = 13;

std::pair<char const*, char const*> wifiCredentials[] = {
    // {"SSID", "PASSWORD"},
};

#define MQTT_TOPIC_BASE "/homeautomation/bad/"
static char const* mqttTopicOverrule = MQTT_TOPIC_BASE "overrule";
static char const* mqttTopicLight = MQTT_TOPIC_BASE "light";
static char const* mqttClientId = "client-linuxmagazin";
static char const* mqttBroker = "192.168.88.241";
static int const mqttPort = 1883;
static char const* mqttUser = 0;
static char const* mqttPassword = 0;

static uint16_t const otaPort = 8266;
static char const* otaHostname = "esp_bad_eg";

static ESP8266WiFiMulti wifiMulti;
static WiFiClient wificlient;
static PubSubClient client(wificlient);
static void lightOn(void);
static void lightOff(void);
static void motionDetected(void);
static void onPeriodEnded(void);
static Delayer delayer(5000, &motionDetected, &onPeriodEnded);
static bool overruleActive = false;

static void setupIO(void);
static void setupWIFI(void);
static void setupOTA(void);
static void setupMQTT(void);

static void mqttCallback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("MQTT message: '");
  for (unsigned int cnt = 0; cnt < length; cnt++)
  {
    Serial.print((char)payload[cnt]);
  }
  Serial.print("' on topic '");
  Serial.print(topic);
  Serial.println("'");

  if (length == 0)
  {
    return;
  }

  if (payload[0] != '0' && payload[0] != '1')
  {
    Serial.print("MQTT: Unknown value");
    return;
  }

  if (!strcmp(topic, mqttTopicOverrule))
  {
    switch (payload[0])
    {
    case '0':
      overruleActive = false;
      break;
    case '1':
      overruleActive = true;
      break;
    default:
      break;
    }
  }
  else if (overruleActive && !strcmp(topic, mqttTopicLight))
  {
    switch (payload[0])
    {
    case '0':
      lightOff();
      break;
    case '1':
      lightOn();
      break;
    default:
      break;
    }
  }
}

static void mqttReconnect()
{
  if (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
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
      Serial.println("connected");
      client.subscribe(MQTT_TOPIC_BASE "#");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
    }
  }
}

static void setupIO(void)
{
  Serial.println("Initializing inputs and outputs.");
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(gpioPin, INPUT);

  digitalWrite(ledPin, HIGH);
  digitalWrite(relayPin, LOW);

  delayer.init(digitalRead(gpioPin));
}

static void setupWIFI(void)
{
  Serial.println("Initializing WiFi credentials.");
  for (auto credentialPair : wifiCredentials)
  {
    wifiMulti.addAP(credentialPair.first, credentialPair.second);
  }
}

static void setupOTA(void)
{
  Serial.print("Proceeding. Initializing ArduinoOTA. ");
  ArduinoOTA.setPort(otaPort);
  ArduinoOTA.setHostname(otaHostname);
  ArduinoOTA.onStart([]() { Serial.println("OTA Update Start"); });
  ArduinoOTA.onEnd([]() { Serial.println("\n OTA UPdate End"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
    {
      Serial.println("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      Serial.println("Begin Failed");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      Serial.println("Connect Failed");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      Serial.println("Receive Failed");
    }
    else if (error == OTA_END_ERROR)
    {
      Serial.println("End Failed");
    }
  });
}

static void setupMQTT(void)
{
  client.setServer(mqttBroker, mqttPort);
  client.setCallback(mqttCallback);
}

static void motionDetected(void)
{
  if (overruleActive)
  {
    return;
  }
  lightOn();
  if (client.connected())
  {
    client.publish(mqttTopicLight, "1");
  }
}

static void onPeriodEnded(void)
{
  if (overruleActive)
  {
    return;
  }
  lightOff();
  if (client.connected())
  {
    client.publish(mqttTopicLight, "0");
  }
}

static void lightOn(void)
{
  digitalWrite(ledPin, LOW); // LED is active-low, so this turns it on
  digitalWrite(relayPin, HIGH);
}

static void lightOff(void)
{
  digitalWrite(ledPin, HIGH); // LED is active-low, so this turns it off
  digitalWrite(relayPin, LOW);
}

} // namespace

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");

  setupIO();
  setupWIFI();
  setupOTA();
  setupMQTT();
}

void loop()
{
  static auto connectionConfigured = false;
  delayer.tick(digitalRead(gpioPin));

  if (wifiMulti.run() == WL_CONNECTED)
  {
    if (!connectionConfigured)
    {
      ArduinoOTA.begin();
      Serial.println("WiFi Ready");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
      connectionConfigured = true;
    }
    if (client.connected())
    {
      client.loop();
    }
    else
    {
      mqttReconnect();
    }
  }
  else
  {
    connectionConfigured = false;
  }

  ArduinoOTA.handle();
}
