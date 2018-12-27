#include "components.h"
#include "states.h"
#include "globals.h"
#include "mqtt.h"
#include "ota.h"
#include "logging.h"

using namespace Config;
using namespace Components;
using namespace States;
using namespace Globals;
using namespace Logging;

namespace {

static void motionCb(void);
static MotionSensor motionSensor(Config::gpioPin, motionCb);
static void setupWIFI(void);

} // namespace

void setup()
{
  setupLogging(true);
  loggingBegin();
  loggingPrintln("Booting");

  motionSensor.begin();
  light.begin();

  setupWIFI();
  setupOTA();
  setupMQTT();

  fsm.setState(LightOff::Instance());
}

void loop()
{
  static auto connectionConfigured = false;

  if (wifiMulti.run() == WL_CONNECTED)
  {
    if (!connectionConfigured)
    {
      beginOTA();
      loggingPrintln("WiFi Ready");
      loggingPrint("IP address: ");
      loggingPrintln(WiFi.localIP());
      connectionConfigured = true;
    }
    loopMQTT();
  }
  else
  {
    connectionConfigured = false;
  }

  motionSensor.loop();
  fsm.update();
  handleOTA();
}

namespace {

static void setupWIFI(void)
{
  loggingPrintln("Initializing WiFi credentials.");
  for (auto credentialPair : wifiCredentials)
  {
    wifiMulti.addAP(credentialPair.first, credentialPair.second);
  }
}

static void motionCb(void)
{
  fsm.event(EVENT_MOTION);
}

} // namespace
