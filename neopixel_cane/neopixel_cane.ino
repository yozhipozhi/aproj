#include <asq.h>
#include <asq_neopixel.h>


#define MAX_ANALOG 255

#define NEOPIXEL_PIN  1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(21, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

asq_make_const_array_t(uint8_t, mask, 1, {255});
const uint32_t colors[6] = {strip.Color(0xcc, 0, 0),    strip.Color(0x66, 0, 0xcc), strip.Color(0, 0, 0xcc),
                            strip.Color(0, 0xcc, 0xcc), strip.Color(0x66, 0xcc, 0), strip.Color(0xcc, 0xcc, 0)};
const asq::uint32_array_t color_sequence(6, colors);

asq::NeoPixelBounceSequencer bounce(strip, mask, color_sequence);


void dump(uint32_t c) {
  Serial.print((c >> 24) & 0xFF); Serial.print(' ');
  Serial.print((c >> 16) & 0xFF); Serial.print(' ');
  Serial.print((c >> 8) & 0xFF); Serial.print(' ');
  Serial.println(c & 0xFF);
}

void setup() {
//  Serial.begin(9600);
//  Serial.println("Hello world");
//  seq1 = new asq::AnalogOutputSequencer(
//      led_pins, 50, kCosineSequence50, /*should_loop=*/true);
//  seq1->Start();
  strip.begin();
  strip.show();
//  dump(color_sequence[0]);
}

const uint8_t delay_values[36] = {100, 100, 100, 80, 80, 80, 80, 50, 50, 50, 50, 50, 50, // 13 values
                                  30, 30, 30, 30, 30, 30, 30, 30, 30, 30, // 10 values
                                  50, 50, 50, 50, 50, 50, 80, 80, 80, 80, 100, 100, 100}; // 13 values
const asq::uint8_array_t delays(36, delay_values);

int step_count = 0;
int max_steps = 42;
int current_delay = 0;


void loop() {
  bounce.NextStep();
  delay(delays[current_delay]);
  if (++step_count >= max_steps) {
    step_count = 0;
    if (++current_delay >= delays.size()) {
      current_delay = 0;
    }
  }
}

