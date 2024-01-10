int buzzer=5;                   //設定蜂鳴器接腳為第3孔
int duration = 500;
int aSo = 392;
int bDo = 523;
int bRe = 587;
int bMi = 659;
int bFa = 698;
int bSo = 784;
int bLa = 880;
int bSi = 988;
int bDDo = 1047;
void setup() {
  // put your setup code here, to run once:
  pinMode(buzzer,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  tone(buzzer,bDo,duration);
  delay(600);
  tone(buzzer,bRe,duration);
  delay(600);
  tone(buzzer,bMi,duration);
  delay(600);
  tone(buzzer,bDo,duration);
  delay(800);
}
