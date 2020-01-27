/*******************************************************************************
 * Copyright (c) 2012, 2017 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution. 
 *
 * The Eclipse Public License is available at 
 *   http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at 
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Ian Craggs - initial contribution
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MQTTClient.h"

//#define ADDRESS     "ssl://mqtt.eclipse.org:8883"
#define ADDRESS     "ssl://localhost:8883"
#define CLIENTID    "ExampleClientPub"
#define TOPIC       "MQTT Examples"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

const char* ca_cert = \
"-----BEGIN CERTIFICATE-----\n\
MIIDZjCCAk4CCQC21WK659buzzANBgkqhkiG9w0BAQsFADB1MQswCQYDVQQGEwJE\n\
RTESMBAGA1UECAwJRnJhbmtvbmlhMRIwEAYDVQQHDAlOdXJlbWJlcmcxHTAbBgNV\n\
BAoMFFRlYW0gU2Vuc29ycyAmIFByb2JlMR8wHQYDVQQDDBZtb21vX2ludGVncmF0\n\
aW9uX3Rlc3RzMB4XDTE5MDQwMzEyMzUwNloXDTI5MDMzMTEyMzUwNlowdTELMAkG\n\
A1UEBhMCREUxEjAQBgNVBAgMCUZyYW5rb25pYTESMBAGA1UEBwwJTnVyZW1iZXJn\n\
MR0wGwYDVQQKDBRUZWFtIFNlbnNvcnMgJiBQcm9iZTEfMB0GA1UEAwwWbW9tb19p\n\
bnRlZ3JhdGlvbl90ZXN0czCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n\
ANfQd3pmcBLI6f/RiecL+C05CC4NZoC8nfse/0EirYAg4pt3N+dJjPXzYpM4CwAz\n\
fmU8irYOruQmokV4/XDBqOXpevDsXsta1RjknXfukyPyx6TE+DmEaqCf72HG1vXn\n\
Z2RsU5kEcvf3phisvFtbKNtEwLN2HqQjkE0Rg9QKwiMk1gSx32cxx6SioQg/q6Xb\n\
j+r8eaVhUiCIkFJrPQwPwq88Cr7anXpkwJUxQhv9dIAjnC1tKB5CftrB3xosieNa\n\
uiHxW8Q94GTZ4MuTO8Vj3R1shekvz1bRAq6ZDdj7SD3PD+IOpWIkMdAYz4ABoYcM\n\
jSxy9af0G0rYbH4j1+6noy0CAwEAATANBgkqhkiG9w0BAQsFAAOCAQEAvkqQwkhk\n\
F53fSio8vN2zzo+U/qQGgJrc6OpgrFjasg4ZxptU2G3OWPmB1tAhz7f9enGzJ9Fp\n\
m9gseH6niTzZ0Oqo4FGqV8y5RCDvgfoY2qL8rQo7vdaS2bqg8fyV6znvAsu1MQFl\n\
6UMv2EmtpljCdJhsHT5SZnM/Zx+7tLMzCtMgjRTapTyRyhAt1+4tpQv1JWmnCChv\n\
0+6gi/4B+kO6vowtNPd0DoHnccm8eP+FdHaI5ZMrDFs2KF/G4t2mqdAXbRqQyJh6\n\
eu1zEX/w3OzxQG2N9nXbbR/tRwV6a1lDFFYsZIxvXfzdrNCq3yzoRTGR0G6QEQ37\n\
pvnXVA+1PJ1GKg==\n\
-----END CERTIFICATE-----\n\
";

const char* mqtt_client_crt = \
"Certificate:\n\
    Data:\n\
        Version: 3 (0x2)\n\
        Serial Number: 4 (0x4)\n\
    Signature Algorithm: sha1WithRSAEncryption\n\
        Issuer: C=DE, ST=Frankonia, L=Nuremberg, O=Team Sensors & Probe, CN=momo_integration_tests\n\
        Validity\n\
            Not Before: Apr  3 15:24:24 2019 GMT\n\
            Not After : Mar 31 15:24:24 2029 GMT\n\
        Subject: C=DE, ST=Frankonia, O=Team Sensors & Probe, CN=momo_integration_tests\n\
        Subject Public Key Info:\n\
            Public Key Algorithm: rsaEncryption\n\
                Public-Key: (2048 bit)\n\
                Modulus:\n\
                    00:b6:2f:4f:f8:ac:dc:ee:9a:88:10:f4:69:57:47:\n\
                    19:e8:e6:d4:c8:e7:0c:64:04:de:78:12:9f:10:ba:\n\
                    76:bb:5c:eb:cc:9e:82:5f:37:08:f6:7f:fe:9b:41:\n\
                    f4:49:f5:4f:fe:d2:a5:70:8c:c1:ec:af:77:6f:d3:\n\
                    4c:8b:b6:42:a3:94:43:d1:b8:06:07:79:06:8b:f8:\n\
                    12:08:63:be:f0:2a:48:71:d2:8a:82:a7:27:9b:42:\n\
                    c8:5c:a9:a7:d4:6b:ce:50:95:c6:00:bb:18:89:5a:\n\
                    d0:0a:20:73:99:f9:e9:9f:a4:51:36:ee:72:35:74:\n\
                    1e:db:21:9a:4d:5f:41:1c:33:a2:8c:b1:fd:c3:c9:\n\
                    7a:d5:f0:d1:62:d4:a4:cc:f1:d3:aa:d1:f5:09:35:\n\
                    a9:d8:ba:28:23:2f:5d:82:fc:a0:21:2b:4e:51:0a:\n\
                    54:f7:6f:4f:c5:6b:61:d0:44:dd:1f:b9:6f:37:e7:\n\
                    2c:8a:0d:f4:d3:d2:88:2c:dd:a4:e7:0b:0f:e5:19:\n\
                    45:69:ef:6b:f0:2c:26:c5:79:2c:1f:a1:ed:d0:d3:\n\
                    94:14:30:13:e9:2c:ee:4c:87:3b:89:03:3e:fa:ce:\n\
                    1b:7b:10:23:d8:31:4f:6a:4c:75:5d:4c:a7:26:a7:\n\
                    ee:13:18:fc:4c:b2:ca:5f:46:a9:c1:5a:f3:d9:26:\n\
                    62:cf\n\
                Exponent: 65537 (0x10001)\n\
        X509v3 extensions:\n\
            X509v3 Basic Constraints: \n\
                CA:FALSE\n\
            X509v3 Subject Alternative Name: \n\
                DNS:localhost, DNS:mosquitto_certificates, DNS:hivemq_certificates, IP Address:127.0.0.1\n\
            X509v3 Subject Key Identifier: \n\
                E2:60:8B:FA:14:69:B8:5F:5A:A0:CC:FA:4F:2A:91:3E:1F:4B:9B:30\n\
    Signature Algorithm: sha1WithRSAEncryption\n\
         3b:2d:46:d6:29:f7:60:87:92:be:f2:56:13:af:02:ee:77:34:\n\
         d3:a8:d2:34:f8:99:0e:24:d7:02:aa:a9:82:50:0d:47:02:df:\n\
         5e:6f:f3:a5:c3:8a:45:d3:a1:fd:1d:64:c4:47:12:01:00:0e:\n\
         1e:24:eb:60:95:af:32:30:f5:a6:9c:d3:a2:a8:51:66:74:49:\n\
         21:31:1b:db:ec:aa:2b:72:51:48:cd:39:ce:0c:cf:cc:52:5d:\n\
         4c:e1:47:ba:49:94:9e:40:31:40:3b:5b:35:f1:aa:17:52:29:\n\
         2c:16:f2:d0:25:c7:ce:b6:6a:2b:22:9f:d6:12:e9:46:a7:e2:\n\
         0d:5b:4a:70:e3:15:11:8f:ea:d3:75:01:9c:36:6b:cb:19:6a:\n\
         5f:eb:e5:69:3d:95:16:a1:11:37:1b:14:06:fc:10:6d:74:32:\n\
         04:cd:39:44:1d:cb:6c:4a:b8:ca:83:33:fd:82:55:4c:0c:51:\n\
         5e:23:9e:c9:b0:0d:64:82:39:eb:93:0b:ae:b2:17:c3:fa:48:\n\
         ad:62:12:22:a9:db:81:48:dd:57:dc:d1:ec:1f:ff:31:54:37:\n\
         04:b1:aa:39:2d:8e:65:9c:a4:b9:a0:09:21:cf:cb:e9:88:25:\n\
         46:6a:9b:89:65:f2:3e:07:b1:d6:93:be:fc:68:20:31:1b:a4:\n\
         2e:c6:3f:b1\n\
-----BEGIN CERTIFICATE-----\n\
MIIDxjCCAq6gAwIBAgIBBDANBgkqhkiG9w0BAQUFADB1MQswCQYDVQQGEwJERTES\n\
MBAGA1UECAwJRnJhbmtvbmlhMRIwEAYDVQQHDAlOdXJlbWJlcmcxHTAbBgNVBAoM\n\
FFRlYW0gU2Vuc29ycyAmIFByb2JlMR8wHQYDVQQDDBZtb21vX2ludGVncmF0aW9u\n\
X3Rlc3RzMB4XDTE5MDQwMzE1MjQyNFoXDTI5MDMzMTE1MjQyNFowYTELMAkGA1UE\n\
BhMCREUxEjAQBgNVBAgMCUZyYW5rb25pYTEdMBsGA1UECgwUVGVhbSBTZW5zb3Jz\n\
ICYgUHJvYmUxHzAdBgNVBAMMFm1vbW9faW50ZWdyYXRpb25fdGVzdHMwggEiMA0G\n\
CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC2L0/4rNzumogQ9GlXRxno5tTI5wxk\n\
BN54Ep8Quna7XOvMnoJfNwj2f/6bQfRJ9U/+0qVwjMHsr3dv00yLtkKjlEPRuAYH\n\
eQaL+BIIY77wKkhx0oqCpyebQshcqafUa85QlcYAuxiJWtAKIHOZ+emfpFE27nI1\n\
dB7bIZpNX0EcM6KMsf3DyXrV8NFi1KTM8dOq0fUJNanYuigjL12C/KAhK05RClT3\n\
b0/Fa2HQRN0fuW835yyKDfTT0ogs3aTnCw/lGUVp72vwLCbFeSwfoe3Q05QUMBPp\n\
LO5MhzuJAz76zht7ECPYMU9qTHVdTKcmp+4TGPxMsspfRqnBWvPZJmLPAgMBAAGj\n\
dTBzMAkGA1UdEwQCMAAwRwYDVR0RBEAwPoIJbG9jYWxob3N0ghZtb3NxdWl0dG9f\n\
Y2VydGlmaWNhdGVzghNoaXZlbXFfY2VydGlmaWNhdGVzhwR/AAABMB0GA1UdDgQW\n\
BBTiYIv6FGm4X1qgzPpPKpE+H0ubMDANBgkqhkiG9w0BAQUFAAOCAQEAOy1G1in3\n\
YIeSvvJWE68C7nc006jSNPiZDiTXAqqpglANRwLfXm/zpcOKRdOh/R1kxEcSAQAO\n\
HiTrYJWvMjD1ppzToqhRZnRJITEb2+yqK3JRSM05zgzPzFJdTOFHukmUnkAxQDtb\n\
NfGqF1IpLBby0CXHzrZqKyKf1hLpRqfiDVtKcOMVEY/q03UBnDZryxlqX+vlaT2V\n\
FqERNxsUBvwQbXQyBM05RB3LbEq4yoMz/YJVTAxRXiOeybANZII565MLrrIXw/pI\n\
rWISIqnbgUjdV9zR7B//MVQ3BLGqOS2OZZykuaAJIc/L6YglRmqbiWXyPgex1pO+\n\
/GggMRukLsY/sQ==\n\
-----END CERTIFICATE-----\n\
";

const char* mqtt_client_key = \
"-----BEGIN RSA PRIVATE KEY-----\n\
Proc-Type: 4,ENCRYPTED\n\
DEK-Info: DES-EDE3-CBC,3B12A9718003322A\n\
\n\
u+v7+Qeh9WOn3ROPbC6iiPU7BFui588I5Ohi+bsoF+XvI5MK7Lg5Y6HR883EYYN4\n\
EMaHNdmfKJbosDRhMG1EljBNPXd37Fd+Idte720C29zZfLnNmAuVPOfTTs1MsTJY\n\
YjaqFwRRaCHCq/oOWiR4/wZ7NSXZofWMViixhtOSYYY5UZ/wKv0JqaFN8LvHAH4J\n\
158BAW8+lDrl70L+EZ7/mqf9x1F8dx4KdKI8ndTfm6pq8D8yZuygCRpfgOKCQhV9\n\
YztSVIPx81TkhePGO/jIZ3ogoxIZNYvPaLVL6BBhVHq72uVhsK7pVh2qBP1nxotr\n\
HP0EQZmWyHl/CCHDfm/X3kQPvgGKOsucvb3HANXN25+lqB0ofX3nWa0cHPfhLXax\n\
//GKnaL1RkGMQ1fse4NMsaUxq/B2r8p11vS/1NU5dQ8OA1hwugIsPji6YUSBCw8O\n\
mnDcJ3ot/noGTJwG9hCHsEi3Kwjn8Z7C2QfCQjlzYhbrexDnawvlfWadB73oh/2k\n\
ohzaj3lGxbiCFDRbY38LG/aSZIxNXYqYaxj8SZM2CBw18nzxG/8hoD+nNwmju66p\n\
p6Jmi+98QOuUyCKsJ2Jmcu9Nj20k9iM9QH53TAB/+Hv1xePQ66NoUyorShT5ETuM\n\
4oQMLaBR/ZLTc2VaRyekKCOYoX+NM9BY/h5ekuo5+xfbWEfH4r+cRaSXdB2J9bbj\n\
Ieo8UBRk+TLsCeaqq0+lyTyFYLUNH7txJpr434c6ojS/L9MYlhtGHhJDd+QGd2Up\n\
FHPnfoDuAx+Gj7Dc3D5FPM9wPT8q4+fhzrTfIgAFwp+GtiZ+uyVQBj782fZq7Jxg\n\
Uum9X4bY4FVSreKwuu7qKgQa13ByE7OsDKCkepTQXyxSFJV2Zp0J2g4M1FaLIMjI\n\
3vhTWOnq9yTP/qPbJVPdeSBX+mAkJJ9GqvF7YYDBOQ/v7VhhyiQ4h2maxaBPZolW\n\
CMO35R4D0vVcvUbVlFtQSThLXUR6o6pp7CLgequAsZYOdqvklXZIMhoPHYsy/Fb2\n\
kAloPKNxhsopxBYUw0sTpuPdaAEg1/EeUqhJ9kg4NJfgokp8JFy1t8jJYWo56GLq\n\
wCDqjf9aQQBhtL6jEXMMTnfZDPjyRxJ0zUVJGpmbhE1KU7JuU0Q1+nBfGnTxT70C\n\
DqeKgnIuFsGlU/idAgqlqlPjH/ekCn1NQ82yILB7TixH1qpHv32AiS3lCrssvTw9\n\
wCEialiHYRkpyS3v1GmLlFWV6jQLaiNbRsqwJqw0cidVfAP8roZYOnbxruCzRgxX\n\
B4UbrHOqRFJTop8vo3X4kpxrQYcQIlxoJHdNdJimEiDE4GWTVR/5PckDUJL/bV32\n\
rSa7M7gua3imytQjrjUQwO0oiWAw75EFMtRN/f7jrueKZzYxtBPtBM10Mc03zgZZ\n\
zPRAwf3wD5pss9VQqslD5Je0cE4zjkcL8iABZgNERBEWxvXTCnrJEnWli1YIecI8\n\
zlRIrGlIgoKXDX4Kvs+uOzbCw+LwQ15G40Lb7174Ihqs+0RON0mSqFVVvkhRdf7n\n\
VSJk76lO+VTyn37PP4hM5gmLvyH7ttLPJ3JYFHPR6hZtvBtXBc4nBg==\n\
-----END RSA PRIVATE KEY-----\n\
";

int main(int argc, char* argv[]) {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    MQTTClient_SSLOptions ssl_options = MQTTClient_SSLOptions_initializer;
    //ssl_options.trustStore = "./certificate_authority.crt";
    //ssl_options.trustStore = ca_cert;

    //ssl_options.keyStore = "./mqtt_client.crt";
    ssl_options.keyStore = mqtt_client_crt;
    //ssl_options.privateKey = "./mqtt_client.key";
    ssl_options.privateKey = mqtt_client_key;
    ssl_options.privateKeyPassword = "secret";

    ssl_options.enableServerCertAuth = 0;
    ssl_options.verify = 0;  // post connection -> host name matches?

    conn_opts.ssl = &ssl_options;


    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(EXIT_FAILURE);
    }
    pubmsg.payload = PAYLOAD;
    pubmsg.payloadlen = (int)strlen(PAYLOAD);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
