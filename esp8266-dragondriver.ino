/*
 *
 * Author: John Glatts
 * Brief: Nodemcu webserver with stepper motor controls and data, testing to see if this jawnt works
 * Date: 3/11/2019
 *
 */
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#define relay_pin 16
#define pix_pin 5
#define limit_pin 4
#define NUM_PIX 12


// Strings for WiFi
const char *ssid = "";
const char *password = "";


// Create a new Server and Neopixel instance
ESP8266WebServer server(80);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIX, pix_pin, NEO_GRB + NEO_KHZ800);


/* HTML landing page */
const char* index_html = "<!DOCTYPE html>"
                         "<html>"
                         "<head>"
                         "<style>"
                         "p {"
                         "font-size: 3em;"
                         "}"
                         "a {"
                         "font-size: 4em;"
                         "}"
                         "body {"
                         "background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088;"
                         "}"
                         "</style>"
                         "</head>"
                         "<body>"
                         "<p>JDG ESP SERVER</p>"
                         "<p>Please select a button below.</p>"
                         "</br>"
                         "<a href=\"/MoveStepper\">Move Motor</a>"
                         "</body>"
                         "</html>";

/* HTML landing page */
const char* motor_html = "<!DOCTYPE html>"
                         "<html>"
                         "<head>"
                         "<meta http-equiv='refresh' content='3'/>"
                         "<style>"
                         "p {"
                         "font-size: 3em;"
                         "}"
                         "a {"
                         "font-size: 4em;"
                         "}"
                         "body {"
                         "background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088;"
                         "}"
                         "</style>"
                         "</head>"
                         "<body>"
                         "<p>JDG ESP SERVER</p>"
                         "<p>MOTOR IS ON</p>"
                         "<p>Please select a button below.</p>"
                         "</br>"
                         "<a href=\"/\">Home</a>"
                         "</body>"
                         "</html>";


/* Send the landing page */
void handleRoot() {
    digitalWrite(relay_pin, LOW);
    server.send(200, "text/html", index_html);
}


/* URL not found */
void handleNotFound() {
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET) ? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";

    for (uint8_t i = 0; i < server.args(); i++) {
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }

    server.send(404, "text/plain", message);
}


void moveStepper() {
    digitalWrite(relay_pin, HIGH);
    server.send(200, "text/html", motor_html);
}


/* Setup Everything Dawg */
void setup(void) {
    pinMode(limit_pin, INPUT_PULLUP);
    pinMode(relay_pin, OUTPUT);
    digitalWrite(relay_pin, LOW); // stepper motor is off
    randomSeed(analogRead(0));  // seed for random(), used for random PIXEL colors
    pixels.begin();
    pixels.show();
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        Serial.println("Not Connected");
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    if (MDNS.begin("esp8266")) {
        Serial.println("MDNS responder started");
    }
    server.on("/", handleRoot);
   // server.on("/PixOn", flashLED); add this at some point, not needed right now
    server.on("/MoveStepper", moveStepper);
    server.onNotFound(handleNotFound);
    server.on("/about", []() {
        server.send(200, "text/plain", "JDG ESP-8266 WEBSERVER\n2019");
    });
    // start the server
    server.begin();
    Serial.println("HTTP server started");
}


/* Loop -- Handles incoming clients*/
void loop(void) {
    server.handleClient();
    MDNS.update();
}
