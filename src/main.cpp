#include "AwsIotWiFiClient.h"
#include "Secrets.h"

AwsIotWiFiClient awsIotWiFiClient;

BearSSL::X509List trustAnchorCertificate(rootCaCertificate);
BearSSL::X509List clientCertificate(deviceCertificate);
BearSSL::PrivateKey clientPrivateKey(privateKeyFile);

void receiveMessage(char *topic, byte *payload, unsigned int length)
{
    //
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

    setupAwsIotWiFiClient();

    Serial.println("Setup was successful!");

    awsIotWiFiClient.publishMessage(publishTopicName, "Hello, world!");
}

void loop()
{
    awsIotWiFiClient.loop();
}
