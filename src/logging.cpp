#include "logging.h"

namespace Logging {

static bool loggingActive = false;

void setupLogging(bool activate)
{
  loggingActive = activate;
}

void loggingBegin()
{
  Serial.begin(115200);
}

void loggingPrint(char const* str)
{
  if (loggingActive)
  {
    Serial.print(str);
  }
}

void loggingPrintln(char const* str)
{
  if (loggingActive)
  {
    Serial.println(str);
  }
}

void loggingPrint(String const& str)
{
  if (loggingActive)
  {
    Serial.print(str);
  }
}

void loggingPrintln(String const& str)
{
  if (loggingActive)
  {
    Serial.println(str);
  }
}

void loggingPrint(Printable const& pr)
{
  if (loggingActive)
  {
    Serial.print(pr);
  }
}

void loggingPrintln(Printable const& pr)
{
  if (loggingActive)
  {
    Serial.println(pr);
  }
}

void loggingPrint(char c)
{
  if (loggingActive)
  {
    Serial.print(c);
  }
}

void loggingPrintln(char c)
{
  if (loggingActive)
  {
    Serial.println(c);
  }
}

} // namespace Logging
