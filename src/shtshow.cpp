#include <Arduino.h>
#include <Homie.h>
#include <Wire.h>
#include <ClosedCube_SHT31D.h>
#include <SimpleTimer.h>
#include <ESP8266WiFi.h>

extern "C" {
#include "user_interface.h"  // Required for wifi_station_connect() to work
}

#define VERSION "1.0.0"
#define DEFAULT_INTERVAL 60000  // ms
#define DEFAULT_FAHRENHEIT_FLAG true
#define I2C_ADDRESS 0x45
#define MQTT_CONNECTION_TIMEOUT 30000
#define SLEEP_TIMEOUT 6000
#define FPM_SLEEP_MAX_TIME 0xFFFFFFF
using namespace std;

SimpleTimer homieLoopTimer;
SimpleTimer mainLoopTimer;

int publishTimerId;
int sleepTimeoutId;
int connectionTimeoutId;
ClosedCube_SHT31D sht30;

HomieNode tempNode("temperature", "environment");
HomieNode humidityNode("humidity", "environment");

HomieSetting<long> intervalSetting(
    "interval", "Interval for publishing data in ms (min 2000)");
HomieSetting<bool> fahrenheitSetting("use_fahrenheit",
                                     "Use Fahrenheit instead of Celsius");

void WiFiOn();
void WiFiOff();
float celsiusToFahrenheit(float);
void gotoDeepSleep();
void loopHandler();
void onHomieEvent(const HomieEvent &);
void prepareToSleep();
void publishStates();

void WiFiOn() {
  wifi_fpm_do_wakeup();
  wifi_fpm_close();
  wifi_set_opmode(STATION_MODE);
  wifi_station_connect();
}

void WiFiOff() {
  wifi_station_disconnect();
  wifi_set_opmode(NULL_MODE);
  wifi_set_sleep_type(MODEM_SLEEP_T);
  wifi_fpm_open();
  wifi_fpm_do_sleep(FPM_SLEEP_MAX_TIME);
}

void gotoDeepSleep() {
  Homie.getLogger() << "Sleeping for " << intervalSetting.get() << " ms"
                    << endl;
  Homie.doDeepSleep(intervalSetting.get() * 1000UL, WAKE_RF_DEFAULT);
  ESP.deepSleep(intervalSetting.get() * 1000UL, WAKE_RF_DEFAULT);
}

void onHomieEvent(const HomieEvent &event) {
  switch (event.type) {
    case HomieEventType::MQTT_READY:
      Homie.getLogger() << "MQTT ready!" << endl;
      mainLoopTimer.disable(connectionTimeoutId);
      break;
    case HomieEventType::MQTT_DISCONNECTED:
      mainLoopTimer.restartTimer(connectionTimeoutId);
      mainLoopTimer.enable(connectionTimeoutId);
      break;
    case HomieEventType::READY_TO_SLEEP:
      Homie.getLogger() << "Ready to sleep" << endl;
      mainLoopTimer.setTimeout(0, gotoDeepSleep);
      break;
    default:
      break;
  }
}

void prepareSleep() { Homie.prepareToSleep(); }

void checkGotoSleep() {
  if (Homie.getMqttClient().connected()) {
    Homie.getLogger() << "Sending signal to sleep" << endl;
    homieLoopTimer.deleteTimer(publishTimerId);
    sleepTimeoutId = mainLoopTimer.setTimeout(SLEEP_TIMEOUT, gotoDeepSleep);
    prepareSleep();
  }
}

float celsiusToFahrenheit(float c) { return (c * 9 / 5) + 32; }

void handleMQTTConnectionTimeout() {
  Homie.getLogger() << "MQTT connection timed out!" << endl;

  prepareSleep();
}

void loopHandler() { homieLoopTimer.run(); }

void publishStates() {
  SHT31D result = sht30.readTempAndHumidity(SHT3XD_REPEATABILITY_HIGH,
                                            SHT3XD_MODE_POLLING, 50);

  if (result.error == SHT3XD_NO_ERROR) {
    float temp = result.t;
    float rh = result.rh;

    bool useFahrenheit = fahrenheitSetting.get();
    if (useFahrenheit) {
      temp = celsiusToFahrenheit(temp);
    }

    Homie.getLogger() << "temp: " << temp << ", humidity: " << rh << endl;

    tempNode.setProperty("unit").send(useFahrenheit ? "F" : "C");
    tempNode.setProperty("value").send(String(temp));

    humidityNode.setProperty("unit").send("RH");
    humidityNode.setProperty("value").send(String(rh));
  } else {
    Homie.getLogger() << "SHT3XD error: " << result.error << endl;
  }

  checkGotoSleep();
}

void setup() {
  WiFiOff();
  delay(5000);
  WiFiOn();
  Serial.begin(115200);
  Wire.begin();
  sht30.begin(I2C_ADDRESS);

  Homie_setFirmware("shtshow", VERSION);

  // these two handlers start "doing actual stuff" if Homie boots in "normal"
  // mode, which is why the stuff they do is not in this function.
  Homie.onEvent(onHomieEvent).setLoopFunction(loopHandler);

  intervalSetting.setDefaultValue(DEFAULT_INTERVAL)
      .setValidator(
          function<bool(long)>([](long value) { return value >= 2000; }));
  fahrenheitSetting.setDefaultValue(DEFAULT_FAHRENHEIT_FLAG);

  humidityNode.advertise("unit");
  humidityNode.advertise("value");
  tempNode.advertise("unit");
  tempNode.advertise("value");
  Homie.setup();

  publishTimerId = homieLoopTimer.setInterval(5000, publishStates);
  connectionTimeoutId = mainLoopTimer.setInterval(MQTT_CONNECTION_TIMEOUT,
                                                  handleMQTTConnectionTimeout);
}

void loop() {
  if (homieLoopTimer.getNumTimers()) {
    Homie.loop();
  }
  mainLoopTimer.run();
}
