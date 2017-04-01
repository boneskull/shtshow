'use strict';

const mqtt = require('mqtt');
const fs = require('fs');

const DEVICE = 'my-device';
const BROKER = 'host:port';
const FIRMWARE_PATH = './.pioenvs/d1_mini/firmware.bin';
const client = mqtt.connect(`mqtt://${BROKER}`);

client.on('connect', () => {
  client.subscribe(`devices/${device}/$implementation/ota/status`);
  client.on('message', (topic, msg) => {
    if (topic === `devices/${device}/$implementation/ota/status`) {
      msg = String(msg);
      if (msg === '202') {
        console.log('checksum ok, publishing');
        client.subscribe(`devices/${device}/$homie`);
        client.publish(`devices/${device}/$implementation/ota/firmware`, fs.readFileSync('./.pioenvs/d1_mini/firmware.bin'));
      } else if (/^206/.test(msg)) {
        console.log(`progress: ${msg}`);
      } else if (msg === '200') {
        console.log('success!');
        process.exit(0);
      } else {
        throw new Error(msg);
      }
    } else if (/\$homie$/.test(msg)) {
      console.log('success!');
      process.exit(0);
    }
  });
  client.publish(`devices/${device}/$implementation/ota/checksum`, '415bad882596a05b1c99952e5ead5512');
});
