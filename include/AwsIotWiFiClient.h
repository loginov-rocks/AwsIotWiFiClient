#ifndef AWSIOTWIFICLIENT_H_INCLUDED
#define AWSIOTWIFICLIENT_H_INCLUDED

#include <PubSubClient.h>
#include <WiFiClientSecure.h>

class AwsIotWiFiClient
{
private:
  WiFiClientSecure wiFiSecureClient;
  PubSubClient *pubSubClient;

  boolean debugOutput = false;
  const char *clientId;
  const char *subscribeTopicFilter;

  void setupTime();

public:
  AwsIotWiFiClient();

  AwsIotWiFiClient &setCertificates(const X509List *, const X509List *, const PrivateKey *);
  AwsIotWiFiClient &setEndpoint(const char *);
  AwsIotWiFiClient &setReceiveMessageCallback(std::function<void(char *, uint8_t *, unsigned int)>);

  AwsIotWiFiClient &setDebugOutput(boolean);
  AwsIotWiFiClient &setClientId(const char *);
  AwsIotWiFiClient &setSubscribeTopicFilter(const char *);

  void connect();
  void loop();

  boolean publishMessage(const char *, const char *);
};

#endif
