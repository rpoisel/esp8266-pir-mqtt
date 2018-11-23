#ifndef STATES_H
#define STATES_H

#include "components.h"

namespace States {

class LightOn : public Components::State {
  public:
  static LightOn* Instance()
  {
    static LightOn instance;
    return &instance;
  }
  void enter(Components::FSM* fsm);
  void execute(Components::FSM* fsm);
  void event(Components::FSM* fsm, Components::EventID event);

  private:
  LightOn() = default;
  ~LightOn() = default;
};
class LightOnOverride : public Components::State {
  public:
  static LightOnOverride* Instance()
  {
    static LightOnOverride instance;
    return &instance;
  }
  void enter(Components::FSM* fsm);
  void execute(Components::FSM* fsm) {}
  void event(Components::FSM* fsm, Components::EventID event);

  private:
  LightOnOverride() = default;
  ~LightOnOverride() = default;
};
class LightOff : public Components::State {
  public:
  static LightOff* Instance()
  {
    static LightOff instance;
    return &instance;
  }
  void enter(Components::FSM* fsm);
  void execute(Components::FSM* fsm) {}
  void event(Components::FSM* fsm, Components::EventID event);

  private:
  LightOff() = default;
  ~LightOff() = default;
};

Components::EventID EVENT_MQTT { 1 };
Components::EventID EVENT_MOTION { 2 };

} // namespace States

#endif /* STATES_H */
