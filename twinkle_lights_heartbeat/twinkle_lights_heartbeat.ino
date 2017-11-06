#include <asq.h>
#include <Adafruit_Sensor.h>

asq_make_const_array_t(uint8_t, LED_PINS1, 3, {9, 10, 11});

asq_make_const_array_t(asq::AnalogOutputSequencer::Action, actions, 20, {
  {1, {0}},
  {1, {61}},
  {1, {122}},
  {1, {183}},
  {1, {255}},
  {1, {183}},
  {1, {122}},
  {1, {61}},
  {1, {0}},
  {1, {61}},
  {1, {122}},
  {1, {183}},
  {1, {255}},
  {1, {204}},
  {1, {153}},
  {1, {102}},
  {1, {76}},
  {1, {51}},
  {1, {25}},
  {10, {0}},
});
asq::AnalogOutputSequencer seq1(LED_PINS1, actions);


void setup() {}


void loop() {
  seq1.NextStep();
  delay(70);
}

