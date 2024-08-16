#ifndef SECRETS_H_INCLUDED
#define SECRETS_H_INCLUDED

#include <pgmspace.h>

const char endpoint[] = "endpoint.iot.us-east-1.amazonaws.com";
const char clientId[] = "client";
const char publishTopicName[] = "topic";
const char subscribeTopicFilter[] = "topic";

// Download certificates and keys

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

#endif
