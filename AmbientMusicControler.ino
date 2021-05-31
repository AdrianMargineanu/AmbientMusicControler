int pwmA = 6; // blue
int pwmB = 11;//red
int pwmC = 5; // green
int enA = 12;
int enB = 13;
int buz = 4;
int values[10] = {0};
int counter = 0;
double avgValue;

void setup() {
  pinMode(pwmA, OUTPUT);
  pinMode(pwmB, OUTPUT);
   pinMode(pwmC, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(buz, OUTPUT);
  Serial.begin(9600);
}
void loop() {
    int sensorValue = analogRead(A0);
    values[counter] = sensorValue;
    int sum = 0;
    for (int i = 0; i < 10; i++)
      sum += values[i];

    avgValue = sum / 10;
    
    avgValue -= 250;
      Serial.println(avgValue);
    if(abs(avgValue) < 10){//blue
    analogWrite(pwmB,0);
    analogWrite(pwmA, 100);
    analogWrite(pwmC,0);
    }else if(abs(avgValue) < 20){//red

     analogWrite(pwmA, 0);
     analogWrite(pwmB,100);
     analogWrite(pwmC,0);
    }else{//green
      analogWrite(pwmA, 0);
     analogWrite(pwmB,0);
     analogWrite(pwmC,100);
    }
    if (counter == 9){
      counter = -1;
    }
      counter++;

}  
