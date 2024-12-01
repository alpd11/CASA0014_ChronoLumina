#include <WiFiNINA.h>   
#include <PubSubClient.h>
#include "arduino_secrets.h"

// Assign sensitive data to local constants for use in the program.
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
const char* mqtt_username = SECRET_MQTTUSER;
const char* mqtt_password = SECRET_MQTTPASS;

// Define the MQTT server address and port.
const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1884;

// Define the pins for the ultrasonic sensors used to measure distances.
const int trigPinR = 0;  // Trigger pin for the red sensor
const int echoPinR = 1;  // Echo pin for the red sensor
const int trigPinG = 2;  // Trigger pin for the green sensor
const int echoPinG = 3;  // Echo pin for the green sensor
const int trigPinB = 4;  // Trigger pin for the blue sensor
const int echoPinB = 5;  // Echo pin for the blue sensor

WiFiClient mkrClient; // Create a WiFi client instance for connectivity.
PubSubClient client(mkrClient); // Initialize an MQTT client using the WiFi client.

const char* mqtt_topic = "student/CASA0014/light/28/pixel/"; // Define the MQTT topic for publishing messages.

void setup() {.
  Serial.begin(9600);
  delay(1000); // Wait for the serial connection to stabilize.

  // Set the hostname for the WiFi module.
  WiFi.setHostname("Lumina ucfnadu");

  // Connect to WiFi and set up MQTT server.
  setupWifi();
  client.setServer(mqtt_server, mqtt_port);

  // Configure the ultrasonic sensor pins as input/output.
  pinMode(trigPinR, OUTPUT);
  pinMode(echoPinR, INPUT);
  pinMode(trigPinG, OUTPUT);
  pinMode(echoPinG, INPUT);
  pinMode(trigPinB, OUTPUT);
  pinMode(echoPinB, INPUT);

  Serial.println("Setup complete");
}

void loop() {
  // Ensure both WiFi and MQTT connections are maintained.
  checkConnections();

  // Measure distances from the ultrasonic sensors.
  int redValue = readUltrasonicDistance(trigPinR, echoPinR);
  int greenValue = readUltrasonicDistance(trigPinG, echoPinG);
  int blueValue = readUltrasonicDistance(trigPinB, echoPinB);

  // Map sensor distances to RGB intensity values, clamping within 5-50 cm range.
  int redIntensity = map(constrain(redValue, 5, 50), 5, 50, 255, 0);
  int greenIntensity = map(constrain(greenValue, 5, 50), 5, 50, 255, 0);
  int blueIntensity = map(constrain(blueValue, 5, 50), 5, 50, 255, 0);
  int whiteIntensity = 200; // Set a fixed intensity for the white color.

  // Output sensor distances and RGB intensity values for debugging.
  Serial.print("Distances - R: ");
  Serial.print(redValue);
  Serial.print(" cm, G: ");
  Serial.print(greenValue);
  Serial.print(" cm, B: ");
  Serial.println(blueValue);

  Serial.print("RGBW Intensity - R: ");
  Serial.print(redIntensity);
  Serial.print(", G: ");
  Serial.print(greenIntensity);
  Serial.print(", B: ");
  Serial.print(blueIntensity);
  Serial.print(", W: ");
  Serial.println(whiteIntensity);

  // Publish RGBW color data to the MQTT topic.
  sendRGBWColors(redIntensity, greenIntensity, blueIntensity, whiteIntensity);

  delay(200); // Introduce a delay to control the update frequency.
}

void setupWifi() {
  // Establish a connection to the WiFi network.
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(ssid, password);

    // Retry connection attempts for a maximum of 10 times.
    int retryCount = 0;
    while (WiFi.status() != WL_CONNECTED && retryCount < 10) {
      delay(500);
      Serial.print(".");
      retryCount++;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Connected to WiFi");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("WiFi connection failed. Retrying...");
    }
  }
}

void reconnectMQTT() {
  // Reconnect to the MQTT broker if the connection is lost.
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    // Generate a unique client ID for MQTT.
    String clientId = "LuminaSelector";
    clientId += String(random(0xffff), HEX);

    // Attempt to connect using the credentials.
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("MQTT connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(". Retrying in 2 seconds...");
      delay(2000);
    }
  }
}

void checkConnections() {
  // Check and maintain WiFi and MQTT connections.
  if (WiFi.status() != WL_CONNECTED) {
    setupWifi();
  }
  if (!client.connected()) {
    reconnectMQTT();
  }
  client.loop(); // Process incoming MQTT messages.
}

int readUltrasonicDistance(int trigPin, int echoPin) {
  // Send a 10-microsecond pulse to the trigger pin.
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Measure the duration of the echo pulse in microseconds.
  long duration = pulseIn(echoPin, HIGH, 25000);
  
  // Convert the duration to a distance in centimeters.
  int distance = duration > 0 ? duration * 0.034 / 2 : 999;
  
  // Return the distance if it's within the valid range; otherwise, return 999 (invalid).
  return (distance > 0 && distance <= 400) ? distance : 999;
}

void sendRGBWColors(int red, int green, int blue, int white) {
  // Format and publish RGBW color data for each of the 12 pixels.
  char mqtt_message[200];
  for (int pixelId = 0; pixelId < 12; pixelId++) {
    snprintf(mqtt_message, sizeof(mqtt_message), 
             "{\"pixelid\": %d, \"R\": %d, \"G\": %d, \"B\": %d, \"W\": %d}", 
             pixelId, red, green, blue, white);

    // Publish the message to the MQTT topic and log the result.
    if (client.publish(mqtt_topic, mqtt_message, false)) {
      Serial.print("MQTT message published for Pixel ");
      Serial.println(pixelId);
    } else {
      Serial.print("Failed to publish MQTT message for Pixel ");
      Serial.println(pixelId);
    }
  }
}
