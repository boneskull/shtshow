#include <Arduino.h>
#include <Homie.h>
#include <Wire.h>
#include <ClosedCube_SHT31D.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"
#define DEFAULT_INTERVAL 60000 // ms
#define DEFAULT_FAHRENHEIT_FLAG true
#define I2C_ADDRESS 0x45

using namespace std;

ClosedCube_SHT31D sht30;

HomieNode tempNode("temperature", "environment");
HomieNode humidityNode("humidity", "environment");

HomieSetting<long> intervalSetting("interval",
  "Interval for publishing data in ms (min 2000)");
HomieSetting<bool> fahrenheitSetting("use_fahrenheit",
  "Use Fahrenheit instead of Celsius");

void onHomieEvent (const HomieEvent &event) {
  switch (event.type) {
    case HomieEventType::READY_TO_SLEEP:
      Homie.getLogger() << "Sleeping for " << intervalSetting.get() << " ms"
        << endl;
      ESP.deepSleep((uint32_t) (intervalSetting.get() * 1e3), WAKE_RF_DEFAULT);
      break;
    default:
      break;
  }
}

float celsiusToFahrenheit (float c) {
  return (c * 9 / 5) + 32;
}

void setup () {
  Serial.begin(115200);
  sht30.begin(I2C_ADDRESS);

  Homie_setFirmware("shtshow", "0.1.0");

  // these two handlers start "doing actual stuff" if Homie boots in "normal"
  // mode, which is why the stuff they do is not in this function.
  Homie.setSetupFunction(function<void ()>([] () {

    SHT31D result = sht30.readTempAndHumidity(REPEATABILITY_HIGH,
      MODE_POLLING, 50);

    float temp = result.t;
    float rh = result.rh;

    bool useFahrenheit = fahrenheitSetting.get();
    if (useFahrenheit) {
      temp = celsiusToFahrenheit(temp);
    }

    Serial << "temp: " << temp << ", humidity: " << rh << endl;

    tempNode.setProperty("unit").send(useFahrenheit ? "F" : "C");
    tempNode.setProperty("degrees").send(String(temp));

    humidityNode.setProperty("unit").send("RH");
    humidityNode.setProperty("percentage").send(String(rh));

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
