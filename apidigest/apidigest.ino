/*
 
 This sketch connects to a IPdevice with digest authentication
 using an Arduino WIZnet Ethernet shield, and send api to it.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13

 created 14 Nov 2023
 by Giuseppe Turturro
 modified 1 Mar 2024
 by Giuseppe Turturro, based on work by Adrian McEwen

*/

#include <SPI.h>
#include <Ethernet.h>
#include "MD5.h"
#include "config.h"

IPAddress ip(192, 168, 1, 100); // IP predefinito
byte mac[] = {0xA8, 0x61, 0x0A, 0xAF, 0x1B, 0x13}; // MAC address eth shild
IPAddress dns(4, 4, 4, 4);
IPAddress gw(192, 168, 1, 1);
IPAddress sn(255, 255, 255, 0);
IPAddress server(192,168,1,208);    // Device ip address

// Initialize the Ethernet client library
EthernetClient client;
// Initialize the cryptography library
MD5 md5;

void setup() {
  Serial.begin(115200);
  // You can use Ethernet.init(pin) to configure the CS pin
  Ethernet.init(10);  // Most Arduino shields
  //Ethernet.init(5);   // MKR ETH Shield
  //Ethernet.init(0);   // Teensy 2.0
  //Ethernet.init(20);  // Teensy++ 2.0
  //Ethernet.init(15);  // ESP8266 with Adafruit FeatherWing Ethernet
  //Ethernet.init(33);  // ESP32 with Adafruit FeatherWing Ethernet

  Ethernet.begin(mac, ip, dns, gw, sn);
  Serial.println(Ethernet.localIP());
  delay(1000);
}

// Function to perform a GET request and return the response
String getHttpRequest(EthernetClient& client, const String& uri) {
  String response = ""; // Server response

  // Connect to the server
  if (client.connect(server, 80)) {
    // Make a GET request

    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    String request = "GET " + uri + " HTTP/1.1";
    // Send the request to the server
    client.print(request);
    client.println("Connection: close");
    client.println();

    // Wait for the response from the server
    while (client.connected() || client.available()) {
      if (client.available()) {
        char c = client.read();
        response += c;
      }
    }

    // Close the connection
    client.stop();
  } else {
    // If the connection fails
    response = "Connessione al server fallita";
  }

  return response;
}

String postHttpRequest(EthernetClient& client, const String& uri, const String& content) {
  String response = ""; // Server response

  // Connect to the server
  if (client.connect(server, 80)) {
    // Make a POST request
    client.println("GET " + uri + " HTTP/1.1");
    client.println("Host: " + server.toString());
    client.println("Authorization: " + content);
    client.println("Connection: close");
    client.println();

    // Wait for the response from the server
    while (client.connected() || client.available()) {
      if (client.available()) {
        char c = client.read();
        response += c;
      }
    }

    // Close the connection
    client.stop();
  } else {
    // If the connection fails
    response = "Connessione al server fallita";
  }

  return response;
}

// Make a random string for the creation of the NConce
String getCNonce(const int len) {
  static const char alphanum[] =
    "0123456789"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz";
  String s = "";

  for (int i = 0; i < len; ++i) {
    s += alphanum[rand() % (sizeof(alphanum) - 1)];
  }

  return s;
}

