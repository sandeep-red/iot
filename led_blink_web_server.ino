#include <WiFi.h>

const char* ssid     = "";
const char* password = "";

WiFiServer server(80);

void setup()
{
    Serial.begin(115200);
    pinMode(27, OUTPUT);      // set the LED pin mode

    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    server.begin();

}

int value = 0;
String state="on";
String url="/H";
void loop(){
 WiFiClient client = server.available();   // listen for incoming clients

  if (client) {                             // if you get a client,
    Serial.println("New Client.");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
//            client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
//            client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");
               client.print("<html><div style=\"text-align:center\"><h1>LED state</h1>");
                client.print("<button><a href=\""+url+"\">"+state+"</a></button>");
                //client.print("<p style=\"text-align:center\">led is "+state+"</p>");
//               if(state.equals("off")){
//                client.print("<button><a href=\"/H\">on</a></button>");
//                client.print("<p style=\"text-align:center\">led is off</p>");
//               }
//               else if(state.equals("on")){
//                client.print("<button><a href=\"/L\">off</a></button>");
//                client.print("<p style=\"text-align:center\">led is on</p>");
//               }
               client.print("</div></html>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.equals("GET /H")) {
          digitalWrite(27, HIGH); 
          state="off";
          url="/L";
          
          // GET /H turns the LED on
        }
        if (currentLine.equals("GET /L")) {
          digitalWrite(27, LOW);                // GET /L turns the LED off
          state="on";
          url="/H";
        }
      }
      
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
