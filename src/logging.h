#ifndef LOGGING_H
#define LOGGING_H

#include <Arduino.h>

namespace Logging {

void setupLogging(bool activate);
void loggingBegin();
void loggingPrint(char const* str);
void loggingPrintln(char const* str = "");
void loggingPrint(String const& str);
void loggingPrintln(String const& str);
void loggingPrint(Printable const& pr);
void loggingPrintln(Printable const& pr);
void loggingPrint(char c);
void loggingPrintln(char c);

} // namespace Logging

#endif /* LOGGING_H */
