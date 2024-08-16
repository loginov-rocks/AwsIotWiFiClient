#include <AwsIotWiFiClient.h>

void AwsIotWiFiClient::setupTime()
{
  time_t nowish = 1510592825;

  if (debugOutput)
  {
    Serial.print("Setting time using SNTP");
  }

  // configTime(TIME_ZONE * 3600, 0 * 3600, "pool.ntp.org", "time.nist.gov");
  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  time_t now = time(nullptr);

  if (debugOutput)
  {
    while (now < nowish)
    {
      delay(500);
      Serial.print(".");
      now = time(nullptr);
    }
    Serial.println("done!");
  }

  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);

  if (debugOutput)
  {
    Serial.print("Current time: ");
    Serial.print(asctime(&timeinfo));
  }
}

AwsIotWiFiClient::AwsIotWiFiClient() : wiFiSecureClient()
{
  pubSubClient = new PubSubClient(wiFiSecureClient);
}

AwsIotWiFiClient &AwsIotWiFiClient::setDebugOutput(boolean _debugOutput)
{
  debugOutput = _debugOutput;

  return *this;
}

AwsIotWiFiClient &AwsIotWiFiClient::setCertificates(const X509List *trustAnchorCertificate, const X509List *clientCertificate, const PrivateKey *clientPrivateKey)
{
  setupTime();

  wiFiSecureClient.setTrustAnchors(trustAnchorCertificate);
  wiFiSecureClient.setClientRSACert(clientCertificate, clientPrivateKey);

  return *this;
}

AwsIotWiFiClient &AwsIotWiFiClient::setEndpoint(const char *endpoint)
{
  pubSubClient->setServer(endpoint, 8883);

  return *this;
}

AwsIotWiFiClient &AwsIotWiFiClient::setReceiveMessageCallback(std::function<void(char *, uint8_t *, unsigned int)> callback)
{
  pubSubClient->setCallback(callback);

  return *this;
}

AwsIotWiFiClient &AwsIotWiFiClient::setClientId(const char *_clientId)
{
  clientId = _clientId;

  return *this;
}

AwsIotWiFiClient &AwsIotWiFiClient::setSubscribeTopicFilter(const char *_subscribeTopicFilter)
{
  subscribeTopicFilter = _subscribeTopicFilter;

  return *this;
}

void AwsIotWiFiClient::connect()
{
  if (debugOutput)
  {
    Serial.println("Connecting to AWS IOT");
  }

  while (!pubSubClient->connect(clientId))
  {
    if (debugOutput)
    {
      Serial.print(".");
    }
    delay(1000);
  }

  if (!pubSubClient->connected())
  {
    if (debugOutput)
    {
      Serial.println("AWS IoT Timeout!");
    }
    return;
  }

  pubSubClient->subscribe(subscribeTopicFilter);

  if (debugOutput)
  {
    Serial.println("AWS IoT Connected!");
  }
}

void AwsIotWiFiClient::loop()
{
  if (!pubSubClient->connected())
  {
    connect();
  }

  pubSubClient->loop();
}

boolean AwsIotWiFiClient::publishMessage(const char *topicName, const char *message)
{
  return pubSubClient->publish(topicName, message);
}
