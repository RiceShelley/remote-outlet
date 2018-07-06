#include <ESP8266WiFi.h>

#define R_OUTLET D1
#define L_OUTLET D2
#define ON LOW
#define OFF HIGH

const char *ssid = "SSID";
const char *pass = "PSK";

WiFiServer server(21);

void setup() {
  Serial.begin(9600);
  pinMode(R_OUTLET, OUTPUT);
  pinMode(L_OUTLET, OUTPUT);
  digitalWrite(R_OUTLET, OFF);
  digitalWrite(L_OUTLET, OFF);

  // Connect to wifi
  IPAddress ip(192, 168, 1, 27);
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  WiFi.config(ip, gateway, subnet);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Failed to connect to wifi");
  }

  Serial.print("Connected to "); Serial.println(ssid);
  Serial.print("IP Address: "); Serial.println(WiFi.localIP());
  server.begin();
}

String readFromSock(WiFiClient sClient) {
  String str = "";
  int accumDelay = 0;
  while (true) {
    if (sClient.available() > 0) {
      Serial.println("reading from client");
      for (int i = 0; i < 100; i++) {
        char c = sClient.read();
        if (c == 0) {
          break;
        }
        str += c;
      }
      return str;
    } else {
      delay(50);
      accumDelay += 50;
      if (accumDelay > 10 * 1000) {
        return "NULL";
      }
    }
  }
}

void loop() {
  WiFiClient sClient = server.available();
  if (sClient) {
    sClient.flush();
    sClient.println("REMOTE E-SOCKET SERV");
    String str = readFromSock(sClient);
    if (str.startsWith("RON")) {
      digitalWrite(R_OUTLET, ON);
      Serial.println("right on");
    } else if (str.startsWith("LON")) {
      digitalWrite(L_OUTLET, ON);
      Serial.println("left on");
    } else if (str.startsWith("ROFF")) {
      digitalWrite(R_OUTLET, OFF);
      Serial.println("right off");
    } else if (str.startsWith("LOFF")) {
      digitalWrite(L_OUTLET, OFF);
      Serial.println("left off");
    } else {
      Serial.println("ERROR: bad cmd");
    }
  }
  delay(500);
}
