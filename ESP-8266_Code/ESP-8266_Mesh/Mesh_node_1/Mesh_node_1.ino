#include "painlessMesh.h"

#define   MESH_PREFIX     "mesh_wifi"
#define   MESH_PASSWORD   "somethingPASSWORD"
#define   MESH_PORT       5656

//DHT
#include "DHTesp.h"
DHTesp dht;
#define DHTTYPE DHT11
float currentTemp;
float currentHumidity;
const int dhtPing = 0;

//Light
#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;
int BH1750Check;

//MQ-7
#include <MQUnifiedsensor.h>
MQUnifiedsensor MQ7("Arduino UNO", 5, 10, A0, "MQ-7");

String msg = "Hi from node ";
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  //String msg = "Hi from Father ";
  //msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  //Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  Serial.println(msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  userScheduler.addTask( taskSendMessage );
  taskSendMessage.enable();

  //DHT
  pinMode (dhtPing, INPUT);
  dht.setup(dhtPing, DHTesp::DHT11);
  Serial.begin(115200);
  Serial.setTimeout(100);

  //Light
  // Initialize the I2C bus (BH1750 library doesn't do this automatically)  
  Wire.begin(4,5);
  // On esp8266 you can select SDA and SCL pins using Wire.begin(D4, D3);

  //lightMeter.begin();
  //  Serial.println(F("BH1750 Test begin"));

  
  if (lightMeter.begin()) {
    Serial.println(F("BH1750 Test begin"));
  }
  else {
    Serial.println(F("BH1750 Initialization FAILED"));
    while (true) //flow trap
    {}
  }

  //MQ-7
  MQ7.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ7.setA(99.042); MQ7.setB(-1.518); // Configurate the ecuation values to get CO concentration
  MQ7.init(); 

  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ7.update(); // Update data, the arduino will be read the voltage on the analog pin
    calcR0 += MQ7.calibrate(27.5);
    Serial.print(".");
  }
  MQ7.setR0(calcR0/10);
  Serial.println("  done!.");
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue founded, R0 is infite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue founded, R0 is zero (Analog pin with short circuit to ground) please check your wiring and supply"); while(1);}
  MQ7.serialDebug(true);
}

void loop() {
  //DHT
  float temperature = dht.getTemperature();
  float humidity = dht.getHumidity();
  
  /*Serial.print("Temperature : ");
  Serial.println(temperature);
  Serial.print("Humidity : ");
  Serial.println(humidity);
  delay(1000);*/

  //Light
  float lux = lightMeter.readLightLevel();
  /*Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);*/

  //MQ-7
  MQ7.update(); // Update data, the arduino will be read the voltage on the analog pin
  float COppm = MQ7.readSensor(); // Sensor will read PPM concentration using the model and a and b values setted before or in the setup
  /*Serial.print("CO: ");
  Serial.print(COppm);
  Serial.println(" ppm");
  delay(1000); //Sampling frequency*/

  // it will run the user scheduler as well
  String abc = "1," + String(temperature) + "," + String(humidity) + "," + String(lux) + "," + String(COppm);
  //String abc = String(temperature);
  msg = abc;
  mesh.update();
  //delay(1000);
}
