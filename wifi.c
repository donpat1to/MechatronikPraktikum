#include "simpletools.h"
#include "fdserial.h"
#include "abdrive360.h"           
#include "servo.h"      
#include "ping.h"
#include "string.h"

// Variable declaration
// wifi connection 
fdserial *board;  
fdserial *wlan;

// protocol parameters
#define BUFFER_MAX 30
int botBusy = 0;
char m_direction[3];
int speed = 0; 
int fire = 0;
int gunPos = 0;

/*** control parameters
 * driving pins(360Servo) [left: pin12(check: pin 14); right: pin13(check: pin 15)]
 */
#define DC_LEFT 0 
#define DC_RIGHT 1
#define SERVO_LIFT 2
#define SERVO_RELOADING 16
#define SERVO_INSERTION 17

//operating variables
#define MAX_REL 900
int angle = 0;


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
   num = 0;
   i = 0;
   isNegative = 0;
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

int main()
{
   board = fdserial_open(30,31,0,115200);
   wlan = fdserial_open(8,7,0,115200);
   
   
   char buffer[BUFFER_MAX];
   
   int index = 0;
   //int printIndex2 = 0; 
  
   /*while(1) {
      char ch = fdserial_rxChar(wlan);  

      if(ch == '\r') {
         printIndex2 = 0; 
         while(printIndex2 < index) {
            fdserial_txChar(board, buffer[printIndex2]); 
            printIndex2++; 
         }
         printf("Received: %s\n", buffer);
         stringSeparation(buffer, ";"); 
         order();
         memset(buffer, 0, sizeof(buffer)); 
         printIndex2 = 0;
         index = 0;
      } 
         
      else if (ch != -1){
         buffer[index] = ch;
         index++;
      }    
          
   }*/
   while(1){
      if(fdserial_rxPeek(wlan) != 0) {     
          char ch = fdserial_rxChar(wlan);
          
          while(ch != '\r') {
             if (ch != -1){
               buffer[index] = ch; 
               index++;
             } 
             ch = fdserial_rxChar(wlan);
          }
          order(buffer);           
          fdserial_rxFlush(wlan);
      }      
   }
   
   fdserial_close(board); 
   fdserial_close(wlan);
   
   free(m_direction);
   return 0; 
}

/*
void send(char bufferSend[BUFFER_MAX])
{
   //printf("in send drin\n");
   int printIndex = 0;
   while(bufferSend[printIndex] != '\r') {
      fdserial_txChar(wlan, bufferSend[printIndex]);
      //printf("while schleife senden: %c\n", bufferSend[printIndex]); 
      printIndex++; 
   }
   fdserial_txChar(wlan, '\r');
   botBusy = 0;
   //printIndex = 0; 
}*/


/*** static sending protocol {[String];[int];[int];[int]}
   * splitting and saving data in received
   */
void stringSeparation(char buffer[BUFFER_MAX])
{
   char delimiter = ";";
   if (buffer == NULL || delimiter == NULL) {
      printf("Error: NULL pointer detected.\n");
      return;
   }
   
   //resetting all variables to default
   m_direction = "";
   speed = 0; 
   fire = 0;
   gunPos = 0;

   //setting variables current [m_direction; speed; fire; gunPos]
   int counter = 0;
   char* token;
   token = strtok(buffer, delimiter);
   strcpy(m_direction, token);
   
   token = strtok(NULL, &delimiter); 
   if(token != NULL) 
     strcpy(tempo_char, token);
   speed = charArrayToInt(tempo_char)
   tempo_char = "";
   
   token = strtok(NULL, &delimiter); 
   if(token != NULL) 
     strcpy(tempo_char, token);
   fire = charArrayToInt(tempo_char)
   tempo_char = "";
   
   token = strtok(NULL, &delimiter); 
   if(token != NULL) 
     strcpy(tempo_char, token);
   gunPos = charArrayToInt(tempo_char)
}

void order(){
   //printf("in order drin\n");
   if(strcmp(m_direction, "w") == 0) {
      botBusy = 1;
      //printf("driving forward\n");
      driveForward(); 
   } 
   //printf("m_direction ist w: %d\n", strcmp(m_direction, "w"));
   if(strcmp(m_direction, "a") == 0) {
      botBusy = 1;
      //printf("driving left\n");
      driveLeft();
   }
   //printf("m_direction ist a: %d\n", strcmp(m_direction, "a"));
   if(strcmp(m_direction, "s") == 0) {
      botBusy = 1;
      //printf("driving backward\n");
      driveBackwards(); 
   }
   if(strcmp(m_direction, "d") == 0) {
      botBusy = 1;
      //printf("driving right\n");
      driveRight(); 
   }
   if(strcmp(m_direction, "0") == 0) {
      botBusy = 0;
   } 
}  

void driveForward() {
   drive_speed(speed,speed);
   fdserial_txChar(wlan, '1');
   fdserial_txChar(wlan, '\r');
} 

void driveRight() {
   drive_speed(speed,-speed);
   fdserial_txChar(wlan, '1');
   fdserial_txChar(wlan, '\r');
} 
void driveLeft() {
   drive_speed(-speed,speed);
   fdserial_txChar(wlan, '1');
   fdserial_txChar(wlan, '\r');
} 

void driveBackwards() {
   drive_speed(-speed,-speed);
   fdserial_txChar(wlan, '1');
   fdserial_txChar(wlan, '\r');
}

void reload(){
   //turn
   //
}

void moveToFirstPosition(){
   //
   //
}

void loadDartRotation() {
   //blink
   //wait
   //turn
   //wait
}      


