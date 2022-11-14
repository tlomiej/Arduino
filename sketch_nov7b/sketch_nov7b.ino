int motor1pin1 = 2;
int motor1pin2 = 3;

int motor2pin1 = 4;
int motor2pin2 = 5;

char command;

/* czyjnik odległości */
int Trig = 9;  //pin 2 Arduino połączony z pinem Trigger czujnika
int Echo = 8;  //pin 3 Arduino połączony z pinem Echo czujnika
int CM;        //odległość w cm
long CZAS;     //długość powrotnego impulsu w uS
int barrier = 0; //0 brak, 1 jest 

void setup() {
  Serial.begin(9600);  //Set the baud rate to your Bluetooth module.
  // put your setup code here, to run once:
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(Trig, OUTPUT);  //ustawienie pinu 2 w Arduino jako wyjście
  pinMode(Echo, INPUT);   //ustawienie pinu 3 w Arduino jako wejście

  Serial.println("Test czujnika odleglosci1");
}

void loop() {

  pomiarOleglosci();
  

  Serial.flush();
  Serial.print(CM);
  Serial.print("WYnik");

  if (CM < 10 and barrier == 0) {
          barrier = 1;
          Stop();
        }

  if (Serial.available() > 0) {
    pomiarOleglosci();  //pomiar odległości
                        //Serial.print("Odleglosc: ");  //wyświetlanie wyników na ekranie w pętli co 200 ms
                        //Serial. print(CM);
                        //Serial.println(" cm");
                        //delay(200);

    command = Serial.read();

  
    Stop();  //initialize with motors stoped
    //Change pin mode only if new command is different from previous.
    //Serial.println(command);
    switch (command) {
      case 'F':
        Serial.print("Prosto");


        forward();


        barrier = 0;

        break;
      case 'B':
        back();
        break;
      case 'L':
        left();
        break;
      case 'R':
        right();
        break;
      case 'S':
        Stop();
        Stop();
        break;

      case 'X':
        forward();
        delayMicroseconds(2000);
        left();
        delayMicroseconds(500);
        forward();
        delayMicroseconds(3000);
        Stop();
        break;
    }
  }
}




void pomiarOleglosci() {
  digitalWrite(Trig, LOW);  //ustawienie stanu wysokiego na 2 uS - impuls inicjalizujacy - patrz dokumentacja
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  //ustawienie stanu wysokiego na 10 uS - impuls inicjalizujacy - patrz dokumentacja
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  digitalWrite(Echo, HIGH);
  CZAS = pulseIn(Echo, HIGH);
  delayMicroseconds(100);
  CM = CZAS / 58;  //szerokość odbitego impulsu w uS podzielone przez 58 to odleglosc w cm - patrz dokumentacja
}


void forward() {
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  //delay(1000);
}

void back() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_BUILTIN, LOW);
}

void left() {
  digitalWrite(motor1pin1, HIGH);
  digitalWrite(motor1pin2, LOW);

  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, HIGH);
}

void right() {
 digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, HIGH);

  digitalWrite(motor2pin1, HIGH);
  digitalWrite(motor2pin2, LOW);
}


void Stop() {
  digitalWrite(motor1pin1, LOW);
  digitalWrite(motor1pin2, LOW);
  digitalWrite(motor2pin1, LOW);
  digitalWrite(motor2pin2, LOW);
}