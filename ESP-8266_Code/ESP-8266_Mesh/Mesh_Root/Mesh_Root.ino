#include "painlessMesh.h"

#define   MESH_PREFIX     "mesh_wifi"
#define   MESH_PASSWORD   "somethingPASSWORD"
#define   MESH_PORT       5656

String sendmsg = "";
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

//LineNotify + ThinkSpeak
/*#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SimpleDHT.h>
#include "TridentTD_LineNotify.h"   //匯入TridentTD_LineNotify程式庫
#define LINE_TOKEN "PonVRq8pcKSqU9xBqHekkY375tVkdT8jpHt0wc4VU6K"  //複製你的LINE權杖
char ssid[] = "Linksys";
char password[] = "26413499";
String url = "http://api.thingspeak.com/update?api_key=TOMZI3GG5HV0VIXX";*/

//Buzzer
int buzzerPin = 5;
int duration = 100;
int aSo = 392;
int bDo = 523;
int bRe = 587;
int bMi = 659;
int bFa = 698;
int bSo = 784;
int bLa = 880;
int bSi = 988;
int bDDo = 1047;

char data;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  //String msg = "Hi from node ";
  //msg += mesh.getNodeId();
  mesh.sendBroadcast( sendmsg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  //Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  Serial.println(msg.c_str());
  //ValueProcessing(msg);
}

void newConnectionCallback(uint32_t nodeId) {
  //Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  //Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
  //Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

//判斷數據是第幾台回傳的
/*void ValueProcessing(String &msg){
  LINE.notify("檢測環境發生異常，請協助儘速派人查看處理，偵測到環境溫度異常");
  String values[10];
  int commaIndex = 0;
  int prevIndex = 0;
  for (int i = 0; i < 5; ++i) {
    commaIndex = msg.indexOf(',', prevIndex); // 找下一個逗號的位置
    if (commaIndex != -1) {
      values[i] = msg.substring(prevIndex, commaIndex); // 提取數值或字串
      prevIndex = commaIndex + 1; // 更新索引
    } else {
      values[i] = msg.substring(prevIndex); // 如果沒有逗號，則取剩餘部分
      break;
    }
  }

  //上傳ThinkSpeak
  WiFiClient client;
  HTTPClient http;
  
  int temperature = 0;
  int humidity = 0;
  int lux = 0;
  int COppm = 0;
  int gas = 0;
  int pm25 = 0;
  String url1;
  //如果第一個數值是1(節點1)
  if(values[0] = "1"){
    temperature = values[1].toInt();
    humidity = values[2].toInt();
    lux = values[3].toInt();
    COppm = values[4].toInt();

    //將溫度及濕度以HTTP Get參數方式補入網址後方
    ////待辦，補完...
    //url1 = url + "&amp;field1=" + (int)temperature + "&amp;field2=" + (int)humidity+ "&amp;field3=" + (int)lux+ "&amp;field4=" + (int)COppm;
    url1 = url + "&amp;field1=" + temperature;
  }

  //如果第一個數值是2(節點2)
  ////待辦，將數值對應第二台的感測器並且補完ThinkSpeak
  else if(values[0] = "2"){
    gas = values[5].toInt();
    pm25 = values[6].toInt();
    url1 = url + "&amp;field1=" + (int)gas + "&amp;field2=" + (int)pm25;
  }
  Serial.println(temperature);

  //ThinkSpeak
  //http client取得網頁內容
  http.begin(client, url1);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    //讀取網頁內容到payload
    String payload = http.getString();
    //將內容顯示出來
    Serial.print("網頁內容=");
    Serial.println(payload);
  } else {
    //讀取失敗
    Serial.println("網路傳送失敗");
  }
  http.end();

  //判斷數值有沒有過高，有的話呼叫下面函式
  ////待辦，撰寫判斷程式
  //情況一，溫度高於30度的話將待傳送訊息改成open，當open這則訊息傳送到第三台node則會控制伺服馬達開啟開關;溫度高於40則呼叫下面函式
  if(temperature > 30){
    msg = "open";
    if(temperature > 40){
      SomeThingHappened(1);
    }   
  }
  //情況二，濕度高於60或低於10呼叫下面程式
  if(humidity > 60 ){
    SomeThingHappened(2);
  }
  //情況三，一氧化碳濃度高於3呼叫下面程式
  if(COppm > 3){
    SomeThingHappened(3);
  }
  //情況四，可燃氣體???
  if(gas > 3){
    SomeThingHappened(4);
  }
  //情況五，PM2.5用香來測
  if(pm25 > 50){
    SomeThingHappened(5);
  }
  
}

//若回傳數據有異常則用line通知及蜂鳴器警告(蜂鳴器會發不同長短聲響對應不同數值異常)
////待辦，蜂鳴器
void SomeThingHappened(int Situation){
  LINE.setToken(LINE_TOKEN);  //設定要傳遞的權杖
  if(Situation == 1){
    LINE.notify("檢測環境發生異常，請協助儘速派人查看處理，偵測到環境溫度異常");
    tone(5,bDo,duration);
    delay(600);
  }
  else if(Situation == 2){
    LINE.notify("檢測環境發生異常，請協助儘速派人查看處理，偵測到環境濕度異常");
    tone(5,bRe,duration);
    delay(600);
  }
  else if(Situation == 3){
    LINE.notify("檢測環境發生異常，請協助儘速派人查看處理，偵測到環境一氧化碳濃度異常");
    tone(5,bMi,duration);
    delay(600);
  }
  else if(Situation == 4){
    LINE.notify("檢測環境發生異常，請協助儘速派人查看處理，偵測到環境可燃氣體異常");
    tone(5,bFa,duration);
    delay(600);
  }
  else if(Situation == 5){
    LINE.notify("檢測環境發生異常，請協助儘速派人查看處理，偵測到環境PM2.5異常");
    tone(5,bSo,duration);
    delay(600);
  }
}*/

void setup() {
  Serial.begin(115200);

  //WIFI
  /*
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }*/

  //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  //Buzzer
  pinMode(5,OUTPUT);
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
  if (Serial.available()){
    data = Serial.read();
    if(data == '1'){
      //Serial.println("點亮LED");
      sendmsg = "open";
    }
    if(data == '0'){
      //Serial.println("關閉LED");
      sendmsg = "close";
    }
    if(data == '2'){
      //Serial.println("溫度警報");
      tone(5,bDo,duration);
    }
    if(data == '3'){
      //Serial.println("濕度警報");
      tone(5,bRe,duration);
    }
    if(data == '4'){
      //Serial.println("Co警報");
      tone(5,bMi,duration);
    }
    if(data == '5'){
      //Serial.println("瓦斯警報");
      tone(5,bFa,duration);
    }
    if(data == '6'){
      //Serial.println("PM2.5警報");
      tone(5,bSo,duration);
    }
  }
}
