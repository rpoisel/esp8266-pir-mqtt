#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <cstdint>
#include <functional>

namespace Components {

typedef std::function<void(void)> DelayerCb;

class Delayer {
  public:
  Delayer(unsigned long period, DelayerCb begin, DelayerCb end);
  virtual ~Delayer();

  void init(bool val);
  void tick(bool curVal);

  private:
  unsigned long const period;
  bool curVal;
  bool state;
  unsigned long beginInstant;
  DelayerCb cbBegin;
  DelayerCb cbEnd;
};

} // namespace Components

#endif /* COMPONENTS_H */
