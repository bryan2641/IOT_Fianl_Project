#include "painlessMesh.h"

#define   MESH_PREFIX     "mesh_wifi"
#define   MESH_PASSWORD   "somethingPASSWORD"
#define   MESH_PORT       5656

String msg = "Hi from node3 ";
Scheduler userScheduler; // to control your personal task
painlessMesh  mesh;

//MG90S
#include <Servo.h>
Servo servo_1; // servo controller (multiple can exist)
int servo_pin = 3; // PWM pin for servo control
int pos = 0;    // servo starting position

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain

Task taskSendMessage( TASK_SECOND * 1 , TASK_FOREVER, &sendMessage );

void sendMessage() {
  //String msg = "Hi from Father ";
  //msg += mesh.getNodeId();
  //mesh.sendBroadcast( msg );
  taskSendMessage.setInterval( random( TASK_SECOND * 1, TASK_SECOND * 5 ));
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  //Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
  Serial.println(msg.c_str());
  Servo(msg);
}

int run = 0;
void Servo(String msg){
  if(msg == "open"){
    run++;
    if(run == 1){
      /*for (int pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        servo_1.write(pos);              // tell servo to go to position in variable 'pos'
        delay(5); 
        // delay to allow the servo to reach the desired position
        if (pos==90){
          delay(1000); //wait 5 seconds once positioned at 90 degrees
        }
      }
      delay(1000); // wait 5 seconds after reaching 180 degrees*/
      servo_1.write(100);
    }   
  }
  else if(msg == "close"){
    run = 0;
    servo_1.write(20);
  }
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

  //MG90S
  servo_1.attach(servo_pin); // start servo control
}

void loop() {
  /*//MG90S
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servo_1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15); // delay to allow the servo to reach the desired position
    if (pos==90){
      delay(5000); //wait 5 seconds once positioned at 90 degrees
    }
  }
  delay(5000); // wait 5 seconds after reaching 180 degrees
 
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    servo_1.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);
    if (pos==90){
      delay(5000); // wait 5 seconds once positioned at 90 degrees
    }
  }
  delay(5000); // wait 5 seconds after arriving back at 0 degrees
*/
  // it will run the user scheduler as well
  //String abc = "Hi from node 3";
  //msg = abc;
  mesh.update();
  
}
