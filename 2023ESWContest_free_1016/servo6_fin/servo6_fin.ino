#include <Servo.h> // 서보모터를 다루기 위한 헤더파일

// 서보모터의 개수
int const numServo = 6; 

// 서보모터를 배열로 선언
Servo servos[numServo];

// Serial로 Read한 문자를 1개씩 저장할 변수
byte inByte;
// Serial 통신의 마지막 입력을 저장한다 -> Serial 통신의 맨 마지막에 항상 garbage값이 들어오기 때문
byte inByte_p = 32;

// Read된 문자에 해당하는 점자 data의 각 자리가 0인지 1인지 판별하기 위한 변수 선언 (초기값 = 0000001) 
byte mask = 1;

// ASCII - 점자 data 변환 배열 선언
byte myBrailleDots[255];

// ASCII - 점자 data 변환 배열 저장 함수
void make_brailleDots(){
  // 배열의 초기값을 99로 설정하여 나중에 사용하지 않는 부분을 검출
  for (int i = 0; i < 256; i =   i + 1) {
     myBrailleDots[i] = 99;
  }
  
  // 문자의 해당 ASCII번호와 점자 data를 저장하는 배열
  // 영문자는 대소문자를 구분하지 않았다.
  myBrailleDots[32] = 0;  // blank is 000000
  myBrailleDots[33]   = 14;  // exlamation mark is 001110
  myBrailleDots[34] = 7;  // double quote   is 000111
  myBrailleDots[34] = 2;  // single quote is 000010
  myBrailleDots[40]   = 15;  // left parenthesis is 001111
  myBrailleDots[41] = 15;  // right parenthesis   is 001111
  myBrailleDots[44] = 8;  // comma is 001000
  myBrailleDots[46]   = 13;  // period is 001101
  myBrailleDots[48] = 28; // 0 is 011100
  myBrailleDots[49]   = 32; // 1 is 100000
  myBrailleDots[50] = 40; // 2 is 101000
  myBrailleDots[51]   = 48; // 3 is 110000
  myBrailleDots[52] = 52; // 4 is 110100
  myBrailleDots[53]   = 36; // 5 is 100100
  myBrailleDots[54] = 56; // 6 is 111000
  myBrailleDots[55]   = 60; // 7 is 111100
  myBrailleDots[56] = 44; // 8 is 101100
  myBrailleDots[58] = 12; // colon is 001100
  myBrailleDots[59]   = 10; // semicolon is 001010
  myBrailleDots[63] = 11; // question mark is 001011
   myBrailleDots[65] = 32; // A is 100000
  myBrailleDots[66] = 40; // B is 101000
   myBrailleDots[67] = 48; // C is 110000
  myBrailleDots[68] = 52; // D is 110100
   myBrailleDots[69] = 36; // E is 100100
  myBrailleDots[70] = 56; // F is 111000
   myBrailleDots[71] = 60; // G is 111100
  myBrailleDots[72] = 44; // H is 101100
   myBrailleDots[73] = 24; // I is 011000
  myBrailleDots[74] = 28; // J is 011100
   myBrailleDots[75] = 34; // K is 100010
  myBrailleDots[76] = 42; // L is 101010
   myBrailleDots[77] = 50; // M is 110010
  myBrailleDots[78] = 54; // N is 110110
   myBrailleDots[79] = 38; // O is 100110
  myBrailleDots[80] = 58; // P is 111010
   myBrailleDots[81] = 62; // Q is 111110
  myBrailleDots[82] = 46; // R is 101110
   myBrailleDots[83] = 26; // S is 011010
  myBrailleDots[84] = 30; // T is 011110
   myBrailleDots[85] = 35; // U is 100011
  myBrailleDots[86] = 43; // V is 101011
   myBrailleDots[87] = 29; // W is 011101
  myBrailleDots[88] = 51; // X is 110011
   myBrailleDots[89] = 55; // Y is 110111
  myBrailleDots[90] = 39; // Z is 100111
   myBrailleDots[97] = 32; // A is 100000
  myBrailleDots[98] = 40; // B is 101000
   myBrailleDots[99] = 48; // C is 110000
  myBrailleDots[100] = 52; // D is 110100
   myBrailleDots[101] = 36; // E is 100100
  myBrailleDots[102] = 56; // F is   111000
  myBrailleDots[103] = 60; // G is 111100
  myBrailleDots[104] = 44;   // H is 101100
  myBrailleDots[105] = 24; // I is 011000
  myBrailleDots[106]   = 28; // J is 011100
  myBrailleDots[107] = 34; // K is 100010
  myBrailleDots[108]   = 42; // L is 101010
  myBrailleDots[109] = 50; // M is 110010
  myBrailleDots[110]   = 54; // N is 110110
  myBrailleDots[111] = 38; // O is 100110
  myBrailleDots[112]   = 58; // P is 111010
  myBrailleDots[113] = 62; // Q is 111110
  myBrailleDots[114]   = 46; // R is 101110
  myBrailleDots[115] = 26; // S is 011010
  myBrailleDots[116]   = 30; // T is 011110
  myBrailleDots[117] = 35; // U is 100011
  myBrailleDots[118]   = 43; // V is 101011
  myBrailleDots[119] = 29; // W is 011101
  myBrailleDots[120]   = 51; // X is 110011
  myBrailleDots[121] = 55; // Y is 110111
  myBrailleDots[122]   = 39; // Z is 100111
}

