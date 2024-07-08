#include "Arduino.h"

#include "ESP8266WiFi.h"

#include "DNSServer.h"
#include "ESP8266WebServer.h"
#include "WiFiManager.h"

#include "AwsIotWiFiClient.h"
#include "Secrets.h"

AwsIotWiFiClient awsIotWiFiClient;

BearSSL::X509List trustAnchorCertificate(rootCaCertificate);
BearSSL::X509List clientCertificate(deviceCertificate);
BearSSL::PrivateKey clientPrivateKey(privateKeyFile);

String inputMessage = "";

void receiveMessage(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message received: ");
  Serial.write(payload, length);
  Serial.println();
}

void setupAwsIotWiFiClient()
{
  Serial.println("Setup AWS IoT Wi-Fi Client...");

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

  WiFiManager wifiManager;
  wifiManager.autoConnect("AwsIotWiFiClient");
  Serial.println("Connected!");

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
