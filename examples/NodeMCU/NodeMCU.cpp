#include <Arduino.h>

#include <AwsIotWiFiClient.h>
#include <ESP8266WiFi.h>

#include <pgmspace.h>

const char endpoint[] = "endpoint.iot.us-east-1.amazonaws.com";
const char clientId[] = "client";
const char publishTopicName[] = "topic";
const char subscribeTopicFilter[] = "topic";

// Device certificate
// Copy and paste contents from *-certificate.pem.crt
static const char deviceCertificate[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)KEY";

// Key files / Private key file
// Copy and paste contents from *-private.pem.key
static const char privateKeyFile[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----

-----END RSA PRIVATE KEY-----
)KEY";

// RSA 2048 bit key: Amazon Root CA 1
// Copy and paste contents from AmazonRootCA1.cer
static const char rootCaCertificate[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----

-----END CERTIFICATE-----
)EOF";

const char ssid[] = "SSID";
const char password[] = "Password";

AwsIotWiFiClient awsIotWiFiClient;

BearSSL::X509List trustAnchorCertificate(rootCaCertificate);
BearSSL::X509List clientCertificate(deviceCertificate);
BearSSL::PrivateKey clientPrivateKey(privateKeyFile);

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
  setupAwsIotWiFiClient();

  Serial.println("Setup was successful!");

  awsIotWiFiClient.publishMessage(publishTopicName, "Hello, world!");
}

void loop()
{
  awsIotWiFiClient.loop();
}
