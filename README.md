# shtshow 

![build status](https://travis-ci.org/boneskull/shtshow.svg?branch=master)

> Temperature & humidity sensor Thing using [ESP8266] + [MQTT] + [SHT30].  Built with [PlatformIO] and [Homie]

## Hardware

I'm using a [Wemos D1 Mini] and a [SHT30 Shield].  Any ESP8266 and any SHT3x *should* work.

> Note: The SHT30 has a I2C address of `0x45`, whereas the factory setting is `0x44`.  You can change this value in the source.

## Installation

1. Wire up hardware
1. Get [PlatformIO]
1. Clone repo
1. Use PlatformIO to flash device
1. Optionally, create `data/homie/config.json` and use PlatformIO to upload to SPIFFS
1. Follow [Homie's setup instructions](https://homie-esp8266.readme.io/docs/getting-started) 

## Notes

- [Wemos' provided library](https://github.com/wemos/WEMOS_SHT3x_Arduino_Library) is hinky and **not recommended**.
- This firmware uses [ClosedCube's library](https://github.com/closedcube/ClosedCube_SHT31D_Arduino) instead.
- For the D1 Mini, I had best results with `WAKE_RF_DEFAULT` when entering deep sleep.
- Likewise, I'm not sure clock stretching works very well on an ESP8266, so we're using the SHT30's "periodic" mode.

## License

Copyright 2017 [Christopher Hiller].  Licensed MIT.

[Homie]: https://homie-esp8266.readme.io/
[SHT30]: https://www.sensirion.com/en/environmental-sensors/humidity-sensors/digital-humidity-sensors-for-various-applications/
[PlatformIO]: http://platformio.org/
[ESP8266]: http://esp8266.net/
[Wemos D1 Mini]: https://www.wemos.cc/product/d1-mini.html
[SHT30 Shield]: https://www.wemos.cc/product/sht30-shield.html
[MQTT]: http://mqtt.org
[Christopher Hiller]: https://boneskull.com
