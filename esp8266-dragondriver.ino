/*
 *
 *  TEST PROGRAM TO MAKE SURE EVERYTHING WORKS
 *
 * Author: John Glatts
 * Brief: Nodemcu webserver with stepper motor controls and data
 * Date: 3/11/2019
 *
 *
 * ToDo:
 *  - Add limitswitch to ESP for Tx/Rx between boards, just to tell when a cycle has been completed
 *  - Check the delay between them
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
const char *ssid = "your-wifi";
const char *password = "your-password";


// keep track of cycles
int cycles = 0;
int new_cycles = 0;
int all_cycles[1000];   // arbitrary size at first


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


/* Setup Everything Dawg */
void setup(void) {
    pinMode(limit_pin, INPUT_PULLUP);
    pinMode(relay_pin, OUTPUT);
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


void moveStepper() {
    // relay-motor on
    digitalWrite(relay_pin, HIGH);

    // make sure PIXEl is off
    for (int i = 0; i < NUM_PIX; ++i) {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        pixels.show();
    }

    String step_html = "<!DOCTYPE html>";
    step_html += "<html>";
    step_html += "<head>";
    step_html += "<meta http-equiv='refresh' content='3'/>";
    step_html += "<style>";
    step_html += "p {";
    step_html += "font-size: 3em;";
    step_html += "}";
    step_html += "a {";
    step_html += "font-size: 4em;";
    step_html += "}";
    step_html += "body {";
    step_html += "background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088;";
    step_html += "}";
    step_html += "</style>";
    step_html += "</head>";
    step_html += "<body>";
    step_html += "<p>JDG ESP SERVER</p>";
    step_html += "<p># of Rotations: ";
    step_html += new_cycles;
    step_html += "</p>";
    step_html += "<p>Please select a button below.</p>";
    step_html += "<a href=\"/\">Home</a>";
    step_html += "</br>";
    step_html += "</body>";
    step_html += "</html>";

    if (cycles <= NUM_PIX && cycles > 0) {
        for (int x = 1; x <= cycles; ++x) {
            pixels.setPixelColor(x,  pixels.Color(255, 0, 0));
            pixels.show();
        }
        //delay(1000);     check if delay is needed
    } else {
        for (int x = 1; x <= cycles; ++x) {
            pixels.setPixelColor(x,  pixels.Color(0, 0, 0));
            pixels.show();
        }
    }

    server.send(200, "text/html", step_html);
    new_cycles++;

    // check if we have to change cycles
    if (cycles > NUM_PIX) {
        cycles = 0;
    } else {
        cycles++;
    }

}
