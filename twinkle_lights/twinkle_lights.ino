#include <Adafruit_Sensor.h>
#include <asq_action.h>
#include <asq_action_data.h>

asq_make_const_array_t(uint8_t, LED_PINS1, 1, {9});
asq_make_const_array_t(uint8_t, LED_PINS2, 1, {10});
asq_make_const_array_t(uint8_t, LED_PINS3, 1, {11});

using Action = asq::ActionSequencer<1>::Action;
const asq::array_t<Action> TWINKLE(50, asq::kActionsCosineUnsigned50);

asq::AnalogOutputSequencer seq1(LED_PINS1, TWINKLE, 0);
asq::AnalogOutputSequencer seq2(LED_PINS2, TWINKLE, 16);
asq::AnalogOutputSequencer seq3(LED_PINS3, TWINKLE, 32);

void setup() {}

void loop() {
  seq1.NextStep();
  seq2.NextStep();
  seq3.NextStep();
  delay(50);
}

