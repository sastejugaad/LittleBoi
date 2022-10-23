
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
//size_t numberOfStr = 0;  // number of valid elements in CommandArray[  ]

const char * ssid = "SINGTEL-42E6";
const char * password = "huiweimaev";
const char * BroadcastData = "";
AsyncUDP udp;
String PhrasedData = "";

int Speed=255;
int count = 1;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

TaskHandle_t xHandle = NULL;

QueueHandle_t queue;
int aninum;

void dispTask(void *pvParameters) {

     int rxaninum;
     while (1) {
        if (xQueueReceive(queue, &(rxaninum), portMAX_DELAY))
        { 
          if(rxaninum ==1)
          {
            Serial.println("left");
             //blink_eyes(500,2);
          }
          if(rxaninum ==2)
          {  Serial.println("right");
             //look_left(500);
          }
        }
           
     vTaskDelay(1000/ portTICK_RATE_MS);
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
            delay(1000);
        }
    }

      // Recieve data through wifi
  if(udp.listen(1234)) {
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
            //Serial.println(PhrasedData);   
        });      
  }
 //display 
 if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();

 queue = xQueueCreate(5, sizeof(int));
 
  if(queue == NULL){
    Serial.println("Error creating the queue");
  }
 
  xTaskCreatePinnedToCore(
    dispTask,   // Display task
    "dispTask", // Task name
    4096,       // Stack size
    NULL,       // No parameters
    1,          // Priority
    &xHandle,       // No handle returned
    0);         // CPU 1

}
void loop() {
  
  Plzgetstring(PhrasedData);
  Speed = (String(CommandArray [1])).toInt();
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
 xQueueSend(queue, (void*)aninum, (TickType_t)0);
}
if(String(CommandArray [0])=="R"|| String(CommandArray [0])=="r")
{
 right(PWM1,AIN1,AIN2,PWM2,BIN1,BIN2,Speed);
 aninum=2;
 xQueueSend(queue, (void*)aninum, (TickType_t)0);
}
if(String(CommandArray [0])=="S" || String(CommandArray [0])=="s")
{
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
