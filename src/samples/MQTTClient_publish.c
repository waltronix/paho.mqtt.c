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
MIIDZjCCAk4CCQCQ6ngvfjINsjANBgkqhkiG9w0BAQUFADB1MQswCQYDVQQGEwJE\n\
RTESMBAGA1UECAwJRnJhbmtvbmlhMRIwEAYDVQQHDAlOdXJlbWJlcmcxHTAbBgNV\n\
BAoMFFRlYW0gU2Vuc29ycyAmIFByb2JlMR8wHQYDVQQDDBZtb21vX2ludGVncmF0\n\
aW9uX3Rlc3RzMB4XDTIwMDExNjE1MDY1NFoXDTIwMDIxNTE1MDY1NFowdTELMAkG\n\
A1UEBhMCREUxEjAQBgNVBAgMCUZyYW5rb25pYTESMBAGA1UEBwwJTnVyZW1iZXJn\n\
MR0wGwYDVQQKDBRUZWFtIFNlbnNvcnMgJiBQcm9iZTEfMB0GA1UEAwwWbW9tb19p\n\
bnRlZ3JhdGlvbl90ZXN0czCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEB\n\
ANeS84+PBVU8MpLMDbzDizhFT/LMfNK1zr0GZ1DRYmin3ZyrWb+eMBehWOcO7pEi\n\
5d29JYcPzZFRicDzY79loKSgBTCSqoSGzpwM+btGg6a4Mj1n3o0L0oWkS917tjss\n\
YnwclWADh4zJcnZBme2c83aXbnpjSxvg0cEWaVsslJmDZDkg9l0Fg2q4O3xoBANI\n\
QDcox1CZlqFSV3lZyOcuVul8MT5KNW2/SUztOhhEBd+gRBQ0UiqDqqE6RkBEBPm0\n\
x762hHw1UcDuWd7/WAq4iX2oHILUQAouRQiVuQedH5pU0gf/OJNb1W0FiPzzC0vp\n\
I7+DJ5/GNHPHG/tEXEJn4cUCAwEAATANBgkqhkiG9w0BAQUFAAOCAQEAh1fBYMKU\n\
uTJpMNkxCxw2d7IZ8NczdLEZFkil32L3Ko+paJ7l/0FgjVnrrlExXDi5fgNiSseM\n\
bLiZJ+SvFWzE7N/J6IGt1U+NlqWoYuWHEQsVWjapMOVkO5xlOd/NbUh7iB4KmffZ\n\
u8zGsilgUtFAzSFW2qcuCbd80hEu8ZCHAsBtyUHvYelx9btVIJzKEAixvsWy9x99\n\
rKCRZqRnJ7f9xAsSvuYXbxNe17jpJ7qtFUNKKw7I7a6nXdpMkI6iohL61PESnIDl\n\
xIEkA6gO1NoGk5tqLly5XyiB5wgao9s1qoI7NiV43e4Vy5c+Fl9Stx8twaYPVToP\n\
cZjgeCbMStzIwQ==\n\
-----END CERTIFICATE-----\n\
";