String getDigestAuth(String& responseString, const String& username, const String& password, const String& uri, unsigned int counter) {
  int realmStart = responseString.indexOf("realm=\"") + 7;
  int realmEnd = responseString.indexOf("\"", realmStart);
  int nonceStart = responseString.indexOf("nonce=\"") + 7;
  int nonceEnd = responseString.indexOf("\"", nonceStart);

  // Extract substrings
  String realm = responseString.substring(realmStart, realmEnd);
  String nonce = responseString.substring(nonceStart, nonceEnd);
  String cNonce = getCNonce(8);
  char nc[9];
  snprintf(nc, sizeof(nc), "%08x", counter);

  String concatenatedString = username + ":" + realm + ":" + password;

  // Convert the concatenated string to a character array
  char concatenatedCharArray[concatenatedString.length() + 1];
  concatenatedString.toCharArray(concatenatedCharArray, sizeof(concatenatedCharArray));

  // Calculate the MD5 hash
  unsigned char *hash = MD5::make_hash(concatenatedCharArray);

  // Generates the hexadecimal representation of the hash
  char *md5str = MD5::make_digest(hash, 16);

  String concatenatedString2 = "GET:" + uri;

  // Convert the concatenated string to a character array
  char concatenatedCharArray2[concatenatedString2.length() + 1];
  concatenatedString2.toCharArray(concatenatedCharArray2, sizeof(concatenatedCharArray2));

  // Calculate the MD5 hash
  unsigned char *hash2 = MD5::make_hash(concatenatedCharArray2);

  // Generates the hexadecimal representation of the hash
  char *md5str2 = MD5::make_digest(hash2, 16);

  // Concatenare i valori con i due punti
  String concatenatedString3 = String(md5str) + ":" + nonce + ":" + String(nc) + ":" + cNonce + ":auth:" + String(md5str2);

  // Convert the concatenated string to a character array
  char concatenatedCharArray3[concatenatedString3.length() + 1];
  concatenatedString3.toCharArray(concatenatedCharArray3, sizeof(concatenatedCharArray3));

  // Calculate the MD5 hash
  unsigned char *hash3 = MD5::make_hash(concatenatedCharArray3);

  // Generates the hexadecimal representation of the hash
  char *md5str3 = MD5::make_digest(hash3, 16);

  // Print the hash to the serial port
  String response = String(md5str3);

  // Free your memory
  free(hash);
  free(md5str);
  free(hash2);
  free(md5str2);
  free(hash3);
  free(md5str3);

  String authorization = "Digest username=\"" + username + "\", realm=\"" + realm + "\", nonce=\"" + nonce +
                         "\", uri=\"" + uri + "\", algorithm=\"MD5\", qop=auth, nc=" + String(nc) + ", cnonce=\"" + cNonce + "\", response=\"" + response + "\"";
  // Serial.println("Authorization: " + authorization);

  return authorization;
}

// Check that the api was correctly returned and if so manipulate the output to display only it
String getContentAfterContentLength(const String& httpResponse) {
  int pos = httpResponse.indexOf('\n');
  if (pos == -1) {
    // If there are no lines, return the empty string
    return "";
  }

  // Extract the first row
  String firstLine = httpResponse.substring(0, pos);

  // Check if the first line contains "200"
  if (firstLine.indexOf("200") == -1) {
    // If it does not contain "200", print only the first line
    Serial.println("Errore nella risposta: " + firstLine);
    return "";
  }

  // Find the position of the double carriage return that indicates the start of the content
  int doubleNewlineStart = httpResponse.indexOf("\r\n\r\n");
  if (doubleNewlineStart != -1) {
    // Extract content after double carriage return
    return httpResponse.substring(doubleNewlineStart + 4);
  }

  // If the double carriage return is not found, return an empty string
  return "";
}

void loop() {
  // send the first request to obtain the digest credential according to the RFC 2617 newer Digest
  String httpResponse = getHttpRequest(client, uri);
  /* 
  Print the server auth response on the serial monitor
  Serial.println("Server response 1:");
  Serial.println(httpResponse);
  */
  // Generate the newest request for the second path of the digest auth
  String authorization = getDigestAuth(httpResponse, String(username), String(password), String(uri), 1);
  String httpResponse2 = postHttpRequest(client, uri, authorization);
  /* 
  Print the server response on the serial monitor
  Serial.println("Server response 2:");
  Serial.println(httpResponse2);
  */
  String content = getContentAfterContentLength(httpResponse2);
  Serial.println("Api response for \"" + String(uri) + "\"");
  Serial.println(content);
  // Calculate time response
  if (!client.connected()) {
    // Fai nulla per sempre:
    while (true) {
      delay(1);
    }
  }
}

