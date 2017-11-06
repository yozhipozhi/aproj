#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <asq_action.h>

#define CALIBRATION_LED  10
asq_make_const_array_t(uint8_t, LED_PINS1, 3, {9, 10, 12});

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

#define USE_ACCEL
#ifdef USE_ACCEL
class MovingAverage {
 public:
  explicit MovingAverage(size_t window_size) : size_(window_size), pos_(0), sum_(0) {
      values_ = new float[window_size];
      memset(values_, 0, sizeof(float) * window_size);
  }

  ~MovingAverage() { delete(values_); }

  float Average() const { return sum_ / size_; }

  void AddValue(const float value) {
    sum_ -= values_[pos_];
    sum_ += value;
    values_[pos_] = value;
    pos_ = (pos_ + 1) % size_;
  }

  private:
    const size_t size_;
    size_t pos_;
    float sum_;
    float* values_;
};


Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(8675309);

MovingAverage avg_accel(40);
float x_base = 0, y_base = 0, z_base = 0;

void setup() {
//#ifndef ESP8266
//  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
//#endif  Serial.begin(9600);
//  Serial.println("Hello world");
   if(!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    while(1);
  }

  const int kCalibrationDelay = 50;
  const int kCalibrationSteps = 2 * 1000 / kCalibrationDelay;
  MovingAverage avg_x(kCalibrationSteps), avg_y(kCalibrationSteps), avg_z(kCalibrationSteps);
  for (int i = 0; i < kCalibrationSteps; ++i) {
    sensors_event_t event;
    accel.getEvent(&event);
    avg_x.AddValue(event.acceleration.x);
    avg_y.AddValue(event.acceleration.y);
    avg_z.AddValue(event.acceleration.z);
    delay(kCalibrationDelay);
    analogWrite(CALIBRATION_LED, i % 2 ? 0 : 255);
  }
  analogWrite(CALIBRATION_LED, 0);
  x_base = avg_x.Average();
  y_base = avg_y.Average();
  z_base = avg_z.Average();
}
#else
void setup() {}
#endif


void loop() {
  seq1.NextStep();

#ifdef USE_ACCEL
  sensors_event_t event;
  accel.getEvent(&event);

  const float dx = event.acceleration.x - x_base;
  const float dy = event.acceleration.y - y_base;
  const float dz = event.acceleration.z - z_base;
  const float accel = sqrt(dx * dx + dy * dy + dz * dz);
  avg_accel.AddValue(accel);
  const float avg = avg_accel.Average();
//  Serial.print("("); Serial.print(event.acceleration.x); Serial.print(", ");
//  Serial.print(event.acceleration.y); Serial.print(", ");
//  Serial.print(event.acceleration.z); Serial.print(")\t");
//  Serial.print("("); Serial.print(dx); Serial.print(", ");
//  Serial.print(dy); Serial.print(", ");
//  Serial.print(dz); Serial.print(")\t");
//  Serial.print(accel); Serial.print("\t"); Serial.println(avg);

  x_base += (event.acceleration.x - x_base) / 200;
  y_base += (event.acceleration.y - y_base) / 200;
  z_base += (event.acceleration.z - z_base) / 200;

  const int delay_ms = max(30, 70 - 4 * avg);
//  Serial.println(delay_ms);
  delay(delay_ms);
#else
  delay(70);
#endif
}

