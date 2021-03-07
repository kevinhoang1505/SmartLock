#include <ESP8266WiFi.h>

#include <EEPROM.h>
const int addr = 0;

#include <Servo.h>
#define servo_pin 14
Servo servo;

const char* ssid = "SSID";
const char* pass = "********";
WiFiServer server(80);
void setup() {
  Serial.begin(115200);
  EEPROM.begin(512);
  servo.attach(servo_pin);
  WiFi.begin(ssid,pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected"); 
  server.begin();  // Starts the Server
  Serial.println("Server started");
  Serial.print("IP Address of network: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  Serial.println("Waiting for new client");
  while(!client.available())
  {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if(request.indexOf("/LOCK") != -1)
  {
    servo.write(120);
    EEPROM.write(addr,LOW);
  }
  if(request.indexOf("/UNLOCK") != -1)
  {
    servo.write(240);
    EEPROM.write(addr,HIGH);
  }

/*------------------HTML Page Creation---------------------*/

  
  client.println("HTTP/1.1 200 OK"); // standalone web server with an ESP8266
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");

  client.print("Status: ");
 
  if(EEPROM.read(0) == HIGH)
  {
    client.print("UNLOCK");
  }
  else
  {
    client.print("LOCK");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LOCK\"\"><button>LOCK</button></a>");
  client.println("<a href=\"/UNLOCK\"\"><button>UNLOCK</button></a><br />");
  client.println("</html>");

  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