const char* mqtt_client_crt = \
"Certificate:\n\
    Data:\n\
        Version: 3 (0x2)\n\
        Serial Number: 2 (0x2)\n\
    Signature Algorithm: sha1WithRSAEncryption\n\
        Issuer: C=DE, ST=Frankonia, L=Nuremberg, O=Team Sensors & Probe, CN=momo_integration_tests\n\
        Validity\n\
            Not Before: Jan 16 15:06:56 2020 GMT\n\
            Not After : Jan 13 15:06:56 2030 GMT\n\
        Subject: C=DE, ST=Frankonia, O=Team Sensors & Probe, CN=mqtt_client\n\
        Subject Public Key Info:\n\
            Public Key Algorithm: rsaEncryption\n\
                Public-Key: (2048 bit)\n\
                Modulus:\n\
                    00:ef:c0:a7:4a:9b:8e:17:31:96:ed:c1:3f:9e:df:\n\
                    09:27:16:66:6b:78:42:8c:bc:d0:c6:bf:27:f2:af:\n\
                    35:8b:4a:0e:bd:26:cf:76:1d:6a:63:8e:5e:ad:a9:\n\
                    f9:f8:92:02:36:fb:57:8d:b6:95:b1:e5:d3:55:ce:\n\
                    22:b0:6d:62:73:07:3f:ec:c8:46:fb:e1:b4:42:a4:\n\
                    f2:68:35:3f:22:67:5c:29:53:59:5c:b7:4a:b9:8c:\n\
                    e6:0c:bf:73:1c:b6:75:ea:a5:a8:31:14:ae:24:15:\n\
                    85:61:bc:44:1b:90:94:72:20:23:c1:b6:db:4c:17:\n\
                    e6:c7:2f:8f:52:bb:b0:69:66:08:02:56:37:1d:01:\n\
                    aa:bf:66:da:d5:81:90:ed:da:bf:89:68:6e:87:eb:\n\
                    61:19:ec:11:40:05:0b:2c:7e:d1:c7:c0:84:19:ca:\n\
                    c0:b6:3c:5b:ba:48:97:40:c3:cf:17:f6:18:b5:28:\n\
                    a7:cd:29:e6:39:f6:a3:bd:aa:96:49:b0:a0:19:ae:\n\
                    fc:fa:cd:02:24:57:8b:10:9b:d4:61:0a:6f:98:08:\n\
                    79:a6:1e:48:53:26:fd:da:8e:cd:1a:f7:79:3b:4d:\n\
                    76:f9:ae:38:8d:92:2a:0a:91:af:3b:9d:70:1a:f6:\n\
                    a4:0b:ad:73:9a:bc:21:1b:d8:0a:3f:e4:6b:ed:ce:\n\
                    28:59\n\
                Exponent: 65537 (0x10001)\n\
        X509v3 extensions:\n\
            X509v3 Basic Constraints: \n\
                CA:FALSE\n\
            X509v3 Subject Alternative Name: \n\
                DNS:localhost, DNS:mosquitto_certificates, DNS:hivemq_certificates, IP Address:127.0.0.1\n\
            X509v3 Subject Key Identifier: \n\
                CC:D8:14:21:1D:14:C1:10:6C:29:18:37:3A:7D:2E:AC:7F:C5:17:4F\n\
    Signature Algorithm: sha1WithRSAEncryption\n\
         77:00:08:a6:09:d0:81:be:64:b7:46:2d:60:29:11:25:08:ee:\n\
         d4:8b:3f:9c:79:42:d2:32:b0:0a:99:8c:a1:45:85:b1:3d:eb:\n\
         83:cf:19:f8:70:75:97:89:b6:36:cd:ac:54:20:e7:c8:32:a4:\n\
         76:27:76:c8:e9:2f:3f:99:30:ae:9e:44:61:e6:e1:8d:88:6c:\n\
         38:f2:ef:4c:39:e3:3d:69:a8:f0:5a:a8:26:a9:d3:ef:69:55:\n\
         63:89:0f:25:a3:7d:32:dc:fb:bd:f7:3f:74:c4:ee:6d:1a:d4:\n\
         38:68:50:dd:9b:c8:1a:c4:bd:d8:1e:e0:20:5d:7f:6e:05:93:\n\
         79:de:1f:20:75:3f:9c:20:3f:ca:da:f4:8d:f3:11:8d:b4:a5:\n\
         b0:f1:5a:58:85:e4:1a:4f:9a:65:9f:16:94:58:8f:22:55:32:\n\
         b6:51:83:a3:d7:c7:e6:0d:b4:1a:cc:9f:6d:50:aa:87:0f:ef:\n\
         93:14:93:e3:16:a4:47:98:db:79:9e:c8:78:d4:04:03:30:51:\n\
         ac:e5:fd:0e:a6:b9:04:5e:11:31:16:56:e1:27:e9:c3:f8:c7:\n\
         96:fa:27:65:4e:e7:e8:2d:6e:ab:fa:b4:93:ac:02:05:c1:16:\n\
         7c:b7:2f:7e:44:52:48:80:b6:5e:7d:ce:82:4f:23:38:51:e1:\n\
         21:12:e2:45\n\
-----BEGIN CERTIFICATE-----\n\
MIIDuzCCAqOgAwIBAgIBAjANBgkqhkiG9w0BAQUFADB1MQswCQYDVQQGEwJERTES\n\
MBAGA1UECAwJRnJhbmtvbmlhMRIwEAYDVQQHDAlOdXJlbWJlcmcxHTAbBgNVBAoM\n\
FFRlYW0gU2Vuc29ycyAmIFByb2JlMR8wHQYDVQQDDBZtb21vX2ludGVncmF0aW9u\n\
X3Rlc3RzMB4XDTIwMDExNjE1MDY1NloXDTMwMDExMzE1MDY1NlowVjELMAkGA1UE\n\
BhMCREUxEjAQBgNVBAgMCUZyYW5rb25pYTEdMBsGA1UECgwUVGVhbSBTZW5zb3Jz\n\
ICYgUHJvYmUxFDASBgNVBAMMC21xdHRfY2xpZW50MIIBIjANBgkqhkiG9w0BAQEF\n\
AAOCAQ8AMIIBCgKCAQEA78CnSpuOFzGW7cE/nt8JJxZma3hCjLzQxr8n8q81i0oO\n\
vSbPdh1qY45eran5+JICNvtXjbaVseXTVc4isG1icwc/7MhG++G0QqTyaDU/Imdc\n\
KVNZXLdKuYzmDL9zHLZ16qWoMRSuJBWFYbxEG5CUciAjwbbbTBfmxy+PUruwaWYI\n\
AlY3HQGqv2ba1YGQ7dq/iWhuh+thGewRQAULLH7Rx8CEGcrAtjxbukiXQMPPF/YY\n\
tSinzSnmOfajvaqWSbCgGa78+s0CJFeLEJvUYQpvmAh5ph5IUyb92o7NGvd5O012\n\
+a44jZIqCpGvO51wGvakC61zmrwhG9gKP+Rr7c4oWQIDAQABo3UwczAJBgNVHRME\n\
AjAAMEcGA1UdEQRAMD6CCWxvY2FsaG9zdIIWbW9zcXVpdHRvX2NlcnRpZmljYXRl\n\
c4ITaGl2ZW1xX2NlcnRpZmljYXRlc4cEfwAAATAdBgNVHQ4EFgQUzNgUIR0UwRBs\n\
KRg3On0urH/FF08wDQYJKoZIhvcNAQEFBQADggEBAHcACKYJ0IG+ZLdGLWApESUI\n\
7tSLP5x5QtIysAqZjKFFhbE964PPGfhwdZeJtjbNrFQg58gypHYndsjpLz+ZMK6e\n\
RGHm4Y2IbDjy70w54z1pqPBaqCap0+9pVWOJDyWjfTLc+733P3TE7m0a1DhoUN2b\n\
yBrEvdge4CBdf24Fk3neHyB1P5wgP8ra9I3zEY20pbDxWliF5BpPmmWfFpRYjyJV\n\
MrZRg6PXx+YNtBrMn21QqocP75MUk+MWpEeY23meyHjUBAMwUazl/Q6muQReETEW\n\
VuEn6cP4x5b6J2VO5+gtbqv6tJOsAgXBFny3L35EUkiAtl59zoJPIzhR4SES4kU=\n\
-----END CERTIFICATE-----\n\
";