// servo motor가 시작시 "S" 문자를 출력한다
void setting_servo(){
  // 서보모터 출력핀 설정
  // 각 자리의 서보모터 마다 올라갔을때의 각도와 내려왔을때의 각도가 다르다.
  
  //servos[0].attach(2); // servos[0] == Right Low , 90degree(dowm) - 45degree(up)
  servos[1].attach(3); // servos[1] == Left Low , 90degree(dowm) - 135degree(up)
  servos[2].attach(4); // servos[2] == Right Middle , 90degree(dowm) - 45degree(up)
  servos[3].attach(5); // servos[3] == Left Middle , 90degree(dowm) - 135degree(up)
  //servos[4].attach(6); // servos[4] == Right High , 90degree(dowm) - 135degree(up)
  servos[5].attach(7); // servos[0] == Left High , 90degree(dowm) - 45degree(up)

  //servos[0].write(45); 
  servos[1].write(135);
  servos[2].write(45);
  servos[3].write(135);
  //servos[4].write(135);
  servos[5].write(45);

  delay(1000);

  //servos[0].write(90);
  servos[1].write(90);
  servos[2].write(90);
  servos[3].write(90);
  //servos[4].write(90);
  servos[5].write(90);

  delay(1000);

  //servos[0].detach();
  servos[1].detach();
  servos[2].detach();
  servos[3].detach();
  //servos[4].detach();
  servos[5].detach();
}

// 입력이 발생하였을 때 어떤 문자가 입력이 되었는지, 입력된 문자가 어떤 점자 data를 가지고 있는지 print하는 함수
void read_print_data(){
  // serial로 들어온 Byte를 읽는다 (한 글자씩 읽는다)
  inByte = Serial.read();
  // 어떤 문자가 들어왔는지 그 문자에 해당하는 ASCII번호를 출력
  Serial.print("Received   (inByte): ");
  Serial.println(inByte);
  Serial.print("Received   (inByte_p): ");
  Serial.println(inByte_p);
  // 그 번호에 해당하는 점자 data 출력
  Serial.print("Matrix points variable (myBrailleDots[inByte]):   ");
  Serial.println(myBrailleDots[inByte]); 
}

// 입력의 마지막 문자를 저장하는 함수
void save_last_message(){
  inByte = inByte_p;
  Serial.print("Renew   (inByte): ");
  Serial.println(inByte);
}

// 입력된 문자가 점자 data로 변환이 불가한 경우 문장을 print하는 함수
void not_translatable_message(){
  Serial.println("Not   a translatable character");
}

