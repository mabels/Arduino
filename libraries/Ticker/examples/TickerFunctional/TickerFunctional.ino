#include "Arduino.h"
#include "Ticker.h"

#define LED1  2
#define LED2  4
#define LED3  12
#define LED4  14
#define LED5  15


class ExampleClass {
  public:
    ExampleClass(int pin, int duration) : _pin(pin), _duration(duration) {
      pinMode(_pin, OUTPUT);
      _myTicker.attach_ms(_duration, std::bind(&ExampleClass::classBlink, this));
    }
    ~ExampleClass() {};

    int _pin, _duration;
    Ticker _myTicker;

    void classBlink() {
      digitalWrite(_pin, !digitalRead(_pin));
    }
};

void staticBlink() {
  digitalWrite(LED2, !digitalRead(LED2));
}

void scheduledBlink() {
  digitalWrite(LED3, !digitalRead(LED2));
}

void parameterBlink(int p) {
  digitalWrite(p, !digitalRead(p));
}

Ticker staticTicker;
Ticker scheduledTicker;
Ticker parameterTicker;
Ticker lambdaTicker;

ExampleClass example(LED1, 100);

void attach_ms() {
  staticTicker.attach_ms(100, staticBlink);

  scheduledTicker.attach_ms_scheduled(100, scheduledBlink);

  parameterTicker.attach_ms(100, std::bind(parameterBlink, LED4));

  lambdaTicker.attach_ms(100, []() {
    digitalWrite(LED5, !digitalRead(LED5));
  });
}

void attach_us() {
  staticTicker.attach_us(900, staticBlink);

  scheduledTicker.attach_us_scheduled(900, scheduledBlink);

  parameterTicker.attach_us(900, std::bind(parameterBlink, LED4));

  lambdaTicker.attach_us(900, []() {
    digitalWrite(LED5, !digitalRead(LED5));
  });
}

void detach() {
  staticTicker.detach();

  scheduledTicker.detach();

  parameterTicker.detach();

  lambdaTicker.detach();
}


void setup() {
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
}

static int ms2us = 0;
void loop() {
	if (++ms2us % 2 == 0) {
		attach_ms();
	} else {
		attach_us();
	}
	delay(2);
	detach();
}
