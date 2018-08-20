# wifi_temperature_sensor
Client part of a server/client temperature logging system based on the ESP8266 NodeMCU with a DHT11 temperature sensor.

#Customization
Add your WiFi SSID and password to the code in order that the sensor can connect to your local network.
Change the ENDPOINT define with your server IP address.
Change the SLEEP_INTERVAL value to the interval you want to probe and send the temperature.
Change the sensor ID definitions to match your sensing network needs.

#Flashing
I use the Arduino IDE to flash this code on my Wemos/LoLin NodeMCU D1 mini Pro.

#Hardware
In order to turn off the DHT11 during deep sleep of the MCU, the DHT11 is powered through the D2 pin. 
The one wire communication with the DHT is performed on pin D3.
If you use a bare DHT11 (like me) you will need to add a pullup resistor (4.7K works well) between the DHT11 power and data lines.
For the deepsleep to work, you NEED to connect the D0 pin to the RST pin, otherwise your device won't wake up when the timer is consumed.
The sensor is meant to be autonomous, you can power it with a 3.7v Li-Ion AA battery for example. Connect the battery to the 3v3 pin.

#Security
This is a simple prototype, no security is implemented between the sensor and the server. 
It means that any device connected to your local network can send fake temperature data to your server.
So you maybe should not use this sensor to monitor the temperature of you nuclear power plant ;)

#License
Creative Commons BY-NC-SA
This software is provided "as is" and its user should assume no liability of its author on any consequence resulting of its usage.
