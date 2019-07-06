# shtshow

![build status]

> Temperature & humidity sensor Thing using [ESP8266] + [MQTT] + [SHT30]. Built with [PlatformIO] and [Homie]

## Hardware

This project _specifically_ uses:

1. [Wemos D1 Mini Pro]
1. [SHT30 Shield v1.0.0]

You probably want to be using a [dual base] to avoid heat interference from the ESP8266 itself.

> _Note_: Wemos' SHT30 v1.0.0 has a I2C address of `0x45`, whereas the factory setting is `0x44`. If you have the shield and need to use `0x45`, solder these two pads together:
>
> ![where to solder](https://cldup.com/hil-dIXkE5.png)

This firmware _should_ work with any D1 Mini. The D1 Mini Pro has 16MB of flash, which is more than you need to run this.

This firmware _may_ also work with the [SHT30 Shield v2.x], but I haven't tried that shield yet.

## Installation

1. Wire up hardware
1. Get [PlatformIO]
1. Clone this repo
1. (Optional) Create `data/homie/config.json` (see [Homie JSON config docs]) and use PlatformIO to upload the `data` dir to SPIFFS
1. Use PlatformIO to flash device
1. Follow [Homie's setup instructions]

## Notes

- [Wemos' provided library] (as of March 2017) is hinky and **not recommended**.
- This firmware uses [ClosedCube's library] instead.
- I'm not sure clock stretching works very well on an ESP8266, so we're using the SHT30's "periodic" mode.

## License

Copyright © 2017-2019 [Christopher Hiller]. Licensed [Apache-2.0].

[homie]: https://github.com/homieiot/homie-esp8266/
[sht30]: https://www.sensirion.com/en/environmental-sensors/humidity-sensors/digital-humidity-sensors-for-various-applications/
[platformio]: http://platformio.org/
[esp8266]: http://esp8266.net/
[wemos d1 mini]: https://www.wemos.cc/product/d1-mini.html
[wemos d1 mini pro]: https://wiki.wemos.cc/products:d1:d1_mini_pro
[sht30 shield v1.0.0]: https://wiki.wemos.cc/products:retired:sht30_shield_v1.0.0
[sht30 shield v2.x]: https://wiki.wemos.cc/products:d1_mini_shields:sht30_shield
[mqtt]: http://mqtt.org
[christopher hiller]: https://boneskull.com
[build status]: https://travis-ci.org/boneskull/shtshow.svg?branch=master
[dual base]: https://wiki.wemos.cc/products:d1_mini_shields:dual_base
[latest release]: https://github.com/boneskull/shtshow/releases
[esptool]: https://github.com/espressif/esptool
[homie's setup instructions]: https://homieiot.github.io/homie-esp8266/docs/2.0.0/quickstart/getting-started/
[wemos' provided library]: https://github.com/wemos/WEMOS_SHT3x_Arduino_Library
[closedcube's library]: https://github.com/closedcube/ClosedCube_SHT31D_Arduino
[homie json config docs]: https://homieiot.github.io/homie-esp8266/docs/develop/configuration/json-configuration-file/
[lgpl-3.0-only]: https://spdx.org/licenses/LGPL-3.0-only.html
