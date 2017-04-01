#include <Arduino.h>
#include <Homie.h>
#include <WEMOS_SHT3X.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#define DEFAULT_INTERVAL 60000 // ms
#define DEFAULT_FAHRENHEIT_FLAG true

using namespace std;

SHT3X sht30(0x45);

HomieNode tempNode("temperature", "environment");
HomieNode humidityNode("humidity", "environment");

HomieSetting<long> intervalSetting("interval",
  "Interval for publishing data in ms (min 2000)");
HomieSetting<bool> fahrenheitSetting("use_fahrenheit",
  "Use Fahrenheit instead of Celsius");

void onHomieEvent (const HomieEvent &event) {
  switch (event.type) {
    case HomieEventType::READY_TO_SLEEP:
      Homie.getLogger() << "Sleeping for " << intervalSetting.get()
        << " ms" << endl;
      ESP.deepSleep(intervalSetting.get() * 1e3, WAKE_RF_DEFAULT);
      break;
    default:
      break;
  }
}

void setup () {
  Serial.begin(115200);
  Homie_setFirmware("shtshow", "0.1.0");

  // these two handlers start "doing actual stuff" if Homie boots in "normal"
  // mode, which is why the stuff they do is not in this function.
  Homie.setSetupFunction(function<void ()>([] () {
    sht30.get();
    float tempF = sht30.fTemp;
    float humidity = sht30.humidity;
    Serial << "temp: " << tempF << ", humidity: " << humidity << endl;
    tempNode.setProperty("unit").send(fahrenheitSetting.get() ? "F" : "C");
    humidityNode.setProperty("unit").send("RH");
    tempNode.setProperty("degrees").send(String(tempF));
    humidityNode.setProperty("percentage").send(String(humidity));
    Homie.prepareToSleep();
  })).onEvent(onHomieEvent);

  intervalSetting.setDefaultValue(DEFAULT_INTERVAL).setValidator(
    function<bool (long)>([] (long value) {
      return value >= 2000;
    }));

  fahrenheitSetting.setDefaultValue(DEFAULT_FAHRENHEIT_FLAG);

  humidityNode.advertise("unit");
  humidityNode.advertise("percentage");
  tempNode.advertise("unit");
  tempNode.advertise("degrees");

  Homie.setup();
}

void loop () {
  Homie.loop();
}

#pragma clang diagnostic pop
