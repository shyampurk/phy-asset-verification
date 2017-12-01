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

##### Troubleshooting

When the application is running, you can connect your device to Serial Debug COM port in Arduino IDE and open “Serial Monitor” in order to view debug information produced by serial output.


#### Programming the Cypress 4.1 PSoC BLE Node

You will need PSoC Designer for this.

Follow this link to get the [getting started guide](http://www.cypress.com/documentation/other-resources/psoc-designer-quick-start-guide)

The code used for progralling is based on this [EddyStone sample program](https://github.com/cypresssemiconductorco/PSoC-4-BLE/blob/master/100_Projects_in_100_Days/Project050_Eddystone/Eddystone/Eddystone_EH_Kit.cydsn/Eddystone.c). 

### Setup For Application Server

[ThingsBoard](https://thingsboard.io/) is a open source IoT platform. For this project we have used a Ubuntu server installed with ThingsBoard for orchestrating the messages between the assets and the dashboard.

#### STEP 1 : Installing ThingsBoard

Install the Thingsboard service [as per your Operating System](https://thingsboard.io/docs/user-guide/install/installation-options/) 

#### STEP 2 : Getting Started (Optional)

Follow this [guide](https://thingsboard.io/docs/getting-started-guides/helloworld/) to get started with building application dashboards with ThingsBoard. If you want to head straight to building the widget dashboard for this application then you can skip this and follow along the next section.

### Setup For Dashboard Widget Application Server

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

#### Creating the Widget

Now we have to create a widget for displaying the status of each BLE node.  Head over to the "WIDGETS LIBRARY" section in the ThingsBoard server portal.

<img src="screenshots/SS-9.jpg">


Follow the below steps to create a customized widget for displaying the presence status of the end nodes.

##### Step  1. Under the "WIDGET LIBRARY" screen, click on the red "+" icon in the bottom right and choose "Create New Widgets Bundle".

<img src="screenshots/SS-10.jpg">


##### Step 2. Enter the widget bundle name as shown in the screen below

<img src="screenshots/SS-11.jpg">


You will see the card representing the newly created widget bundle

<img src="screenshots/SS-12.jpg">


##### Step 3. Go back to the "DEVICES" screen, select the "FACTORY ASSET DEVICE" and choose the "ATTRIBUTES" tab

<img src="screenshots/SS-13.jpg">


##### Step 4. Select the first data attribute received from one of the end nodes and click on "SHOW ON WIDGET"

<img src="screenshots/SS-14.jpg">


##### Step 5. Select "Cards" from the "Current Bundle" dropdown option and choose the third style from the four available styles.

<img src="screenshots/SS-15.jpg">


##### Step 6. Click on Add to Dashboard choose "Create New Dashboard" and enter the name as given and click "ADD".

<img src="screenshots/SS-16.jpg">


##### Step 7. Go to "WIDGET LIBRARY" screen and select the widge named "Cards" . 

<img src="screenshots/SS-17.jpg">


This is the predefined widget that we selected for dislaying the aseet status but we will modify it.

##### Step 8. This will open up a few widgets showing random data. We will start with the Temp widget for our BLE end nodes.

<img src="screenshots/SS-18.jpg">


##### Step 9. Click on the edit icon on the Temp widget.

<img src="screenshots/SS-19.jpg">


This will open up the code editing screen for the widget. 

<img src="screenshots/SS-20.jpg">

##### Step 10. Now we need to replace the Javascript and CSS code with our code. 

<img src="screenshots/SS-21.jpg">

##### Step 11..Copy the Javascript code and CSS code and paste it in the respective code windoow replacing the existing code completely and click on the "SAVE AS" button on the top.

Replace the default JS code with our [custom JS code](code/thingsboard/asset_tracking.js)

Replace the default CSS code with our [custom CSS code](code/thingsboard/asset_tracking.css)

<img src="screenshots/SS-22.jpg">

##### Step 12. You have to give a new title for the widget and add it to an widget bundle as follows

<img src="screenshots/SS-23.jpg">

Once done, you will see your newly created widget as shown below

<img src="screenshots/SS-24.jpg">

##### Step 13. Now, head over to the "WIDGETS LIBRARY" and select "FACTORY ASSET". 

You will now see the asset widget as per the new design and style that we did in step 10 and 11 above.

<img src="screenshots/SS-25.jpg">

You have successfully created the widget. Now lets add it to a dashboard.


#### Creating the Dashboard

We already defined the dashboard as part of step 6 of widget creation. Now we will add the widget to the dashboard.

##### Step 1. Go to the "DEVICES" screen and click on the "FACTORY ASSET DEVICE"

<img src="screenshots/SS-26.jpg">

##### Step 2. Select "ATTRIBUTES" tab, check the first data record and click on "SHOW ON WIDGET"

<img src="screenshots/SS-27.jpg">

##### Step 3. Select "FACTORY ASSET" from the "Current Bundle" dropdown and click on "ADD TO DASHBOARD"

<img src="screenshots/SS-28.jpg">

##### Step 4. Select the "Select existing dashboard" option and choose "FACTORY ASSET DASHBOARD" and click "ADD"

<img src="screenshots/SS-29.jpg">

##### Step 5. Now go to the "DASHBOARDS" screen and select "FACTORY ASSET DASHBOARD"

<img src="screenshots/SS-30.jpg">

You can see that the widget is now added to the dashboard.

<img src="screenshots/SS-31.jpg">

##### Step 6. Enter the edit mode by clicking the edit icon on the right bottom of dashboard screen.

<img src="screenshots/SS-32.jpg">

You will now get the edit option for the widget.

<img src="screenshots/SS-33.jpg">

##### Step 7. Click on the edit icon of the widget.

<img src="screenshots/SS-34.jpg">

##### Step 8. Click on the edit icon for Timeseries

<img src="screenshots/SS-35.jpg">

##### Step 9. Change the Label to A1 to reflect the first asset.

<img src="screenshots/SS-36.jpg">

##### Step 10. Click the right icon to apply changes

<img src="screenshots/SS-37.jpg">

##### Step 11. Finally, the widget will look like this.

<img src="screenshots/SS-38.jpg">

This is the widget for showing the status of first asset (A1)

You need to perform the above steps ( from step 2 above) to create widget for A2 and A3. For this you have to 

create the widget separately from the attributes list of data record by selecting the second record (for A2) and 

third record (for A3).

<img src="screenshots/SS-39.jpg">

The final set of widgets for the three assets will look like this

<img src="screenshots/SS-40.jpg">

 
