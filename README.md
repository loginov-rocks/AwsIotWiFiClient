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
      // Callback function that will be triggered when incoming messages are received (defined above):
      .setReceiveMessageCallback(receiveMessage)
      // MQTT client ID aka thing name:
      .setClientId("client")
      // MQTT topic filter to subscribe to incoming messages:
      .setSubscribeTopicFilter("topic")
      // Connect to the AWS IoT service:
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

AWS IoT Wi-Fi Client class is the core component of the library, encapsulating all the functionalities needed to
connect an ESP8266 microcontroller to AWS IoT Core. It manages the setup of secure SSL/TLS communication using
[WiFiClientSecure](https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/WiFiClientSecureBearSSL.h)
and facilitates MQTT messaging via [PubSubClient](https://registry.platformio.org/libraries/knolleary/PubSubClient).

The class provides methods to configure device-specific credentials, establish connections, and subscribe to and
publish MQTT messages. Additionally, it includes a callback mechanism to process incoming messages.

**Kind**: class

<!-- no toc -->
* [AwsIotWiFiClient](#awsiotwificlient-1)
  * [setCertificates(&trustAnchorCertificate, &clientCertificate, &clientPrivateKey)](#setcertificatestrustanchorcertificate-clientcertificate-clientprivatekey)
  * [setEndpoint(endpoint)](#setendpointendpoint)
  * [setReceiveMessageCallback(callback)](#setreceivemessagecallbackcallback)
  * [setClientId(clientId)](#setclientidclientid)
  * [setSubscribeTopicFilter(subscribeTopicFilter)](#setsubscribetopicfiltersubscribetopicfilter)
  * [setDebugOutput(debugOutput)](#setdebugoutputdebugoutput)
  * [connect()](#connect)
  * [loop()](#loop)
  * [publishMessage(topicName, message) ⇒ boolean](#publishmessagetopicname-message--boolean)

---

#### `setCertificates(&trustAnchorCertificate, &clientCertificate, &clientPrivateKey)`

Set certificates to establish secure communication.

**Kind**: instance method of `AwsIotWiFiClient`

| Parameter               | Type          | Description                             |
| ----------------------- | ------------- | --------------------------------------- |
| &trustAnchorCertificate | `X509List*`   | Pointer to the trust anchor certificate |
| &clientCertificate      | `X509List*`   | Pointer to the Client certificate       |
| &clientPrivateKey       | `PrivateKey*` | Pointer to the Client private key       |

---

#### `setEndpoint(endpoint)`

Set Device Data Endpoint from IoT Core -> Settings.

**Kind**: instance method of `AwsIotWiFiClient`

| Parameter | Type    | Description          |
| --------- | ------- | -------------------- |
| endpoint  | `char*` | Device Data Endpoint |

---

#### `setReceiveMessageCallback(callback)`

Set the callback function that will be triggered when incoming messages are received.

**Kind**: instance method of `AwsIotWiFiClient`

| Parameter | Type                                                   | Description                                           |
| --------- | ------------------------------------------------------ | ----------------------------------------------------- |
| callback  | `std::function<void(char *, uint8_t *, unsigned int)>` | Function to call when an incoming message is received |

Example:

```cpp
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message received: ");
  Serial.write(payload, length);
  Serial.println();
}
```

---

#### `setClientId(clientId)`

Set the MQTT client ID aka thing name.

**Kind**: instance method of `AwsIotWiFiClient`

| Parameter | Type    | Description    |
| --------- | ------- | -------------- |
| clientId  | `char*` | MQTT client ID |

---

#### `setSubscribeTopicFilter(subscribeTopicFilter)`

Set the MQTT topic filter to subscribe to incoming messages.

**Kind**: instance method of `AwsIotWiFiClient`

| Parameter            | Type    | Description       |
| -------------------- | ------- | ----------------- |
| subscribeTopicFilter | `char*` | MQTT topic filter |

---

#### `setDebugOutput(debugOutput)`

Enable or disable debug output. Enabled by default.

**Kind**: instance method of `AwsIotWiFiClient`

| Parameter   | Type      | Description                                       |
| ----------- | --------- | ------------------------------------------------- |
| debugOutput | `boolean` | `true` to enable, `false` to disable debug output |

---

#### `connect()`

Connect to the AWS IoT service.

**Kind**: instance method of `AwsIotWiFiClient`

---

#### `loop()`

Keep the MQTT connection alive and process incoming messages. This should be called regularly in the main loop.

**Kind**: instance method of `AwsIotWiFiClient`

---

#### `publishMessage(topicName, message)` ⇒ `boolean`

Publish a message to the specified MQTT topic.

**Kind**: instance method of `AwsIotWiFiClient`

**Returns**: `boolean` - `true` in case of success, or `false` otherwise

| Parameter | Type    | Description                  |
| --------- | ------- | ---------------------------- |
| topicName | `char*` | The MQTT topic to publish to |
| message   | `char*` | The message payload          |
