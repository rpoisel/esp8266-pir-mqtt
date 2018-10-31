#include <components.h>
#include <mock.h>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <climits>
#include <vector>

using namespace Components;

class TimingTestsFixture {
  protected:
  static size_t numMillisInvocations;
  static size_t numBeginInvocations;
  static size_t numEndInvocations;
  static unsigned long currentTime;
  TimingTestsFixture() { millisImpl = &myMillisImpl; }

  private:
  static unsigned long myMillisImpl(void)
  {
    numMillisInvocations++;
    return currentTime;
  }
};

size_t TimingTestsFixture::numMillisInvocations = 0;
size_t TimingTestsFixture::numBeginInvocations = 0;
size_t TimingTestsFixture::numEndInvocations = 0;
unsigned long TimingTestsFixture::currentTime = 0;

TEST_CASE_METHOD(TimingTestsFixture, "Default behavior of delayer functionality", "[single-file]")
{
  unsigned long const samplePeriod = 5000;
  Delayer d(samplePeriod, [&]() { TimingTestsFixture::numBeginInvocations++; },
            [&]() { TimingTestsFixture::numEndInvocations++; });
  d.init(false);
  REQUIRE(0 == numMillisInvocations);
  REQUIRE(0 == numBeginInvocations);

  currentTime = 1;
  d.tick(true);
  REQUIRE(1 == numMillisInvocations);
  REQUIRE(1 == numBeginInvocations);
  REQUIRE(0 == numEndInvocations);

  currentTime = 2;
  d.tick(false);
  REQUIRE(2 == numMillisInvocations);
  REQUIRE(1 == numBeginInvocations);
  REQUIRE(0 == numEndInvocations);

  currentTime = samplePeriod + 2;
  d.tick(false);
  REQUIRE(3 == numMillisInvocations);
  REQUIRE(1 == numBeginInvocations);
  REQUIRE(1 == numEndInvocations);

  currentTime = ULONG_MAX - samplePeriod  + 1;
  d.tick(true);
  REQUIRE(4 == numMillisInvocations);
  REQUIRE(2 == numBeginInvocations);
  REQUIRE(1 == numEndInvocations);

  currentTime = 0;
  d.tick(true);
  REQUIRE(5 == numMillisInvocations);
  REQUIRE(2 == numBeginInvocations);
  REQUIRE(1 == numEndInvocations);

  currentTime = 1;
  d.tick(true);
  REQUIRE(6 == numMillisInvocations);
  REQUIRE(2 == numBeginInvocations);
  REQUIRE(2 == numEndInvocations);
}
