#include "states.h"
#include "globals.h"
#include "logging.h"

using namespace Config;
using namespace Globals;
using namespace Components;
using namespace Logging;

namespace States {

void LightOn::enter(FSM* fsm)
{
  loggingPrintln("LightOn ENTER");
  light.on();
  if (client.connected())
  {
    client.publish(mqttTopicLightState, "LightOn");
  }
  motionTimer.start();
}
void LightOn::execute(FSM* fsm)
{
  loggingPrintln("LightOn EXECUTE");
  if (motionTimer.isExpired())
  {
    fsm->setState(LightOff::Instance());
  }
}
void LightOn::event(FSM* fsm, EventID event)
{
  loggingPrint("LightOn EVENT: ");
  switch(event)
  {
    case EVENT_MQTT:
      loggingPrintln("MQTT");
      fsm->setState(LightOff::Instance());
      break;
    case EVENT_MOTION:
      loggingPrintln("MOTION");
      motionTimer.start();
      break;
    default:
      break;
  }
}
void LightOnOverride::enter(FSM* fsm)
{
  loggingPrintln("LightOnOverride ENTER");
  light.on();
  if (client.connected())
  {
    client.publish(mqttTopicLightState, "LightOnOverride");
  }
}
void LightOnOverride::event(FSM* fsm, EventID event)
{
  loggingPrint("LightOnOverride EVENT: ");
  switch(event)
  {
    case EVENT_MQTT:
      loggingPrintln("MQTT");
      fsm->setState(LightOff::Instance());
      break;
    case EVENT_MOTION:
      loggingPrintln("MOTION");
      /* ignored */
      break;
    default:
      break;
  }
}
void LightOff::enter(FSM* fsm)
{
  loggingPrintln("LightOff ENTER");
  light.off();
  if (client.connected())
  {
    client.publish(mqttTopicLightState, "LightOff");
  }
}
void LightOff::event(FSM* fsm, EventID event)
{
  loggingPrint("LightOff EVENT: ");
  switch(event)
  {
    case EVENT_MQTT:
      loggingPrintln("MQTT");
      fsm->setState(LightOnOverride::Instance());
      break;
    case EVENT_MOTION:
      loggingPrintln("MOTION");
      fsm->setState(LightOn::Instance());
      break;
    default:
      break;
  }
}

} // namespace States
