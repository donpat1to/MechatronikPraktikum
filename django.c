#include "simpletools.h"
#include "fdserial.h"
#include "abdrive360.h"           
#include "servo.h"      
#include "string.h"

// Variable declaration
// wifi connection
char delimiter = ';';

// protocol parameters
#define BUFFER_MAX 100
int m_direction = 0;
int dir_temp = -1;
int speed = 0;
int gunPos = 0;
int gunTemp = -1;
int jiggl = 0;
/*dart
 *1:fire; 2:dropDart; 3:rotDrum
 */
int dart = 0;
int gunReady = 0;
int dartsFired = 0;


/*** control parameters
 * driving pins(360Servo) [left: pin12(check: pin 14); right: pin13(check: pin 15)]
 */
#define DC_LEFT 0 
#define DC_RIGHT 1
#define SERVO_LIFT 3
#define LED 10
#define SERVO_RELOADING 16
#define SERVO_INSERTION 17


//operating variables
#define MIN_REL 50
#define MIN_INSERT 400
#define MAX_INSERT 1100
#define STEP 490
int angle = 0;
int pos = MIN_REL;


void intToCharArray(int value, char result[], int size) {
    if (size <= 0) {
        return; //Error: size invalid
    }

    //final array filled with zeros
    memset(result, 0, size);

    //check value equals zero
    if (value == 0) {
        result[0] = '0';
        return;
    }

    int index = 0;
    while (value > 0 && index < size - 1) {
        result[index++] = '0' + (value % 10);
        value /= 10;
    }

    //mirroring char array
    int i = 0;
    int j = index - 1;
    while (i < j) {
        char temp = result[i];
        result[i] = result[j];
        result[j] = temp;
        i++;
        j--;
    }
}


int charArrayToInt(char tempo_char[]){
   int num = 0;
   int i = 0;
   int isNegative = 0;
   if (tempo_char[i] == '-'){
      isNegative = 1;
      i++;
   }
   while (tempo_char[i] && (tempo_char[i] >= '0' && tempo_char[i] <= '9')){
      num = num * 10 + (tempo_char[i] - '0');
      i++;
   }                
   if(isNegative) num = -1 * num;

   return num;
}

/*** static sending protocol {[String];[int];[int];[int]}
   * splitting and saving data in received
   */
void stringSeparation(char buffer[BUFFER_MAX])
{
   if (buffer == NULL || delimiter == NULL) {
      return;
   }
   
   //resetting all variables to default
   char tempo_char[30] = "";

   //setting variables current [m_direction; speed; dart; gunPos]
   int counter = 0;
   char* token = strtok(buffer, &delimiter);
   if(token != NULL)
     strcpy(tempo_char, token);    
   m_direction = charArrayToInt(tempo_char);
   strcpy(tempo_char, "");
   
   token = strtok(NULL, &delimiter); 
   if(token != NULL){
     strcpy(tempo_char, token);
   }     
   speed = charArrayToInt(tempo_char);
   strcpy(tempo_char, "");
   
   token = strtok(NULL, &delimiter); 
   if(token != NULL) 
     strcpy(tempo_char, token);
   dart = charArrayToInt(tempo_char);
   strcpy(tempo_char, "");
   
   token = strtok(NULL, &delimiter); 
   if(token != NULL) 
     strcpy(tempo_char, token);
   gunPos = charArrayToInt(tempo_char);
   strcpy(tempo_char, "");
   
   token = strtok(NULL, &delimiter); 
   if(token != NULL) 
     strcpy(tempo_char, token);
   gunReady = charArrayToInt(tempo_char);
   strcpy(tempo_char, "");
   
   token = strtok(NULL, &delimiter); 
   if(token != NULL) 
     strcpy(tempo_char, token);
   jiggl = charArrayToInt(tempo_char);
   strcpy(tempo_char, "");
}


void jiggle(){
   for (int i = 0; i < 10; i++){
      servo_angle(SERVO_RELOADING, pos+50);
      pause(150);
      servo_angle(SERVO_RELOADING, pos-50);
      pause(150);
   }    
   servo_angle(SERVO_RELOADING, pos);  
}   

void rotForward() {
   pos = pos + STEP;
   servo_angle(SERVO_RELOADING, pos);
   pause(300);
   if (dartsFired == 3){
      pos = MIN_REL;
      servo_angle(SERVO_RELOADING, pos);
   }   
}     

int main()
{
   fdserial *board = fdserial_open(31,30,0,115200);
   fdserial *wlan = fdserial_open(8,7,0,115200);
   
   //initializing servos
   servo_angle(SERVO_RELOADING, pos);
   servo_angle(SERVO_INSERTION, MAX_INSERT);
   servo_angle(SERVO_LIFT, 250);
   //pos = MIN_REL;
   
   while(1){
      m_direction = 0;
      speed = 0; 
      dart = 0;
      gunReady = 0;
      jiggl = 0;
      gunTemp = 0;
            
      char buffer[BUFFER_MAX];
      int index = 0;
      
      if(fdserial_rxPeek(wlan) != 0) {     
          char ch = fdserial_rxChar(wlan);
          
          while(ch != '\r') {
             if (ch != -1){
               buffer[index] = ch; 
               index++;
             } 
             ch = fdserial_rxChar(wlan);
          }
                    
          fdserial_rxFlush(wlan);
      }
      
      stringSeparation(buffer);
      speed = speed * 5;
      
      
      if (m_direction != dir_temp){
         if(m_direction == 1) {
            fdserial_txChar(board, '1');
            drive_speed(-speed,-speed);
            dir_temp = 1;
            fdserial_txChar(wlan, '1');
            fdserial_txChar(wlan, '\r');
         } 
         if(m_direction == 2) {
            fdserial_txChar(board, '2');
            dir_temp = 2;
            drive_speed(speed,speed);
            
            fdserial_txChar(wlan, '2');
            fdserial_txChar(wlan, '\r');
         }
         if(m_direction == 3) {
            fdserial_txChar(board, '3');
            dir_temp = 3;
            drive_speed(-speed,speed);
            fdserial_txChar(wlan, '3');
            fdserial_txChar(wlan, '\r');
         }
         if(m_direction == 4) {
            fdserial_txChar(board, '4');
            dir_temp = 4;
            drive_speed(speed ,-speed);
            fdserial_txChar(wlan, '4');
            fdserial_txChar(wlan, '\r'); 
         }
         if(m_direction == 0) {
            fdserial_txChar(board, '0');
            dir_temp = 0;
            drive_speed(0,0);
            fdserial_txChar(wlan, '0');
            fdserial_txChar(wlan, '\r');
         }
      }
      if (dart == 1){
         //DC
         high(0);
         pause(500);
         servo_angle(SERVO_INSERTION, MIN_INSERT);
         pause(500);
         servo_angle(SERVO_INSERTION, MAX_INSERT);
         pause(1000);
         low(0);
         rotForward();
         dartsFired++;
      }
      if (dart == 2) {
         high(LED);
         pause(700);
         low(LED);
      }
      if (dart == 3) {
         rotForward();
      }
      if (gunReady == 1){
         for (int i = 5; i > 0; i--) {
            pause(500);
            pos = MIN_REL * i * 6;
            if (i == 1) {
               pos = MIN_REL;
            }
            servo_angle(SERVO_RELOADING, pos);               
         }         
      }
      int gunTempo = gunPos * 180;
      if (gunTemp != gunPos) {
         servo_angle(SERVO_LIFT, gunTempo);
         gunTemp = gunPos;
      }         
      if (jiggl == 1) {
         jiggle();
      }                                              
   }
   return 0; 
}

