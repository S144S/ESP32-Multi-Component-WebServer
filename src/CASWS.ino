/////////////////////////////////////////////////////////////////////
// NAME          :     ESP32 Bidirectional Server
//
// Version       :     1.0.0
//
// PURPOSE       :     Test Real-Time Send and Recieve
//                     Data From/To Server and Test
//                     Different Componentes on Web Page
//
// DESCRIBTION   :     This ESP32 server has a page containing 
//                     simple button, tabs, chart,
//                     string box(for send and receive), Images,
//                     table,  radio button, slider,
//                     analog guage and ...
//
// AUTHOR        :     Saeed Hosseini
//                     <saeed144.73@gmail.com>
//                     <https://github.com/S144S>
// DATE          :     14-September-2021
//
/////////////////////////////////////////////////////////////////////
/*****************************LIBRARIES*****************************/
#include <Arduino.h>

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>
#include <ESPmDNS.h>

#include <ArduinoJson.h>

#include "Webpages.h"

/****************************CONSTANTS******************************/
const char *SSID        ="CAYAN_AWS";
const char *PASS        ="12345678";
const char *DOMAIN_NAME ="casws";
const int HTTP_PORT     =80;
const int DNS_PORT      =53;
const int WS_PORT       =1337;

const int LED           =5; 
const int PUSH_BUTTON   =32;
/***************************PUBLIC VARS*****************************/
/*****************************CLASSES*******************************/
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
/****************************FUNCTIONS******************************/
void system_config();

void server_config();

void page_404_request(AsyncWebServerRequest *request);
/****************************MAIN BODY******************************/
void setup() {
    system_config();

    server_config();
}
/*-----------------------------------------------------------------*/
void loop() {

}
/*************************FUNCTIONS BODY****************************/
void system_config() {
    Serial.begin(115200);
    delay(250);

    pinMode(LED, OUTPUT);
    pinMode(PUSH_BUTTON, INPUT);

    Serial.println("Welcome to Cayan Async Sample Web Server");
    Serial.println("           Version 1.0.0");
    Serial.println("--------------------------------------------");
}
/*-----------------------------------------------------------------*/
void server_config() {
    WiFi.softAP(SSID, PASS);
    delay(50);

    if(!MDNS.begin(DOMAIN_NAME))
    {
        Serial.println("SERVER DOES NOT ACCEPT DOMAIN NAME");
        delay(500);
        while(true)
        {
            // System stop working
        }
    }
    Serial.println("SERVER CREATED SUCCESSFULLY");

    server.onNotFound(page_404_request);

    server.begin();
    Serial.println("SERVER STARTED");
    Serial.println("--------------------------------------------");
}
/*-----------------------------------------------------------------*/
void page_404_request(AsyncWebServerRequest *request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(404, "text/html", page_not_found);
}
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
/*-----------------------------------------------------------------*/
