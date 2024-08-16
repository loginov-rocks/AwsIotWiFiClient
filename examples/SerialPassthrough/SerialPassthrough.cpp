#include <Arduino.h>

#include <AwsIotWiFiClient.h>
#include <ESP8266WiFi.h>

// Uncomment the following to use the WiFiManager library.
// #include <DNSServer.h>
// #include <ESP8266WebServer.h>
// #include <WiFiManager.h>

#include "Secrets.h"

const char ssid[] = "SSID";
const char password[] = "Password";

AwsIotWiFiClient awsIotWiFiClient;

BearSSL::X509List trustAnchorCertificate(rootCaCertificate);
BearSSL::X509List clientCertificate(deviceCertificate);
BearSSL::PrivateKey clientPrivateKey(privateKeyFile);

String inputMessage = "";

void connectWiFi()
{
  Serial.print("Connecting to Wi-Fi \"");
  Serial.print(ssid);
  Serial.print("\"...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print('.');
  }

  Serial.println();
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Wi-Fi connection was successful!");
}

void receiveMessage(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message received: ");
  Serial.write(payload, length);
  Serial.println();
}

void setupAwsIotWiFiClient()
{
  Serial.println("Setting up AWS IoT Wi-Fi Client...");

  awsIotWiFiClient.setDebugOutput(true)
      .setCertificates(&trustAnchorCertificate, &clientCertificate, &clientPrivateKey)
      .setEndpoint(endpoint)
      .setReceiveMessageCallback(receiveMessage)
      .setClientId(clientId)
      .setSubscribeTopicFilter(subscribeTopicFilter)
      .connect();

  Serial.println("AWS IoT Wi-Fi Client setup was successful!");
}

void setup()
{
  Serial.begin(9600);
  Serial.println("Setup...");

  connectWiFi();
  // Comment the previous line and uncomment the following to use the WiFiManager library.
  // WiFiManager wifiManager;
  // wifiManager.autoConnect("AwsIotWiFiClient");
  // Serial.println("Connected!");

  setupAwsIotWiFiClient();

  Serial.println("Setup was successful!");

  awsIotWiFiClient.publishMessage(publishTopicName, "Hello, world!");
}

void loop()
{
  while (Serial.available() > 0)
  {
    char input = Serial.read();

    if (input == ';')
    {
      Serial.println();
      Serial.println("Publishing message: " + inputMessage);
      const char *charArray = inputMessage.c_str();
      awsIotWiFiClient.publishMessage(publishTopicName, charArray);
      Serial.println("Message published!");
      inputMessage = "";
    }
    else
    {
      Serial.print(input);
      inputMessage += input;
    }
  }

  awsIotWiFiClient.loop();
}
