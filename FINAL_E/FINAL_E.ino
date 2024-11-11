/*************************************************************

  This is a simple demo of sending and receiving some data.
  Be sure to check out other examples!
 *************************************************************/

/* Fill-in information from Blynk Device Info here */
#include <SoftwareSerial.h>
#define BLYNK_TEMPLATE_ID           "TMPL6H1yG62VQ"
#define BLYNK_TEMPLATE_NAME         "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "Qj63zMHAKgDPnw6eH3-kGJjGU_9QtXuh"

#include <TridentTD_LineNotify.h>

#define SSID        "ลู้ๆล้าๆ"                                    
#define PASSWORD    "pumpuy281151"                                   
#define LINE_TOKEN  "BjLGUw3cCZ1oRhTzG9lF7sEyZ3R0FJsjgWaBrOx2aAB"
#define BLYNK_PRINT Serial

SoftwareSerial mySerial(4,5); // TX, RX
unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;
int Relay1 = D3;
int LED1 = D6;
int LED2 = D7;
int buzzer = D8;

int val = 0;

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Servo.h>
#include <Wire.h>

Servo myservo;


// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "ลู้ๆล้าๆ";
char pass[] = "pumpuy281151";

BlynkTimer timer;

// This function is called every time the Virtual Pin 0 state changes 
BLYNK_WRITE(V0)
{
  int value = param.asInt();
  // Update state
}
BLYNK_WRITE(V1) {
// Called when the datastream V1 value changes

// Assign incoming value from pin V1 to a variable
// according to the datastream data type
int pinValue = param.asInt(); 
// double pinValue = param.asDouble();

}

// This function is called every time the device is connected to the Blynk.Cloud
BLYNK_CONNECTED()
{
  // Change Web Link Button message to "Congratulations!"
  Blynk.setProperty(V3, "offImageUrl", "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations.png");
  Blynk.setProperty(V3, "onImageUrl",  "https://static-image.nyc3.cdn.digitaloceanspaces.com/general/fte/congratulations_pressed.png");
  Blynk.setProperty(V3, "url", "https://docs.blynk.io/en/getting-started/what-do-i-need-to-blynk/how-quickstart-device-was-made");
}

// This function sends Arduino's uptime every second to Virtual Pin 2.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, millis() / 1000);
}





void setup()
{
  pinMode(Relay1, OUTPUT); // กำหนดขาทำหน้าที่ให้ขา 2 เป็น OUTPUT
  digitalWrite(Relay1, HIGH);

  pinMode(LED1, OUTPUT);
  digitalWrite(LED1,LOW);

  pinMode(LED2, OUTPUT);
  digitalWrite(LED2,LOW);
  pinMode(buzzer, OUTPUT);
  myservo.attach(2);

  // Debug console
  Serial.begin(115200);
while (!Serial) ;
mySerial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  Serial.println();
  Serial.println(LINE.getVersion());

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());
  // You can also specify server:
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);
   timer.setInterval(5000L,PM);
}

void loop()
{
  Blynk.run();
  timer.run();
}

void PM(){
  int index = 0;
char value;
char previousValue;
 int pm25;
 LINE.setToken(LINE_TOKEN);
while (mySerial.available()) {
value = mySerial.read();
if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)){
Serial.println("Cannot find the data header.");
break;
}
 
if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
previousValue = value;
}
else if (index == 5) {
pm1 = 256 * previousValue + value;
Serial.print("{ ");
Serial.print("\"pm1\": ");
 Blynk.virtualWrite(V10, pm2_5);
Serial.print(pm1);
Serial.print(" ug/m3");
Serial.print(", ");
}
else if (index == 7) {
pm2_5 = 256 * previousValue + value;
Serial.print("\"pm2_5\": ");
 Blynk.virtualWrite(V9, pm2_5);
Serial.print(pm2_5);
Serial.print(" ug/m3");
Serial.print(", ");
}
else if (index == 9) {
pm10 = 256 * previousValue + value;
Serial.print("\"pm10\": ");
 Blynk.virtualWrite(V8, pm10);
Serial.print(pm10);
Serial.print(" ug/m3");
} 
else if (index > 15) {
break;
}
index++;
}
while(mySerial.available()) mySerial.read();
Serial.println(" }");
delay(1000);
 pm25 = pm2_5;

 myservo.write(0);
 delay(1000);
 myservo.write(180);
 delay(1000);

 if(pm25 >= 40)
 {
   Serial.println(pm25);
 LINE.notify("PM2.5 มีค่ามากกว่าปกติ! โปรดช่วยด้วยยยๆ");
 LINE.notify("PM 2.5 = ");
 LINE.notify(pm25);
 digitalWrite(Relay1,LOW);
 digitalWrite(LED1,HIGH);
 digitalWrite(LED2,LOW);
  digitalWrite(buzzer, HIGH);
  delay(1000);
  digitalWrite(buzzer, LOW);
 }else
 {
  digitalWrite(Relay1, HIGH);
  digitalWrite(LED1,LOW);
  digitalWrite(LED2,HIGH);
  digitalWrite(buzzer, LOW);
 }

}
