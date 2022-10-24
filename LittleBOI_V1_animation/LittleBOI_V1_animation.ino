//https://www.dfrobot.com/blog-912.html
//https://www.freertos.org/a00117.html
//https://esp32tutorials.com/esp32-esp-idf-freertos-queue-tutorial/
//https://techtutorialsx.com/2017/09/13/esp32-arduino-communication-between-tasks-using-freertos-queues/
//https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/system/freertos.html#queue-api


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


void dispTask(void *pvParameters) {

     int rxaninum;
     while (1) {
         xQueueReceive(queue, &(rxaninum), portMAX_DELAY);//(TickType_t)0  portMAX_DELAY
         
          if(rxaninum ==1)
          {
            Serial.println("left");
             look_left(1000);
              xQueueReset( queue ); 
          }
          if(rxaninum ==2)
          {  Serial.println("right");
             look_right(500);
          }
        
           
    //vTaskDelay(1/ portTICK_RATE_MS);
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

 queue = xQueueCreate(1, sizeof(int));
 
  if(queue == NULL){
    Serial.println("Error creating the queue");
  }
 
  xTaskCreatePinnedToCore(
    dispTask,   // Display task
    "dispTask", // Task name
    4096,       // Stack size
    NULL,       // No parameters
    0,          // Priority
    &xHandle,       // No handle returned
    0);         // CPU 1

}
void loop() {
  int aninum;
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
 xQueueSend(queue, &aninum, portMAX_DELAY);
}
if(String(CommandArray [0])=="R"|| String(CommandArray [0])=="r")
{
 right(PWM1,AIN1,AIN2,PWM2,BIN1,BIN2,Speed);
// aninum=2;
// xQueueSend(queue, &aninum, portMAX_DELAY);
}
if(String(CommandArray [0])=="S" || String(CommandArray [0])=="s")
{
 Stop(PWM1,AIN1,AIN2,PWM2,BIN1,BIN2,Speed); 
 //xQueueReset( queue );
 //aninum=0;
 //xQueueSend(queue, &aninum, portMAX_DELAY);
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
