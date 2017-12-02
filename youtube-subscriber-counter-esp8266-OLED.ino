#include <YoutubeApi.h>
#include "SSD1306.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

SSD1306  display(0x3c, 5 /*D1*/, 4 /*D2*/);

//------- Replace the following! ------
char ssid[] = "****";       // your network SSID (name)
char password[] = "****";  // your network key
#define API_KEY "****"  // your google apps API Token
#define CHANNEL_ID "UCNFcfQTu48PPvFZgioPBpAA" // makes up the url of channel

WiFiClientSecure client;
YoutubeApi api(API_KEY, client);

int api_mtbs = 60000; //mean time between api requests
long api_lasttime;   //last time api request has been done

long subs = 0;
void setup() {

  Serial.begin(115200);

  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
}

void loop() {
  if (millis() > api_lasttime + api_mtbs)  {
    if (api.getChannelStatistics(CHANNEL_ID))
    {
      Serial.println("---------Stats---------");
      Serial.print("Subscriber Count: ");
      Serial.println(api.channelStats.subscriberCount);
      Serial.print("View Count: ");
      Serial.println(api.channelStats.viewCount);
      Serial.print("Comment Count: ");
      Serial.println(api.channelStats.commentCount);
      Serial.print("Video Count: ");
      Serial.println(api.channelStats.videoCount);
      // Probably not needed :)
      //Serial.print("hiddenSubscriberCount: ");
      //Serial.println(api.channelStats.hiddenSubscriberCount);
      Serial.println("------------------------");

    }
    api_lasttime = millis();
  }

int subcountme = api.channelStats.subscriberCount;

  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 0, "YoutubeStats");

  if (subcountme == 0)
{
  display.setFont(ArialMT_Plain_16);
  display.drawString(64, 24, "loading ...");
  }
else {
  display.setTextAlignment(TEXT_ALIGN_RIGHT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(30, 27, "Subs:");
  
  display.setFont(ArialMT_Plain_16);
  display.drawString(120, 25, String(api.channelStats.subscriberCount));
  
  display.setFont(ArialMT_Plain_10);
  display.drawString(30, 47, "Views:");
  
  display.setFont(ArialMT_Plain_16);
  display.drawString(120, 45, String(api.channelStats.viewCount));
  }
  
  

  
  //display.drawString(12, 32, "Video:");
  //display.drawString(72, 32, String(api.channelStats.videoCount));

  display.display();
}
