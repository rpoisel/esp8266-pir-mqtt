#include "components.h"

#include <ArduinoOTA.h>
#include <ESP8266WiFiMulti.h>
#include <PubSubClient.h>

using namespace Components;

namespace {

static int const gpioPin = 14;
static int const relayPin = 12;
static int const ledPin = 13;

static char const* mqttTopicSubscribe = "/test/sonoff/relay";
static char const* mqttTopicPublish = "/test/sonoff/gpio";
static char const* mqttClientId = "client-linuxmagazin";
static char const* mqttBroker = "192.168.88.241";
static int const mqttPort = 1883;
static char const* mqttUser = 0;
static char const* mqttPassword = 0;

static uint16_t const otaPort = 8266;
static char const* otaHostname = "esp_bad_eg";

static uint8_t prevButtonVal;
static ESP8266WiFiMulti wifiMulti;
static WiFiClient wificlient;
static PubSubClient client(wificlient);

static void setupIO(void);
static void setupWIFI(void);
static void setupOTA(void);
static void setupMQTT(void);

static inline void blinkNum(size_t num, uint8_t pin)
{
  for (size_t cnt = 0; cnt < num; cnt++)
  {
    digitalWrite(pin, LOW);
    delay(100);
    digitalWrite(pin, HIGH);
    delay(100);
  }
}

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

  switch (payload[0])
  {
  case '1':
    digitalWrite(ledPin, LOW); // LED is active-low, so this turns it on
    digitalWrite(relayPin, HIGH);
    break;
  case '0':
    digitalWrite(ledPin, HIGH); // LED is active-low, so this turns it off
    digitalWrite(relayPin, LOW);
    break;
  default:
    Serial.print("MQTT: Unknown value");
    break;
  }
}

static void mqttReconnect()
{
  while (!client.connected())
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
      blinkNum(2, ledPin);
      client.subscribe(mqttTopicSubscribe);
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
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
  prevButtonVal = digitalRead(gpioPin);
}

static void setupWIFI(void)
{
  Serial.println("Initializing WiFi credentials.");
  // wifiMulti.addAP("SSID", "PASSWORD");
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

} // namespace

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting");

  Delayer d(5000, [&]() {}, [&]() {});
  d.init(false);
  d.tick(true);

  setupIO();
  setupWIFI();
  setupOTA();
  setupMQTT();
}

void loop()
{
  static auto connectionConfigured = false;
  auto curButtonVal = digitalRead(gpioPin);

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
      if (prevButtonVal != curButtonVal)
      {
        // movement has been detected
        client.publish(mqttTopicPublish, curButtonVal == HIGH ? "0" : "1");
      }
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
  prevButtonVal = curButtonVal;
}
