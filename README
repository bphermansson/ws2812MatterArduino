# ws2812MatterArduino

The point of this project is to build a RGB-light that can controlled by Google Home and other Home control systems. It is universal in that it uses Matter, a common standard for smart devices.
This project uses a ESP32, c3, s3 or some sort. It has a led WS2812 connected, either a single RGB-led or a strip with many leds. Dont forget to add a resistor, 330 ohm, to the data line.
The code compiles with VSCode and PlatformIO or PioArduino.

Before you start, create a new file "private_settings.ini" in the root directory. Let it contain:

```
[env]
build_flags =
    -DWIFI_SSID=\"<YOUR WIFI SSID>\"
    -DWIFI_PASSWORD=\"YOUR WIFI PASSWORD>\"
    -DLED_PIN=<THE PIN CONNECTED TO THE WS2812>
    -DNUM_LEDS=<NUMBER OF LEDS IN STRIP>
```

Then build and upload the code. Use the serial monitor to see onboarding info to add the device to Google Home.
