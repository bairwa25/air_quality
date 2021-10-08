# Air Pollution Monitoring System

This is **IOT based Air Pollution Monitoring System** which measures indoor harmful gases present in air like Carbon Dioxide(CO2) using an MQ135 gas sensor and Carbon Monoxide(CO) using an MQ7 sensor. It will show the air quality in PPM(Parts Per Million) on the LCD and as well as on [Thingspeak](https://thingspeak.com/channels/1347787) so that we can monitor it very easily in PPM.

## Components Requirement:
- Arduino Uno
- Wi-Fi module Node-MCU ESP8266
- 16x2 LCD
- MQ135 Gas sensor
- MQ 7 LPG gas sensor
- Buzzer
- LEDs

## System Architecture:
![architecture](assets/images/Design.png)

## Software Requirement:
- [Arduino IDE](https://www.arduino.cc/)
> Arduino IDE used to upload programming in Arduino board and Node-MCU board with required library.
- [Thingspeak](https://thingspeak.com/)
> ThingSpeak is an IoT analytics platform service that allows you to aggregate, visualize, and analyze live data streams in the cloud. You can send data to ThingSpeak from your devices, create instant visualization of live data, and send alerts.

## Measurement:
The most important step is to calibrate the sensor in fresh air and then draw an equation that converts the sensor output voltage value into our convenient units PPM (parts per
million). Here are the mathematical calculations derived:

<img src="https://user-images.githubusercontent.com/71394525/136597549-2f8f6810-ab9a-4042-a847-99b989679a26.png" width="300" height="50" />

For a log-log scale, the formula looks like this:

<img src="https://user-images.githubusercontent.com/71394525/136597505-823d507c-8215-4c14-8c83-c105f5cc6475.png" width="300" height="50" />

Let’s find the slope. The formula to calculate slope m(here) is the following:

<img src="https://user-images.githubusercontent.com/71394525/136597462-bef17157-1b8e-4893-b68d-e5552b6ea2e6.png" width="300" height="50" />

If we apply the logarithmic quotient rule we get the following:

<img src="https://user-images.githubusercontent.com/71394525/136597007-7457d561-9678-4e5d-ac90-1bd1ea19e921.png" width="300" height="50" />

Now we substitute the values for x, x0, y, and y0.
Now that we have m and b, we can find the gas concentration for any ratio with the following formula:

<img src="https://user-images.githubusercontent.com/71394525/136596791-877a8b37-5394-4dcb-a82a-3065ac7dbea3.png" width="300" height="50" />

Using this we will be able to convert the sensor output values into PPM (Parts per Million)

However, in order to get the real value of the gas concentration according to the log-log plot we need to find the inverse log of x:

<img src="https://user-images.githubusercontent.com/71394525/136596657-6a3f4ec0-f717-49f3-b5a0-3d45a17cadc2.png" width="300" height="50" />

> More details for calculation [here](assets/docs/Final_Paper.pdf)

> Datasheets for:
  - [MQ135](https://www.olimex.com/Products/Components/Sensors/Gas/SNS-MQ135/resources/SNS-MQ135.pdf)
  - [MQ7](https://www.sparkfun.com/datasheets/Sensors/Biometric/MQ-7.pdf)
 
## Results:
<p align="center">
  <img src="https://user-images.githubusercontent.com/71394525/136599171-dcd25636-8602-413f-8a2d-8820683af360.png" width="400" height="400" /> 
  <img src="https://user-images.githubusercontent.com/71394525/136599503-6ad07c23-1344-41fd-b735-1ea4a546efba.png" width="400" height="400" />
</p>

<p align="center">
  <img align="center" src="https://user-images.githubusercontent.com/71394525/136599263-e84b990a-dead-48c4-b1c8-b88777c3dafe.png" width="700" height="400" />
</p>

> **More details found in Document [here](assets/docs/Air_Pollution.pdf)**

## Credit:
- [AJAY KUMAR BAIRWA](https://www.linkedin.com/in/ajay-bairwa)
- [SURENDRA KUMAR](https://www.instagram.com/suruu.meena/)
- [ROHIT KUMAR](https://www.instagram.com/rohit_meena__23/)
