#include <WiFi.h>
#include <HTTPClient.h>
#include <ESP32Servo.h> // Include the ESP32Servo library

Servo myservo;  // create servo object to control a servo

// Your existing WiFi credentials and server details
const char* ssid = "Aimanisaac";
const char* pass = "rasydan808";
String payload;

const char* serv = "link goes here";

void setup() {
  myservo.attach(15);  // attaches the servo on GPIO pin 18 to the servo object
  
  Serial.begin(115200); 
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }
  Serial.println(" ");
  Serial.print("Connected to WiFi Network With Local IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  get_data();
  if (!payload.isEmpty()) {
    Serial.println("Full payload:");
    Serial.println(payload);
    turnOnLED(payload);
  }
  delay(2000); // Wait 1 second before the next request
}

void get_data() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    String serverPath = String(serv) + "/data/1/status";

    http.begin(serverPath.c_str());

    int httpResponse = http.GET();

    if (httpResponse > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponse);
      payload = http.getString();
    } else {
      Serial.print("Error: ");
      Serial.println(httpResponse);
      printHttpError(httpResponse);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

// Function to print detailed HTTP error messages
void printHttpError(int errorCode) {
  switch (errorCode) {
    case HTTPC_ERROR_CONNECTION_REFUSED:
      Serial.println("Connection refused");
      break;
    case HTTPC_ERROR_SEND_HEADER_FAILED:
      Serial.println("Send header failed");
      break;
    case HTTPC_ERROR_SEND_PAYLOAD_FAILED:
      Serial.println("Send payload failed");
      break;
    case HTTPC_ERROR_NOT_CONNECTED:
      Serial.println("Not connected");
      break;
    case HTTPC_ERROR_CONNECTION_LOST:
      Serial.println("Connection lost");
      break;
    case HTTPC_ERROR_NO_STREAM:
      Serial.println("No stream");
      break;
    case HTTPC_ERROR_NO_HTTP_SERVER:
      Serial.println("No HTTP server");
      break;
    case HTTPC_ERROR_TOO_LESS_RAM:
      Serial.println("Too less RAM");
      break;
    case HTTPC_ERROR_ENCODING:
      Serial.println("Encoding error");
      break;
    case HTTPC_ERROR_STREAM_WRITE:
      Serial.println("Stream write error");
      break;
    case HTTPC_ERROR_READ_TIMEOUT:
      Serial.println("Read timeout");
      break;
    default:
      Serial.println("Unknown error");
      break;
  }
}

void turnOnLED(const String& STATUS) {
  if (STATUS == "\"ON\"") {
    myservo.write(180);  // Move servo to 180 degrees instantly
  } else if (STATUS == "\"OFF\"") {
    myservo.write(0);    // Move servo back to 0 degrees instantly
  }
}
