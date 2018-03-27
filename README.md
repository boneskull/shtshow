# shtshow

![build status]

> Temperature & humidity sensor Thing using [ESP8266] + [MQTT] + [SHT30].  Built with [PlatformIO] and [Homie]

## Hardware

I'm using a [Wemos D1 Mini] and a [SHT30 Shield].  Any ESP8266 and any SHT3x *should* work.

You probably want to be using a [dual base] to avoid heat interference from the ESP8266 itself. 

> *Note*: Wemos' SHT30 has a I2C address of `0x45`, whereas the factory setting is `0x44`.  If you have the shield and need to use `0x45`, solder these two pads together:
>
> ![where to solder](https://cldup.com/hil-dIXkE5.png)

## Installation (From Binary)

1. Wire up hardware
1. Download the appropriate firmware for your D1 Mini board from the [latest release]
1. Open the archive
1. Use [esptool] to flash your board with the firmware
1. Follow [Homie's setup instructions]

## Installation (From Source)

1. Wire up hardware
1. Get [PlatformIO]
1. Clone repo
1. Use PlatformIO to flash device
1. Optionally, create `data/homie/config.json` and use PlatformIO to upload to SPIFFS
1. Follow [Homie's setup instructions]

## Notes

- [Wemos' provided library] is hinky and **not recommended**.
- This firmware uses [ClosedCube's library] instead.
- For the D1 Mini, I had best results with `WAKE_RF_DEFAULT` when entering deep sleep. YMMV.
- Likewise, I'm not sure clock stretching works very well on an ESP8266, so we're using the SHT30's "periodic" mode.

## License

Copyright © 2017-2018 [Christopher Hiller].  Licensed MIT.

[Homie]: https://homie-esp8266.readme.io/
[SHT30]: https://www.sensirion.com/en/environmental-sensors/humidity-sensors/digital-humidity-sensors-for-various-applications/
[PlatformIO]: http://platformio.org/
[ESP8266]: http://esp8266.net/
[Wemos D1 Mini]: https://www.wemos.cc/product/d1-mini.html
[SHT30 Shield]: https://www.wemos.cc/product/sht30-shield.html
[MQTT]: http://mqtt.org
[Christopher Hiller]: https://boneskull.com
[build status]: https://travis-ci.org/boneskull/shtshow.svg?branch=master
[dual base]: https://www.wemos.cc/product/dual-base.html
[latest release]: https://github.com/boneskull/shtshow/releases
[esptool]: https://github.com/espressif/esptool
[Homie's setup instructions]: https://marvinroger.github.io/homie-esp8266/docs/develop/quickstart/getting-started/
[Wemos' provided library]: https://github.com/wemos/WEMOS_SHT3x_Arduino_Library
[ClosedCube's library]: https://github.com/closedcube/ClosedCube_SHT31D_Arduino
