# GoogleAssistant-Controlled-SmartGarage
___
## Introduction 

Making  <b>Smart Garage system </b> IoT device using<b> NodeMCU </b>which can be opened or closed using Google Assistant only when you are near your Garage.

![google-assistant](https://user-images.githubusercontent.com/34485667/35230959-7d44f826-ffbd-11e7-866b-cdd3b011e92e.png)

<b>IoT</b> will use<b> MQTT </b>protocol (Similar to Client Server communication protocol ) for communicating with server.
It is a light message transfer protocol used for communication among various IoT devices we see is the Market.

Those who don't know much about Mqtt Protocol can refer the PDF uploaded above to get basic insight of ,how the Protocol
works.

We will use Adafruit io as our Broker in Mqtt system ,which provides free Mqtt cloud based Broker Service and NodeMCU based
IoT device as client.

Further we will build Applets using IFTTT to Make Google Assistant to control our IoT device.

___

## Configuring Arduino IDE for ESP8266 based Dev Boards

Initially Arduino IDE dosen't have Addon by default ,so we have to install the Addons for Esp8266 based Boards using Arduino Board Manager.To begin, we’ll need to update the board manager with a custom URL. Open up Arduino, then go to the Preferences 
<b>(File > Preferences)</b>. Then, towards the bottom of the window, copy this URL into the “Additional Board Manager URLs” text box:

<pre> http://arduino.esp8266.com/stable/package_esp8266com_index.json </pre>


![arduino-board-manager-link](https://user-images.githubusercontent.com/34485667/34893687-62adaa8c-f804-11e7-9b83-b9fb8b594853.png)


Hit OK. Then navigate to the Board Manager by going to <b>Tools > Boards > Boards Manager</b>.There should be a couple new entries in addition to the standard Arduino boards. Look for <b>esp8266</b>. Click on that entry, then select <b>Install</b>.


![arduino-board-install](https://user-images.githubusercontent.com/34485667/34894102-0d901fc4-f806-11e7-8a04-c6e0be9259ae.png)

The board definitions and tools for the ESP8266 Thing include a whole new set of gcc, g++, and other reasonably large, compiled binaries, so it may take a few minutes to download and install (the archived file is ~110MB). Once the installation has completed, an Arduino-blue “INSTALLED” will appear next to the entry.

Now go on <b> Tools > Boards </b> and Scroll down and select <b> NodeMCU 1.0 (ESP 12-E Module) </b> .  


___

## Broker Adafruit io

First of all you need to make Account on  [Adafruit io](https://io.adafruit.com/) .

![screenshot 7](https://user-images.githubusercontent.com/34485667/34895622-9cccf0e4-f80c-11e7-8ae6-467993e205cf.png)

login to your Adafruit io Account and go on<b> dashboard</b> and Click on create new option and from various type select toggle & Gauge feed and the enter feed name


![screenshot 12](https://user-images.githubusercontent.com/34485667/35231543-1e43546a-ffbf-11e7-8a35-232d6a5d2e9c.png)


now your Broker is established copy username and key and put them into the code<b> garagedata.ino</b> 

___

## Burning Code On Node MCU

First take above two libraries file provided by vendors itself which are Arduino & Adafruit and put them in the <b>Libraries</b>
folder inside your Arduino folder. 

Open garagedate.ino & put your Router SSID & Password , also put your AIO username & Key into it. Connect your NodeMCU to your computer and go to <b>tools > Port</b> 
and select COM port and compile & load the code to your dev. board

Set Baud Rate of Serial Monitor 115200 for syncing Serial Communication.

![nodemcu_pins](https://user-images.githubusercontent.com/34485667/34895006-b9a297f8-f809-11e7-94f8-736550533f9a.png)

Pin diagram given above will Assist you to map GPIO pin number to your on Board Pin names.
___

## Establishing IFTTT
 
 Sign up on IFTTT and Make your account on [IFTTT](https://ifttt.com/).
 
 ![screenshot 10](https://user-images.githubusercontent.com/34485667/34896129-9752dfb4-f80e-11e7-97cd-d6325e990769.png)

Login to your Account go on Applets Create Applets Accordingly Choosing triggers as Google Assistant & Adafruit respectively 
and response as Adafruit & Notification, and select Appropriate feeds. Finally Switch on your Applets & you are done

![screenshot 13](https://user-images.githubusercontent.com/34485667/35231840-d718c2b8-ffbf-11e7-818c-c3574438d1cf.png)

Now your device is ready to work so connect your device to your Router. 
