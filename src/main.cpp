/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

#include <Arduino.h>

// Load Wi-Fi library
#include <ESP8266WiFi.h>

const char* ssid     = "dlink";
const char* password = "8karaktere";

const char* PARAM_INPUT = "input";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String d7State = "off";
String d6State = "off";

// Assign output variables to GPIO pins
const int d6 = 12;
const int d7 = 13;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;


void setup() {
  Serial.begin(115200);
  // Initialize the output variables as outputs
  pinMode(d7, OUTPUT);
  pinMode(d6, OUTPUT);
  // Set outputs to LOW
  digitalWrite(d7, LOW);
  digitalWrite(d6, LOW);
  Serial.println();
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {  
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,    
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then        
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // turns the GPIOs on and off
            if (header.indexOf("GET /13/on") >= 0) {
              Serial.println("GPIO 13 on");
              d7State = "on";
              digitalWrite(d7, HIGH);
            } else if (header.indexOf("GET /13/off") >= 0) {
              Serial.println("GPIO 13 off");
              d7State = "off";
              digitalWrite(d7, LOW);
            } else if (header.indexOf("GET /12/on") >= 0) {
              Serial.println("GPIO 12 on");
              d6State = "on";
              digitalWrite(d6, HIGH);
            } else if (header.indexOf("GET /12/off") >= 0) {
              Serial.println("GPIO 12 off");
              d6State = "off";
              digitalWrite(d6, LOW);
            }

            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>ESP8266 Web Server</h1>");

            client.println("<input type='text' name='inputtext'>");

            // Display current state, and ON/OFF buttons for GPIO 13
            client.println("<p>GPIO 13 - State " + d7State + "</p>");
            // If the d7State is off, it displays the ON button
            if (d7State == "off") {
              client.println("<p><a href=\"/13/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/13/off\"><button class=\"button button2\">OFF</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 12
            client.println("<p>GPIO 12 - State " + d6State + "</p>");
            // If the d6State is off, it displays the ON button
            if (d6State == "off") {
              client.println("<p><a href=\"/12/on\"><button class=\"button\">ON</button></a></p>");
            } else {
              client.println("<p><a href=\"/12/off\"><button class=\"button button2\">OFF</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

//space between words
void space() {
  delay (1200);
}
//the dot this code make the d6 on for 300 than off for 300
void dot() {
  digitalWrite(d6,HIGH);
  delay (300);
  digitalWrite(d6,LOW);
  delay (300);  
}
//the dash this code make the d6 on for 900 than off for 300
void dash() {
  digitalWrite(d6,HIGH);
  delay (900);
  digitalWrite(d6,LOW);
  delay (300);  
}
//space between letters
void shortspace() {
  delay(600);
}

//fonctions for the letters and the numbers
void lA () {
  dot();
  dash();
  shortspace(); 
}//letter A in morse code!
void lB () {
  dash();
  dot();
  dot();
  dot();
  shortspace(); 
}//same for B
void lC () {
  dash();
  dot();
  dash();
  dot();
  shortspace();
}
void lD () {
  dash();
  dot();
  dot();
  shortspace();
}
void lE () {
  dot();
  shortspace(); 
}
void lF () {
  dot();
  dot();
  dash();
  dot();
  shortspace();
}
void lG () {
  dash();
  dash();
  dot();
  shortspace(); 
}
void lH () {
  dot();
  dot();
  dot();
  dot();
  shortspace();
}
void lI () {
  dot();
  dot();
  shortspace();
}
void lJ () {
  dot();
  dash();
  dash();
  dash();
  shortspace();
}
void lK () {
  dash();
  dot();
  dash();
  shortspace();
}
void lL () {
  dot();
  dash();
  dot();
  dot();
  shortspace(); 
}
void lM () {
  dash();
  dash();
  shortspace();
}
void lN () {
  dash();
  dot();
  shortspace();
}
void lO () {
  dash();
  dash();
  dash();
  shortspace();
}
void lP () {
  dot();
  dash();
  dash();
  dot();
  shortspace();
}
void lQ () {
  dash();
  dash();
  dot();
  dash();
  shortspace();
}
void lR () {
  dot();
  dash();
  dot();
  shortspace();
}
void lS () {
  dot();
  dot();
  dot();
  shortspace();
}
void lT () {
  dash();
  shortspace();
  
}
void lU () {
  dot();
  dot();
  dash();
  shortspace();
}
void lV () {
  dot();
  dot();
  dot();
  dash();
  shortspace(); 
}
void lW () {
  dot();
  dash();
  dash();
  shortspace(); 
}
void lX () {
  dash();
  dot();
  dot();
  dash();
  shortspace(); 
}
void lY () {
  dash();
  dot();
  dash();
  dash();
  shortspace(); 
}
void lZ () {
  dash();
  dash();
  dot();
  dot();
  shortspace(); 
}
//number 1 in morse code
void n1 () {
  dot();
  dash();
  dash();
  dash();
  dash();
  shortspace(); 
}
void n2 () {
  dot();
  dot();
  dash();
  dash();
  dash();
  shortspace(); 
}
void n3 () {
  dot();
  dot();
  dot();
  dash();
  dash();
  shortspace(); 
}
void n4 () {
  dot();
  dot();
  dot();
  dot();
  dash();
  shortspace(); 
}
void n5 () {
  dot();
  dot();
  dot();
  dot();
  dot();
  shortspace(); 
}
void n6 () {
  dash();
  dot();
  dot();
  dot();
  dot();
  shortspace(); 
}
void n7 () {
  dash();
  dash();
  dot();
  dot();
  dot();
  shortspace();
}
void n8 () {
  dash();
  dash();
  dash();
  dot();
  dot();
  shortspace(); 
}
void n9 () {
  dash();
  dash();
  dash();
  dash();
  dot();
  shortspace();
}
void n0 () {
  dash();
  dash();
  dash();
  dash();
  dash();
  shortspace(); 
}
