
#include <arduinoFFT.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
int GREENPIN = 5;
int REDPIN = 6;
int BLUEPIN = 11;
int MICROFON = 14;

const uint16_t samples = 64;
//const uint16_t num_pixels = 60;
const uint8_t loop_num = 1;
const uint16_t sound_thresh = 100;


double vReal[samples];
double vImag[samples];


arduinoFFT FFT = arduinoFFT();
void setup() {
  // put your setup code here, to run once:

  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
  pinMode(MICROFON,INPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(REDPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  Serial.begin(9600);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  int freq_avg = 0;
  int max_avg = 0;
  for (int loop_count = 0; loop_count < loop_num; loop_count++) {
    double average = 0;
    for (int i = 0; i < samples; i++) {
      while (!(ADCSRA & 0x10)); // wait on ADIF bit
      ADCSRA = 0b11110101; // clear ADIF bit
     vReal[i] = (double)ADC; average += ADC;
      vImag[i] = 0;
    }
    average = average / samples;
    for (int i = 0; i < samples; i++) {
      vReal[i] = vReal[i] - average;
    }

    FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, samples, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, samples);
    double max_val = 0;
    uint16_t frequency = 0;
    for (int i = 0; i < samples/2; i++) {
      if (vReal[i] > max_val) {
        max_val = vReal[i];
        frequency =(2000 / (samples/2)) * i;
      }
    }

    max_avg += max_val;
    freq_avg += frequency;
  }
    max_avg /= loop_num;
  freq_avg /= loop_num;
  const int range_max = 2000;
//  if (freq_avg > range_max) freq_avg = range_max;
    int x = map(freq_avg,0,range_max,0,255);
    
     int red = -abs(2*(x-255))+255;
    int green = -abs(2*x)+255;
    int blue = -abs(2*(x-128))+255;
////     if (red < 0) red = 0;
////    if (green < 0) green = 0;
////    if (blue < 0) blue = 0;

     analogWrite(REDPIN, red);
     analogWrite(GREENPIN,green);
     analogWrite(BLUEPIN,blue);
//    Serial.print(blue);
//    Serial.print(",");
//    Serial.print(green);
//    Serial.print(",");
//    Serial.print(red);
//    Serial.print(",");
    Serial.println(freq_avg);
}
