# AwsIotWiFiClient

[![PlatformIO Registry](https://badges.registry.platformio.org/packages/loginov-rocks/library/AwsIotWiFiClient.svg)](https://registry.platformio.org/libraries/loginov-rocks/AwsIotWiFiClient)
[![Arduino Lint](https://github.com/loginov-rocks/AwsIotWiFiClient/actions/workflows/arduino-lint.yml/badge.svg)](https://github.com/loginov-rocks/AwsIotWiFiClient/actions/workflows/arduino-lint.yml)
[![PlatformIO Build](https://github.com/loginov-rocks/AwsIotWiFiClient/actions/workflows/platformio-build.yml/badge.svg)](https://github.com/loginov-rocks/AwsIotWiFiClient/actions/workflows/platformio-build.yml)

Connect NodeMCU (ESP8266) to AWS IoT: [Medium](https://loginov-rocks.medium.com/connect-nodemcu-esp8266-to-aws-iot-b75d2470b40b)

## Quick Start

```cpp
#include "AwsIotWiFiClient.h"
#include "ESP8266WiFi.h"

#include "pgmspace.h"

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

AwsIotWiFiClient awsIotWiFiClient;

BearSSL::X509List trustAnchorCertificate(rootCaCertificate);
BearSSL::X509List clientCertificate(deviceCertificate);
BearSSL::PrivateKey clientPrivateKey(privateKeyFile);

void receiveMessage(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message received: ");
  Serial.write(payload, length);
  Serial.println();
}

void setup()
{
  // Connect to Wi-Fi first!
  // ...

  // Set up AWS IoT Wi-Fi Client:
  awsIotWiFiClient
      // Enable debug output:
      .setDebugOutput(true)
      // Certificates to establish secure communication (defined above):
      .setCertificates(&trustAnchorCertificate, &clientCertificate, &clientPrivateKey)
      // Device Data Endpoint from IoT Core -> Settings:
      .setEndpoint("endpoint.iot.us-east-1.amazonaws.com")
      // Callback that will be triggered when incoming messages are received (defined above):
      .setReceiveMessageCallback(receiveMessage)
      // Client ID aka thing name:
      .setClientId("client")
      // Topic name to subscribe to incoming messages:
      .setSubscribeTopicFilter("topic")
      .connect();
}

void loop()
{
  // ...

  // Publish message "Hello, world!" to the "topic" topic:
  awsIotWiFiClient.publishMessage("topic", "Hello, world!");
  awsIotWiFiClient.loop();
}
```

## Infrastructure View

![Infrastructure View](https://raw.githubusercontent.com/loginov-rocks/AwsIotWiFiClient/main/docs/Infrastructure%20View.png)

## API

### `AwsIotWiFiClient`

<!-- no toc -->
* [AwsIotWiFiClient](#awsiotwificlient)
  * [setCertificates(&trustAnchorCertificate, &clientCertificate, &clientPrivateKey)](#setcertificatestrustanchorcertificate-clientcertificate-clientprivatekey)
  * [setEndpoint(endpoint)](#setendpointendpoint)
  * [setReceiveMessageCallback(callback)](#setreceivemessagecallbackcallback)
  * [setClientId(clientId)](#setclientidclientid)
  * [setSubscribeTopicFilter(subscribeTopicFilter)](#setsubscribetopicfiltersubscribetopicfilter)
  * [setDebugOutput(debugOutput)](#setdebugoutputdebugoutput)
  * [connect()](#connect)
  * [loop()](#loop)
  * [publishMessage(topicName, message)](#publishmessagetopicname-message)

---

#### `setCertificates(&trustAnchorCertificate, &clientCertificate, &clientPrivateKey)`

---

#### `setEndpoint(endpoint)`

---

#### `setReceiveMessageCallback(callback)`

---

#### `setClientId(clientId)`

---

#### `setSubscribeTopicFilter(subscribeTopicFilter)`

---

#### `setDebugOutput(debugOutput)`

---

#### `connect()`

---

#### `loop()`

---

#### `publishMessage(topicName, message)`
