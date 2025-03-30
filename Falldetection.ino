#define BLYNK_TEMPLATE_ID "TMPL3uTkoOBwW" 
#define BLYNK_TEMPLATE_NAME "fall detection" 
#define BLYNK_AUTH_TOKEN "xxxxvy0k5qbp7" 
#include <Wire.h> 
#include <MPU6050.h> 
#include <WiFi.h> 
#include <BlynkSimpleEsp32.h> 
const char* ssid = "iPhone"; 
const char* password = "xxxx"; 
char auth[] = BLYNK_AUTH_TOKEN; 
MPU6050 mpu; 
const int ledPin = 18;    // LED connected to GPIO 18 
const int buzzerPin = 19; // Buzzer connected to GPIO 19 
float upperThreshold = 2.5;  // Upper limit to detect sharp movement 
float lowerThreshold = 0.5;  // Lower limit for a fall (near free fall) 
BlynkTimer timer; 
void checkFallDetection() { 
int16_t ax, ay, az; 
mpu.getAcceleration(&ax, &ay, &az); 
float totalAcceleration = sqrt(ax * ax + ay * ay + az * az) / 16384.0;  
Blynk.virtualWrite(V0, totalAcceleration);  
Serial.print("Total Acceleration: "); 
Serial.println(totalAcceleration); 
if (totalAcceleration > upperThreshold || totalAcceleration < lowerThreshold) 
{ 
Serial.println("Fall detected!"); 
digitalWrite(ledPin, HIGH); 
digitalWrite(buzzerPin, HIGH); 
Blynk.virtualWrite(V1, 255);  

Blynk.logEvent("fall_detected"); 
delay(2000);   
digitalWrite(ledPin, LOW); 
digitalWrite(buzzerPin, LOW); 
Blynk.virtualWrite(V1, 0);  
} else { 
digitalWrite(ledPin, LOW); 
digitalWrite(buzzerPin, LOW); 
} 
} 
void setup() { 
pinMode(ledPin, OUTPUT); 
pinMode(buzzerPin, OUTPUT); 
digitalWrite(ledPin, LOW); 
digitalWrite(buzzerPin, LOW); 
Serial.begin(115200);   
Wire.begin();           
mpu.initialize(); 
if (mpu.testConnection()) { 
Serial.println("MPU6050 connection successful."); 
} else { 
Serial.println("MPU6050 connection failed! Check wiring."); 
while (1);   
} 
mpu.setSleepEnabled(false); 
WiFi.begin(ssid, password); 
while (WiFi.status() != WL_CONNECTED) { 
delay(1000); 
Serial.println("Connecting to WiFi..."); 
} 
Serial.println("Connected to WiFi"); 
Blynk.begin(auth, ssid, password); 
timer.setInterval(500L, checkFallDetection);   
} 
void loop() { 
Blynk.run();   
timer.run();   
}