// 점자 data의 출력을 위한 함수
void servo_control(){
  // 출력할 핀 (오른쪽 아래의 점자)
  int thisPin = 2;
  // 서보모터의 전력소모를 줄이기 위하여 연속적인 data입력시 이전의 data와 비교하여 변화가 있는
  // 서보모터만 attach하여 각도를 제어
  for (mask   = 000001; mask<64; mask <<= 1) { 
    Serial.print("thisPin =   ");
    Serial.println(thisPin); 
    // 이전값과 비교하여 DOWN -> UP으로 바뀌는 부분
    if (((myBrailleDots[inByte] ^ myBrailleDots[inByte_p]) & (myBrailleDots[inByte])) & mask){ 
      Serial.print("AND   successful, put pin on!");
      Serial.println(mask);  
      // 각 핀마다 올라와야하는 각도가 다르므로 if문을 이용하여 각 핀의 출력각도를 제어해 주었다.
      if(thisPin == 2){
        servos[0].attach(2);
        servos[0].write(45);               
      }
      else if(thisPin == 3){
        servos[1].attach(3);
        servos[1].write(135);
      }
      else if(thisPin == 4){
        servos[2].attach(4);
        servos[2].write(45);
      }
      else if(thisPin == 5){
        servos[3].attach(5);
        servos[3].write(135);
      }
      else if(thisPin == 6){
        servos[4].attach(6);
        servos[4].write(135);
      }
      else if(thisPin == 7){
        servos[5].attach(7);
        servos[5].write(45);
      }
    }
    // 이전값과 비교하여 UP -> DOWN으로 바뀌는 부분
    else if(((myBrailleDots[inByte] ^ myBrailleDots[inByte_p]) & (myBrailleDots[inByte_p])) & mask){                     
      Serial.print("AND   unsuccessful, put pin off!");
      Serial.println(mask);  
      if(thisPin == 2){
        servos[0].attach(2);
        servos[0].write(90);               
      }
      else if(thisPin == 3){
        servos[1].attach(3);
        servos[1].write(90);
      }
      else if(thisPin == 4){
        servos[2].attach(4);
        servos[2].write(90);
      }
      else if(thisPin == 5){
        servos[3].attach(5);
        servos[3].write(90);
      }
      else if(thisPin == 6){
        servos[4].attach(6);
        servos[4].write(90);
      }
      else if(thisPin == 7){
        servos[5].attach(7);
        servos[5].write(90);
      }
    }
    thisPin   = thisPin + 1;   
  }
}

// 문자 1개를 출력 후 전부 detach하여 전류를 아낀다
void save_detach(){
   // 이전값과 비교를 위한 이전값을 저장해주는 변수 inByte_p
   inByte_p = inByte;
   // 한 문자를 출력하고 난 다음 전부 detach()를 해주어 전류소모를 아끼는 부분
   servos[0].detach();
   servos[1].detach();
   servos[2].detach();
   servos[3].detach();
   servos[4].detach();
   servos[5].detach();
}

void setup() {
  make_brailleDots();
  
  // baudrate 설정
  Serial.begin(9600);

  // 통신이 시작되었는지를 test해보기 위해 serial monitor에 출력
  Serial.println("ASCII   - Braille Arduino Converter");
  Serial.println("Servo test - begin");

   setting_servo();

  //마찬가지로 serial monitor에 test가 끝났다는 것을 알리는 출력 문구이다.
  Serial.println("Servo test - end");
  Serial.println("Type   some character: it will be transmitted to Arduino and displayed on a Braille 2 x   3 matrix");
}


void loop() {
   // 점자 출력은 점자 data를 받았을 때에만 실행된다
   if (Serial.available() > 0) {
      read_print_data(); 

      // 입력의 마지막 부분을 저장
      if (inByte == 10){
        save_last_message();
      }
      // 만약 저장하지 않은 문자가 나온다면 점자 data는 99로 저장되어 있을 것이고 저장하지 않았으므로 출력하지 않는다
      else if   (myBrailleDots[inByte] == 99){
         not_translatable_message();
      }

      // 저장된 문자가 나왔을 시
      else{
        servo_control();
      }
      delay(500); // 1개의 문자당 1초의 delay를 갖는다.
   }
   save_detach();
} 
