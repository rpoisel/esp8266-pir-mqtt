#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Arduino.h>

#include <cstdint>
#include <functional>

namespace Components {

typedef uint8_t const EventID;
EventID EVENT_INVALID { 0 };

class FSM;

class State
{
  public:
    State() = default;
    virtual ~State() = default;

    virtual void enter(FSM* fsm) {}
    virtual void execute(FSM* fsm) = 0;
    virtual void exit(FSM* fsm) {}
    virtual void event(FSM* fsm, EventID event) {}
};

class FSM
{
  public:
    FSM() = default;
    FSM(State* curState) :curState(curState) {}
    virtual ~FSM() = default;

    void update()
    {
      if (!curState)
      {
        return;
      }
      curState->execute(this);
    }
    void setState(State* newState)
    {
      if (!newState)
      {
        return;
      }

      if (curState)
      {
        curState->exit(this);
      }
      curState = newState;
      curState->enter(this);
    }
    void event(EventID event)
    {
      if (!curState)
      {
        return;
      }
      curState->event(this, event);
    }
  private:
    State* curState;
};

class Timer
{
  public:
    Timer(unsigned long period) : period(period), startTime(0) { }
    virtual ~Timer() = default;

    void start()
    {
      startTime = millis();
    }
    bool isExpired() const
    {
      return millis() - startTime >= period;
    }
  private:
    unsigned long const period;
    unsigned long startTime;
};

class Light {
  public:
  Light(uint8_t pin) : pin(pin) { }
  ~Light() = default;

  void begin()
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }
  void setState(bool newState)
  {
    digitalWrite(pin, newState ? HIGH : LOW);
  }
  bool getState() const
  {
    return digitalRead(pin);
  }

  inline void on()
  {
    setState(true);
  }
  inline void off()
  {
    setState(false);
  }
  inline void flip()
  {
    setState(!getState());
  }

  private:
  uint8_t pin;
};


class MotionSensor
{
  typedef std::function<void(void)> Callback;
  public:
    MotionSensor(uint8_t pin, Callback cb) : pin(pin), cb(cb) { }
    virtual ~MotionSensor() = default;

    void begin()
    {
      pinMode(pin, INPUT);
      lastVal = digitalRead(pin);
    }
    void loop()
    {
      auto curVal = digitalRead(pin);
      if (curVal != lastVal)
      {
        cb();
        lastVal = curVal;
      }
    }
  private:
    uint8_t pin;
    Callback cb;
    int lastVal;
};

} // namespace Components

#endif /* COMPONENTS_H */
