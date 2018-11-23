#include "ota.h"
#include "globals.h"
#include "logging.h"

#include <Arduino.h>
#include <ArduinoOTA.h>

using namespace Config;
using namespace Logging;

void setupOTA()
{
  loggingPrint("Proceeding. Initializing ArduinoOTA. ");
  ArduinoOTA.setPort(otaPort);
  ArduinoOTA.setHostname(otaHostname);
  ArduinoOTA.onStart([]() { loggingPrintln("OTA Update Start"); });
  ArduinoOTA.onEnd([]() { loggingPrintln("\n OTA UPdate End"); });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    /* Serial.printf("Progress: %u%%\r", (progress / (total / 100))); */
  });
  ArduinoOTA.onError([](ota_error_t error) {
    /* Serial.printf("Error[%u]: ", error); */
    if (error == OTA_AUTH_ERROR)
    {
      loggingPrintln("Auth Failed");
    }
    else if (error == OTA_BEGIN_ERROR)
    {
      loggingPrintln("Begin Failed");
    }
    else if (error == OTA_CONNECT_ERROR)
    {
      loggingPrintln("Connect Failed");
    }
    else if (error == OTA_RECEIVE_ERROR)
    {
      loggingPrintln("Receive Failed");
    }
    else if (error == OTA_END_ERROR)
    {
      loggingPrintln("End Failed");
    }
  });
}

void beginOTA()
{
  ArduinoOTA.begin();
}

void handleOTA()
{
  ArduinoOTA.handle();
}
