# Automated Physical Asset Verification using BLE

This repository contains the hardware setup details and source code for an automated asset verification system based on the BLE (Bluetooth Low Energy)

## OVERVIEW

This project uses Mediatek Linkit ONE board as a BLE gateway and Cypress 4.1 BLE / Beacon device as the BLE node. The application server is hosted on Thingsboard and enables users to track their physical assets attached with the BLE node. The LinkIt-One board acts as the BLE gateway that aggregates all beacon signals from the BLE nodes. It detects the presence or absense of assets in a specified area and updates this information on a dashboard hosted by the Thingsboard server.

## INTRODUCTION

This system is assumed to be installed in a Factory space or any close office space where there are movable or portable assets which need to be monitored. The system has two parts

1. Asset Management Server - The application server that runs on Thingsboard and also hosts the dashboard for real-time presence monitoring of assets. This is written in JavaScript.

2. Hardware - BLE Gateway along with the BLE end nodes that are configured as a beacon. Mediatek LinkitONE is the gateway and Cypress 4.1 BLE is the end node. There are three end nodes configured in this applicaiton which means that we can track three assets at a time. The code for hardware is written in C.

## BUILD & INSTALL STEPS

### Build Setup For hardware

The following hardware components are used for this project:

  1.	LinkIt-One v1.0

  2.	Cypress4.1

  3.	BLE and Wifi antenna
  

#### Programming the Linkit ONE Device

If you are familiar with basics of LinkIt One programming using Arduino IDE, then you can skip the following step
and proceed directly to step 2.

#### STEP 1 : LinkIt ONE and Arduino IDE setup

In order to start programming LinkIt One device, you will need Arduino IDE installed and all related libraries. Please follow this
[guide](https://docs.labs.mediatek.com/resource/linkit-one/en/getting-started) in order to install the Arduino IDE and LinkIt One SDK.

The above "Getting Started" guide also contains instructions for updating the LinkitONE firmware and try out your first LinkIt One sample application. 

#### STEP 2 : PubSubClient Library Installation

Open Arduino IDE and go to Sketch -> Include Library -> Manage Libraries. Find PubSubClient by Nick O’Leary and install it.

#### STEP 3 : BLE Application Application Code Config

Clone this repository and open the [application code for BLE gateway](code/hardware/asset_tracking.ino)

You will have to edit the following Macros/variables in the source code as per your network configuration

  •	WIFI_AP (Line 15) - SSID Name of your WiFi Access Point
  
  •	WIFI_PASSWORD (Line 16) - WiFi Password of Access Point
  
  •	WIFI_AUTH (Line 17) - WiFi Auth (choose one between LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP)
  
  •	TOKEN (Line 18) - the $ACCESS_TOKEN from ThingsBoard configuration step (See "Step 6. Copy the Access Token and save it in a text file" under "Creating the Device" section below ).
  
  •	thingsboardServer (Line 25) - The IP Address fo the server hosting Thingsboard
  
  •	BEACON_MAC_ADDR_1 , BEACON_MAC_ADDR_2 , BEACON_MAC_ADDR_3 (Line 21,22,23) - Known mac-addresses of the ble end nodes.
    
    
#### STEP 4 : BLE Application Application Code Install

Connect your LinkItONE device via USB cable and select Serial Debug COM port in Arduino IDE. Compile and Upload your sketch to the device using “Upload” button. Once the application starts, it will try to connect to ThingsBoard server using MQTT. Refer to the steps below to deploy ThingsBoard server to receive the messages from the Linkit ONE.

#### Troubleshooting

When the application is running, you can connect your device to Serial Debug COM port in Arduino IDE and open “Serial Monitor” in order to view debug information produced by serial output.


### Setup For Application Server

[ThingsBoard](https://thingsboard.io/) is a open source IoT platform. For this project we have used a Ubuntu server installed with ThingsBoard for orchestrating the messages between the assets and the dashboard.

#### STEP 1 : Installing ThingsBoard

Install the Thingsboard service [as per your Operating System](https://thingsboard.io/docs/user-guide/install/installation-options/) 

#### STEP 2 : Getting Started (Optional)

Follow this [guide](https://thingsboard.io/docs/getting-started-guides/helloworld/) to get started with building application dashboards with ThingsBoard. If you want to head straight to building the widget dashboard for this application then you can skip this and follow along the next section.

### Setup Dashboard Widget Application Server

The ThingsBoard configuration is done in three steps

1) Creating the Device

2) Creating Widget 

3) Creating Dashboard

#### Creating the Device

##### Step 1. Login to the Thingsboard Server Portal. The home screen will launch

<img src="screenshots/SS-1.jpg">

##### Step 2. Go to the Device Menu and click on the red "+" icon in the bottom right to create a new device.

<img src="screenshots/SS-2.jpg">

<img src="screenshots/SS-3.jpg">

##### Step 3. Enter the device name and description as given below

<img src="screenshots/SS-4.jpg">

##### Step 4. You can see the newly created device card 

<img src="screenshots/SS-5.jpg">

##### Step 5. Click on the device card and click on manage credentials

<img src="screenshots/SS-6.jpg">

##### Step 6. Copy the Access Token and save it in a text file.

<img src="screenshots/SS-7.jpg">

 - Paste the access token in the Linkit ONE program (mentioned in "STEP 3 : BLE Application Application Code Config" under "Programming the Linkit ONE Device" section above) and then perform the procedure to execute the program in LinkitONE.
   
   The hardware will now start publishing the end node's MAC ID along with the presence status. 

<img src="screenshots/SS-8.jpg">

At this point, device creation is complete and we are receiving the actual data from hardware.


