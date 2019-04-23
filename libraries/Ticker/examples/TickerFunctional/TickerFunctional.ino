#include "Arduino.h"
#include "Ticker.h"

#define LED1  2
#define LED2  2
#define LED3  2
#define LED4  2
#define LED5  2

#define TICKFREQ_MS 250
#define TICKFREQ_US (TICKFREQ_MS * 1000)


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

// ExampleClass example(LED1, TICKFREQ_MS);

int lambdaTickerCount = 1;
long lambdaTickerTotal = 0;
long lambdaTickerLast = 0;
void attach_ms() {
/*
  staticTicker.attach_ms(TICKFREQ_MS, staticBlink);

  scheduledTicker.attach_ms_scheduled(TICKFREQ_MS, scheduledBlink);

  parameterTicker.attach_ms(TICKFREQ_MS, std::bind(parameterBlink, LED4));
*/

  lambdaTickerLast = micros();
  lambdaTicker.attach_ms(TICKFREQ_MS, []() {
    ++lambdaTickerCount;
    auto now = micros();
    lambdaTickerTotal += now - lambdaTickerLast; 
    digitalWrite(LED5, !digitalRead(LED5));
    lambdaTickerLast = now;
  });
}


int us2ms;
void attach_us() {
/*
  staticTicker.attach_us(TICKFREQ_US/500, staticBlink);

  scheduledTicker.attach_us_scheduled(TICKFREQ_US/500, scheduledBlink);

  parameterTicker.attach_us(TICKFREQ_US/500, std::bind(parameterBlink, LED4));
*/
  lambdaTickerLast = micros();
  us2ms = 0;
  lambdaTicker.attach_us(TICKFREQ_US/500, []() {
    us2ms += TICKFREQ_US/500;
    if (us2ms >= TICKFREQ_US) {
	    us2ms = 0;
	    ++lambdaTickerCount;
	    auto now = micros();
	    lambdaTickerTotal += now - lambdaTickerLast; 
	    digitalWrite(LED5, !digitalRead(LED5));
	    lambdaTickerLast = now;
    }
  });
}

void detach() {
/*
  staticTicker.detach();

  scheduledTicker.detach();

  parameterTicker.detach();
*/
  lambdaTicker.detach();
}


void setup() {
  Serial.begin(115200);
  Serial.println("Start");
/*
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
*/
  pinMode(LED5, OUTPUT);
}

int ms2us = 0;
void loop() {
	if (++ms2us % 2 == 0) {
		attach_ms();
	} else {
		attach_us();
	}
	delay(2000);
	detach();
        auto avg = lambdaTickerTotal/lambdaTickerCount;
	// if (!(245000 <= avg && avg <= 255000)) {
		Serial.print(lambdaTickerCount);
		Serial.print(":");
		Serial.println(avg);
	// }
	lambdaTickerCount = 1;
	lambdaTickerTotal = 0;
}
