#include "components.h"
#include <Arduino.h>

namespace Components {

Delayer::Delayer(unsigned long period, DelayerCb begin, DelayerCb end)
    : period(period), curVal(false), state(false), cbBegin(begin), cbEnd(end)
{
}

Delayer::~Delayer() {}

void Delayer::init(bool val) { curVal = val; }

void Delayer::tick(bool curVal)
{
  auto now = millis();
  if (curVal != this->curVal)
  {
    if (!state)
    {
      state = true;
      beginInstant = now;
      cbBegin();
    }
    this->curVal = curVal;
    return;
  }
  if (state && now - beginInstant > period)
  {
    state = false;
    cbEnd();
  }
}

} // namespace Components