const char* mqtt_client_key = \
"-----BEGIN RSA PRIVATE KEY-----\n\
Proc-Type: 4,ENCRYPTED\n\
DEK-Info: DES-EDE3-CBC,C63B42FAAF34EDB8\n\
\n\
XlhPwGfz/1YepjWOq4Zw4faFaRmBvV4MrW/udc4fTM+LlsJui5fdoDw+aK4Rnh+w\n\
GkcqBmj76GHPZYlP2vs0P53yxIZ14dmpd2dK8s93N5VeO6ugKwSuiLnfBBBjcgOu\n\
v8hfmi8YsbeZ5FSCUhdSxhKwCZP9bJzGIQcv0WwX5wuZxskN2UMawC11cSAxkNNz\n\
MgcVwiValIllB0EMgjtHE8Z9JLzvWg7FJop3zuCNeSjB6sYtrIwIB2+JEcRrBSV/\n\
Qq6/6QZRNZPtqLfHgQXJV4/xIbc2srMZRUOpIfxnj0m+bY0K0VEEKb3NhhuWoq05\n\
5hZXfMFKlbHBllcq0hr1zDgld0Z5jQIfVXKGu1J6kDK4IU9TlL5Yp/GOkM7XTR//\n\
gxIN5atvWvyBqD7HDM6xuw1RdzwaQ1i9TmhBW5vyi/w7r/24XzQC/iV0zf8YD2RT\n\
XC/DlZ4piulbTIFkaJzPRtuPYPBqwf+fq3MDQvm0CngEmjJMdQa+hQDAM1R8Z0JM\n\
LdT9i7E1nwr/5dzWGnAie405vgQnyYCmRoCGqFQpLHNmp8OTiXjQQ3comuBLEtr5\n\
Nef61jaGe1mvYnGD+M5oFxKEJGVwSqoWRij/ITVYPnY8lzXXFEA4RdWBjCst7fdS\n\
KvKCDv0BYmaOTcfIdvsOGhUUY4MDUidYDWyzfnZouw7WaOsChOPunQ2OnAOzBScV\n\
8rq2WMrJqxu7iZKbsvkByq134CEaQPmhWNKhFWGo2S0w3GB9yncBUQ+tjcH9mHd2\n\
/506zU3ioI68rIVmCesQ3r2YQpHwjHq4BOl4Tq27Kzo4ow+ydS8gYaTikZJqVze2\n\
A8EC09nkSJQickQrDy8GJS8J+WToZbwGb10Dwe2ImQUiFNWrJ02fU1PrN9bAc+eT\n\
1QBnI4lC1R45m5pf0FC8Zvystu+5ltq5NT394+Z5JxuhZsBoB7gQk8GrwYWXSf4C\n\
SjvyzKvwnHLR+5ssOit57Thi2eUhhC7i0KRGAnxpDckgCUx+EnWXXpApY3UOgyZD\n\
jTXkP4UnRbtvvUtRY99XuNS7jvZIQ853N0R3iJU4bMRyECLNI+TNk2tJstXkOVAz\n\
1io60z5uVk/AVsR0NHBTC308fgFrXG2ZrsekTVmSKexhYe8Z97UHHNuvhK6X9OI4\n\
pOZnaqz84LcF1XXme2ZBPFIDp3JekkhOLgeC/M25MCJ8IIM0EEr0OuZTb8KWQ7kx\n\
aYg24PnTBOrMap6F5g9dYcO+JeaKAfSrQDnAw8rDxgcuiq1wReO+tPKx3JSvsz+q\n\
pRB5+wtTYznM4SnwZb6CropHzOMUibiMwuXAu6lhy9kSvnHzpXoDWQ74dTW1nzjI\n\
hMtWePpOMAr9vHU73ix/qe9whTUt8tQ5oUTIMN+ltjt6T+VJCcf+U8Pt3wYYJCGh\n\
d0cGARomt4sIQipLafc2TT+qqn+alNje2pdMLVs3ySMcB1pggYk1DYxKiZGg/C1z\n\
EV494uU4vmeKIkiWWkrPx3gncPwZzvT4hVIzmcr+r2jcIX9zHPLxxkdbxdfiediF\n\
pUkdvlU15Yxg5m3EQ6AGLTDpQ01/siMpgTSeonfVBpSBuIWwS/BDnw==\n\
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
    ssl_options.trustStore = ca_cert;

    //ssl_options.keyStore = "./mqtt_client.crt";
    ssl_options.keyStore = mqtt_client_crt;
    //ssl_options.privateKey = "./mqtt_client.key";
    ssl_options.privateKey = mqtt_client_key;
    ssl_options.privateKeyPassword = "secret";

    ssl_options.enableServerCertAuth = 1;
    ssl_options.verify = 1;  // post connection -> host name matches?

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
