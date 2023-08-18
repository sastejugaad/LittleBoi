/* ESP32 robot with freeRTOS and facial expressions. It is controlled wirelessly through a wifi app.  
 * Robots can show emotions like vectors and cosmos. It has an MPU6050 and an Oled screen.  
 * The real-time operating system will allow me to push ESP32 at its limit. 
 * This is the first version of this project. I have only added basic expressions more are coming in the future. 
 * I will also add SLAM and Swarming features in the future.
 * Download the wifi app from Git Hub. Get the Ip address from the serial monitor. UDP port is 1234 unless you have changed the code.
 */


#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "WiFi.h"
#include "roboteyes.h"
#include "AsyncUDP.h"
#include "seperatestring.h"
#include "MotorController.h"
#define freertos/queue.h

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//char Direction ='';
//left side motor
#define PWM1 12
#define AIN1 14
#define AIN2 27
//Right side motor
#define PWM2 32
#define BIN1 33
#define BIN2 25
//#define STBY 26
#define CommandArray_SIZE   20

char *CommandArray [CommandArray_SIZE]; // this is reused each call
char *strData = NULL; // this is allocated in separate and needs to be free( ) eventually

const char * ssid = "SINGTEL-42E6";//Change your wifi name
const char * password = "huiweimaev";//Change your wifi password
const char * BroadcastData = "";
AsyncUDP udp;
String PhrasedData = "";

int Speed=255;
int count = 1;
int aninum=0;
// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

TaskHandle_t xHandle = NULL;
//Animation task
//There are better ways of doing this but this was the easiest.
//Let me know if you can make this part better.
void dispTask(void *pvParameters) {

     while (1) {   
          if(aninum ==1) //animation number 1 when robot goes left
          {
            //Serial.println("left");
             look_left(500);
          }
          if(aninum ==2)//animation number 2 when robot goes right
          {  //Serial.println("right");
             look_right(500);
          }
          if(aninum ==0)//animation number 0 for rest.
          {  //Serial.println("right");
             //blink_eyes(1000,1);
             sad_blink(400,1);
          }
         // vTaskDelete(xHandle);
           //vTaskSuspend( NULL );
    vTaskDelay(1);
    }
}

void setup() {
      //Motor drivers pins
  pinMode(PWM1,OUTPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(PWM2,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  Serial.begin(115200);
  //-startwifi  
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            vTaskDelay(500/ portTICK_RATE_MS);
        }
    }
      // Recieve data through wifi
  if(udp.listen(1234)) {//UDP port number if there is any problem change this.
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([] (AsyncUDPPacket packet) {      
            /*
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            */
            //Data phrasing making recieved readable into string
            //packet.printf("Got %u bytes of data", packet.length());
            char* tmpStr = (char*) malloc(packet.length() + 1);
            memcpy(tmpStr, packet.data(), packet.length());
            tmpStr[packet.length()] = '\0'; // ensure null termination
            PhrasedData = String(tmpStr);
            free(tmpStr); // Strign(char*) creates a copy so we can delete our one
            Serial.println(PhrasedData);   
        });      
  }
 //display 
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  //Create a task to diaply eyes animation
    xTaskCreatePinnedToCore(
    dispTask,   // Display task
    "dispTask", // Task name
    4096,       // Stack size
    NULL,       // No parameters
    0,          // Priority
    &xHandle,       // No handle returned
    0);

}
void loop() {
  
  Plzgetstring(PhrasedData);// Get the incoming data and seprerate the commands into array.
  Speed = (String(CommandArray [1])).toInt();//Speed of the robot
  if(String(CommandArray [0])=="f"|| String(CommandArray [0])=="F")
{
 forward(PWM1,AIN1,AIN2,PWM2,BIN1,BIN2,Speed); 
}
if(String(CommandArray [0])=="b"|| String(CommandArray [0])=="B")
{
 backward(PWM1,AIN1,AIN2,PWM2,BIN1,BIN2,Speed); 
}
if(String(CommandArray [0])=="L"|| String(CommandArray [0])=="l")
{
 left(PWM1,AIN1,AIN2,PWM2,BIN1,BIN2,Speed);
 aninum=1;
}
if(String(CommandArray [0])=="R"|| String(CommandArray [0])=="r")
{
 right(PWM1,AIN1,AIN2,PWM2,BIN1,BIN2,Speed);
 aninum=2;
}
if(String(CommandArray [0])=="S" || String(CommandArray [0])=="s")
{
 aninum=0;
 Stop(PWM1,AIN1,AIN2,PWM2,BIN1,BIN2,Speed);
} 
}

void Plzgetstring(String phraseData)
{
  String str;
  int N = 0;
  str = phraseData;
  //Serial.print(F("input str:")); Serial.println(str);
  N = separate (str, CommandArray, CommandArray_SIZE,  &strData);
 // for (int n = 0; n < N; n++) {
   // Serial.println (CommandArray [n]);
 // }
  freeData(&strData);
}
