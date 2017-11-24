/*********************************************************************************
ASSET TRACKING SYSTEM
*********************************************************************************/

//Import the Libraries Required 
#include <PubSubClient.h>
#include <LWiFiClient.h>
#include <LGATTClient.h>
#include <LGATTServer.h>
#include <string.h>
#include <LGATT.h>
#include <LWiFi.h>

//Configuring WiFi AP and password
#define WIFI_AP "IoTracX"
#define WIFI_PASSWORD "iotracx@123"            //set id and password for WIFI
#define WIFI_AUTH LWIFI_WPA                           // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.
#define TOKEN "eMHVoRDkyDDpUVZWeX8N"           //set Acess Token

//Beacon MAC Addresses
#define BEACON_MAC_ADDR_1 "00a050122c97"      //Configure the known mac-addressess of your beacons
#define BEACON_MAC_ADDR_2 "00a0501216d8"
#define BEACON_MAC_ADDR_3 "00a050121451"

char thingsboardServer[] = "192.168.1.7";         //set host ip address
LWiFiClient wifiClient;
LGATTClient ble;
static LGATTUUID test_uuid;
PubSubClient client( wifiClient );
unsigned long lastSend;

/******************************************************************************************
  Function Name  : InitWiFi
  Description    : Connecting to AP by interfacing with WiFi
  Parameters     : void
  Return         : void
*****************************************************************************************/
void InitLWiFi()
{
  LWiFi.begin();                                 //wifi begins here
  Serial.println("Connecting to AP ...");        // Keep retrying until connected to AP
  while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
  {
    delay(500);
  }
  Serial.println("Connected to AP");
}

/******************************************************************************************
  Function Name : setup
  Description   : Connecting to Bluetooth and initializing Wifi with Thngsboard Server.
  Parameters    : void
  Return        : void
*****************************************************************************************/
void setup()
{
  Serial.begin(19200);
  InitLWiFi();
  Serial.println("Beacon Solution started.");
  client.setServer( thingsboardServer, 1883 );    //setting server
  lastSend = 0;
  if (ble.begin(test_uuid))
  {
    Serial.println("LGATTC begin() success to register.");
  }
  else
  {
    Serial.println("LGATTC begin() failed to register.");
    delay(500);
  }
}

/******************************************************************************************
  Function Name : wifi_status
  Description   : Different cases of WiFi connection status
  Parameters    : LWifiStatus ws - Wifi Connection Status Object 
  Return        : boolean
*****************************************************************************************/
boolean wifi_status(LWifiStatus ws)           //choosing wifi status
{
  switch (ws)
  {
    case LWIFI_STATUS_DISABLED:
      return false;
      break;
    case LWIFI_STATUS_DISCONNECTED:
      return false;
      break;
    case LWIFI_STATUS_CONNECTED:
      return true;
      break;
  }
  return false;
}

/******************************************************************************************
  Function Name : reconnect
  Description   : Reconnecting to Bluetooth and initializing Wifi with Thngsboard Server.
  Parameters    : void
  Return        : void
*****************************************************************************************/
void reconnect()
{
  while (!client.connected())                                         // Loop until we're reconnected
  {
    Serial.print("Connecting to ThingsBoard node ...");
    if ( client.connect("LinkIt One Device", TOKEN, NULL) )           // Attempt to connect (clientId, username, password)
    {
      Serial.println( "[DONE]" );
    }
    else
    {
      Serial.print( "[FAILED] rc = " );
      Serial.print( client.state() );
      Serial.println( " : retrying in 5 seconds]" );
      delay( 5000 );                                                 // Wait 5 seconds before retrying
    }
  }
}

/******************************************************************************************
  Function Name : loop
  Description   : Reconnecting to wifi
  Parameters    : void
  Return        : void
*****************************************************************************************/
void loop()
{
  LWifiStatus ws = LWiFi.status();
  boolean status = wifi_status(ws);
  if (!status) {
    Serial.println("Connecting to AP ...");
    while (0 == LWiFi.connect(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD)))
    {
      delay(500);
    }
    Serial.println("Connected to AP");
  }

  if ( !client.connected() ) {
    reconnect();
  }

  if ( millis() - lastSend > 1000 ) { // Update and send only after 1 seconds
    beaconsenData();
    lastSend = millis();
  }

  client.loop();
}

/******************************************************************************************
  Function Name : beaconsenData
  Description   : Scanning the beacons and comparing with our beacons mac-address for checking its presence status
  Parameters    : void
  Return        : void
*****************************************************************************************/
void beaconsenData()
{
  LGATTDeviceInfo info = { 0 };
  int count[3] = {0, 0, 0};
  while (1)
  {
    int32_t num = 0;
    char com_macid[3][18] = {BEACON_MAC_ADDR_1, BEACON_MAC_ADDR_2, BEACON_MAC_ADDR_3};   //Store the known mac-address of the beacons
    int i, len = 0, j, k = 0;
    char temp[13];                                                              //Store the temporary scanned mac-address of the beacons
    char macadd[10][13];
    char buff[3];
    char json[100] = "{";                                                       //Starting the json string to publish it in thingsboard server

    //Step 1. Scanning the Beacons 
    num = ble.scan(5);
    Serial.printf("Number of Devices Found %d", num);
    Serial.println();
    for (i = 0; i < num ; i++)
    {
      boolean ret = ble.getScanResult(i, info);                                //Scanning the mac-address of the beacons
      sprintf(temp, "%02x%02x%02x%02x%02x%02x", info.bd_addr.addr[5], info.bd_addr.addr[4], info.bd_addr.addr[3], info.bd_addr.addr[2], info.bd_addr.addr[1], info.bd_addr.addr[0]);
      Serial.print("Scanned beacons are :");
      Serial.println(temp);

      temp[12] = '\0';
      macadd[len][0] = '\0';
      strcpy(macadd[len], temp);
      macadd[len][13] = '\0';
      len++;    
    }  
    if (len == 0) 
    {
      count[0] = 0;
      count[1] = 0;
      count[2] = 0;
    }
    
    // Step 2. Comapring the scanned Beacons with known Beacons
    for (i = 0; i < 3; i++)
    {
      for (j = 0; j < len; j++)
      {
        if (strcmp(com_macid[i], macadd[j]) == 0)               //Comparing the known mac-address with scannned mac-address and count it
        {
          count[i]++;
          Serial.printf("%s = %d \n", macadd[j], count[i]);
          break;
        }
        else if (j == len - 1)
        {
          count[i] = 0;
          Serial.printf("%s = %d \n", com_macid[i], count[i]);
        }
      }
      
      //Adding the data in JSON string
      itoa(count[i], buff, 10);                                   //Conversion of integer to array 
      strcat(json, "\"");
      strcat(json, com_macid[i]);
      strcat(json, "\"");
      strcat(json, ":");
      strcat(json, buff);

      if (i < sizeof(com_macid) / sizeof(com_macid[0]) - 1)
      {
        strcat(json, ",");
      }
    }
    strcat(json, "}");                                          //End of the json string
    Serial.println("JSON DATA is");
    Serial.println(json);

    //Step 4. Publishing the data to Thingsboard Server
    client.publish( "v1/devices/me/attributes", json );        //publishing the json string on the topic "v1/devices/me/attributes" in Thingsboard server
  }
  delay(1000);
}

/********************************************End of the Program************************************************/
