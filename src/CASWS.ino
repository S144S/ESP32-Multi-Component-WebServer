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
#include <DNSServer.h>

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
bool web_request_done = false;

bool new_packet = false;
String packet_data = "";

bool led_state = false;
/*****************************CLASSES*******************************/
AsyncWebServer server(80);
WebSocketsServer webSocket = WebSocketsServer(1337);
DNSServer dnsServer;
IPAddress apIP(192, 168, 1, 1);

DynamicJsonDocument json_data(1024);
/****************************FUNCTIONS******************************/
void system_config();

void server_config();

void websocket_callback(byte client_num, WStype_t type,
                        byte *payload, size_t length);

void page_home_request(AsyncWebServerRequest *request);
void page_second_request(AsyncWebServerRequest *request);
void page_404_request(AsyncWebServerRequest *request);

bool packet_parser(String rec_packet);

bool apply_user_setting();
/****************************MAIN BODY******************************/
void setup() {
    system_config();

    server_config();
}
/*-----------------------------------------------------------------*/
void loop() {
    webSocket.loop();
    if(new_packet)
    {
        packet_parser(packet_data);
        apply_user_setting();

    }
}
/*************************FUNCTIONS BODY****************************/
void system_config() {
    Serial.begin(115200);
    delay(250);

    pinMode(LED, OUTPUT);
    digitalWrite(LED, HIGH);
    pinMode(PUSH_BUTTON, INPUT);

    Serial.println("Welcome to Cayan Async Sample Web Server");
    Serial.println("           Version 1.0.0");
    Serial.println("--------------------------------------------");
}
/*-----------------------------------------------------------------*/
void server_config() {
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(SSID, PASS);
    delay(50);
    Serial.println("SERVER CREATED SUCCESSFULLY");

    server.on("/", HTTP_GET, page_home_request);
    server.on("/second-page", HTTP_GET, page_second_request);
    server.onNotFound(page_404_request);

    server.begin();
    Serial.println("SERVER STARTED");

    webSocket.begin();
    webSocket.onEvent(websocket_callback);
    Serial.println("WEBSOCKET SERVER STARTED");
    Serial.println("--------------------------------------------");
}
/*-----------------------------------------------------------------*/
void websocket_callback(byte client_num, WStype_t type,
                        byte *payload, size_t length) {
    Serial.println("WS:event happend");
    switch(type) 
    {
        // Client has disconnected
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", client_num);
            break;
        // New client has connected
        case WStype_CONNECTED:
        {
            IPAddress ip = webSocket.remoteIP(client_num);
            Serial.printf("[%u] Connection from ", client_num);
            Serial.println(ip.toString());
        }
            break;
        // Handle text messages from client
        case WStype_TEXT:
            // Print out raw message
            new_packet = true;
            Serial.printf("[%u] %s\n", client_num, payload);
            packet_data = String((char*)( payload));
            Serial.println(packet_data);
            if(!packet_data.startsWith("{"))
            {
                new_packet = false;
            }
            break;
        // For everything else: do nothing
        case WStype_BIN:
        case WStype_ERROR:
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
        default:
            break;
    }
    
    web_request_done = true;

}
/*-----------------------------------------------------------------*/
void page_home_request(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send_P(200, "text/html", home_page);
}
/*-----------------------------------------------------------------*/
void page_second_request(AsyncWebServerRequest *request) {
  IPAddress remote_ip = request->client()->remoteIP();
  Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send_P(200, "text/html", second_page);
}
/*-----------------------------------------------------------------*/
void page_404_request(AsyncWebServerRequest *request) {
    IPAddress remote_ip = request->client()->remoteIP();
    Serial.println("[" + remote_ip.toString() +
                  "] HTTP GET request of " + request->url());
  request->send(404, "text/html", page_not_found);
}
/*-----------------------------------------------------------------*/
bool packet_parser(String rec_packet) {
    Serial.print("PACKET NEED TO BE PARSED: ");
    Serial.println(rec_packet);
    //deserializeJson(json_data, rec_packet);    
    DeserializationError error = deserializeJson(json_data, rec_packet);
    if (error) 
    {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return false;
    }
    led_state = json_data["LED"];
    Serial.println(led_state);
    new_packet = false;
    return true;
}
/*-----------------------------------------------------------------*/
bool apply_user_setting() {
    digitalWrite(LED, !led_state);

    return true;
}
/*-----------------------------------------------------------------*/
