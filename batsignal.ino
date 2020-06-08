/*
  Batsignal

  Turns on an LED lamp connected via a relay, based on an Adafruit IO
  feed.

  Author: Kushagra Sinha <kush@j4nu5.com>
  https://github.com/j4nu5/batsignal

*/

#include "config.h"


// The Adafruit IO feed we want to connect to.
AdafruitIO_Feed * const kInputFeed = io.feed(kInputFeedName);

// Sets up debugging via serial port.
void SetupDebugging();

// Emits a log line (including newline) to the serial port.
// Does not do anything for release builds.
void Logln(const char * const);

// Emits a log (not including newline) to the serial port.
// Does not do anything for release builds.
void Log(const char * const);

// Handles incoming messages from |kInputFeed|.
void HandleMessage(AdafruitIO_Data *);


void setup() {
#if defined(DEBUG_BUILD)
  SetupDebugging();
#endif  // DEBUG_BUILD

  Log("Connecting to Adafruit IO");
  // Start MQTT connection to io.adafruit.com.
  io.connect();

  // Set up a message handler for the input feed.
  kInputFeed->onMessage(HandleMessage);

  // Wait for an MQTT connection
  while (io.mqttStatus() < AIO_CONNECTED) {
    Log(".");
    delay(500);
  }
  Logln("");
  Logln("Connected");

  // Because Adafruit IO doesn't support the MQTT retain flag,
  // we can use the get() function to ask IO to resend the
  // last value for this feed to just this MQTT client after
  // the io client is connected.
  kInputFeed->get();
}

void loop() {
  // It should always be present at the top of your loop
  // function. It keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
}

void SetupDebugging() {
  // Start the serial connection with a baud rate of 9600.
  Serial.begin(9600);

  // Wait for serial monitor to open.
  while (!Serial);
}

void Logln(const char * const line) {
#if defined(DEBUG_BUILD)
  Serial.println(line);
#endif  // DEBUG_BUILD
}

void Log(const char * const line) {
#if defined(DEBUG_BUILD)
  Serial.print(line);
#endif  // DEBUG_BUILD
}

void HandleMessage(AdafruitIO_Data *data) {
  const char * const message = data->value();
  if (message == nullptr) {
    return;
  }

  Log("Received <- ");
  Logln(message);
}
