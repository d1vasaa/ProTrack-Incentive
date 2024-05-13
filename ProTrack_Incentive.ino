#include <HTTPClient.h>
#include <WiFi.h>
#include <ESP32Servo.h>

// Define the PWM pin connected to the servo
#define SERVO_PIN 13 // Change this to the desired PWM pin number

const char* ssid = "pocopoco2@unifi";
const char* password = "24681012";

// Create a Servo object
Servo myServo;

void setup() {
  // Attach the servo to the PWM pin
  myServo.attach(SERVO_PIN);
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
}

void loop() {
  get_data();

  delay(1000);
}

void get_data()
{
  HTTPClient http;

  // Replace the URL with the webpage you want to fetch HTML content from
  http.begin("https://vaunted-sleeve.000webhostapp.com/main.php");

  http.addHeader("Content-Type", "application/x-www-form-urlencoded");

  int httpCode = http.GET();

   if (httpCode > 0) {
    String payload = http.getString();
    int servoStatusIndex = payload.indexOf("Servo Status: ");
    if (servoStatusIndex != -1) {
      int servoStatusLineEnd = payload.indexOf("</p>", servoStatusIndex);
      if (servoStatusLineEnd != -1) {
        // Extract the servo status value without "Servo Status: " and "</p>"
        String servoStatus = payload.substring(servoStatusIndex + 14, servoStatusLineEnd);
        
        // Convert servoStatus to integer
        int servoStatusValue = servoStatus.toInt();

        if (servoStatusValue == 0)
        {
          myServo.write(0);
        }
        else if (servoStatusValue == 1)
        {
          myServo.write(105);
        }
        Serial.println(servoStatus);
      }
    } else {
      Serial.println("Servo status line not found in HTML content");
    }
  } else {
    Serial.println("Error on HTTP request");
  }

  http.end();
}
