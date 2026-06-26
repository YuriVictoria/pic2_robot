#define M1_1 7
#define M1_2 8
#define M1_en 9

#define M2_1 5
#define M2_2 4
#define M2_en 6

//pinos analogicos para os encoders AS5600 (pino OUT)
const int pino_M1 = A0;
const int pino_M2 = A1;

volatile long encoderCount_M1 = 0;
volatile long encoderCount_M2 = 0;

int lastRaw_M1 = 0;
int lastRaw_M2 = 0;

void handle_motor_speed(String);
void provide_encoder_values();
void set_speed(int, int);
void forward();
void backward();
void stop_motor();
void leftward();
void rightward();

void setup() {
  Serial.begin(9600);

  pinMode(M1_1, OUTPUT);
  pinMode(M1_2, OUTPUT);
  pinMode(M1_en, OUTPUT);

  pinMode(M2_1, OUTPUT);
  pinMode(M2_2, OUTPUT);
  pinMode(M2_en, OUTPUT);

  //leitura inicial para evitar saltos de odometria ao ligar
  lastRaw_M1 = analogRead(pino_M1);
  lastRaw_M2 = analogRead(pino_M2);
}

void loop() {
  atualizarEncodersAnalogicos();
  static String command;

  while (Serial.available() > 0) {
    char incomingChar = Serial.read();

    if (incomingChar == '\n') {
      if (command[0] == 'm') {
        handle_motor_speed(command);
        Serial.println("ok");
      }
      else if (command[0] == 'e') {
        provide_encoder_values();
      }
      command = "";
    }
    else {
      command += incomingChar;
    }
  }

}

void atualizarEncodersAnalogicos() {
  //leitura da posicao (0 a 1023)
  int currentRaw_M1 = analogRead(pino_M1);
  int currentRaw_M2 = analogRead(pino_M2);

  //calcula o quanto a roda andou desde a ultima leitura
  int diferenca_M1 = currentRaw_M1 - lastRaw_M1;
  int diferenca_M2 = currentRaw_M2 - lastRaw_M2;

  //corrige o estouro se a roda der uma volta completa (passar pelo 0)
  if (diferenca_M1 > 512) diferenca_M1 -= 1024;
  else if (diferenca_M1 < -512) diferenca_M1 += 1024;

  if (diferenca_M2 > 512) diferenca_M2 -= 1024;
  else if (diferenca_M2 < -512) diferenca_M2 += 1024;

  //acumula o deslocamento na contagem global do encoder
  encoderCount_M1 += diferenca_M1;
  encoderCount_M2 += diferenca_M2;

  lastRaw_M1 = currentRaw_M1;
  lastRaw_M2 = currentRaw_M2;
}

void handle_motor_speed(String s) {
  if (s.length() > 0) {
    int index = s.indexOf(' ');
    int left_motor_speed = s.substring(1, index).toInt();
    int right_motor_speed = s.substring(index + 1).toInt();

    set_speed(left_motor_speed, right_motor_speed);
  }
}

void provide_encoder_values() {
  Serial.print(encoderCount_M1);
  Serial.print(" ");
  Serial.println(encoderCount_M2);
}

void backward() {
  digitalWrite(M1_1, HIGH);
  digitalWrite(M1_2, LOW);
  digitalWrite(M2_1, HIGH);
  digitalWrite(M2_2, LOW);
}

void forward() {
  digitalWrite(M1_1, LOW);
  digitalWrite(M1_2, HIGH);
  digitalWrite(M2_1, LOW);
  digitalWrite(M2_2, HIGH);
}

void stop_motor() {
  analogWrite(M1_en, 0);
  analogWrite(M2_en, 0);
  digitalWrite(M1_1, LOW);
  digitalWrite(M1_2, LOW);
  digitalWrite(M2_1, LOW);
  digitalWrite(M2_2, LOW);
}

void leftward() {
  digitalWrite(M1_1, LOW);
  digitalWrite(M1_2, HIGH);
  digitalWrite(M2_1, HIGH);
  digitalWrite(M2_2, LOW);
}

void rightward() {
  digitalWrite(M1_1, HIGH);
  digitalWrite(M1_2, LOW);
  digitalWrite(M2_1, LOW);
  digitalWrite(M2_2, HIGH);
}

void set_speed(int left_motor_speed, int right_motor_speed) {
  if (left_motor_speed > 0 and right_motor_speed > 0) {
    forward();
  }
  else if (left_motor_speed < 0 and right_motor_speed < 0) {
    backward();
  }
  else if (left_motor_speed > 0 and right_motor_speed < 0) {
    rightward();
  }
  else if (left_motor_speed < 0 and right_motor_speed > 0) {
    leftward();
  }

  int left_motor_speed_ = abs(left_motor_speed);
  int right_motor_speed_ = abs(right_motor_speed);

  analogWrite(M1_en, left_motor_speed_);
  analogWrite(M2_en, right_motor_speed_);
}