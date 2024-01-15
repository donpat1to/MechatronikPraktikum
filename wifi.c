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
char* m_direction;
int speed = 0; 
int fire;
int gunPos;
char** received;

// control parameters
/*
int dc_left = 
int dc_right =
int 360servo_left = 
int 360servo_right = 
int servo_reloading = 
int servo_insertion = 
int servo_lift = 
*/

void send(char bufferSend[128])
{
   int printIndex = 0;
   //bufferSend = "Bye!\r";  
   char chSend = fdserial_rxChar(board);
   if(chSend == '\r') { 
      while(printIndex < sizeof(bufferSend)) {
         fdserial_txChar(wlan, bufferSend[printIndex]); 
         printIndex++; 
      }
      printIndex = 0; 
   }   
}

/*** static sending protocol {[String];[int];[int];[int]}
   * splitting and saving data in received
   */
void stringSeparation(char* buffer, char* delimiter)
{
   int counter = 0;
   char* token;
   
   while(token != NULL) {
      token = strtok(buffer, delimiter);
      if (counter == 0)
         strcpy(m_direction, token);
         printf("Direction: %s",m_direction);
      if (counter == 2)
         speed = atoi(token);
      if (counter == 3)
         fire = atoi(token);
      if (counter == 4)
         gunPos = atoi(token);
      counter++;
   }
}

void order(char message[]){
   if(strcmp(message, "w") == 0) {
      driveForward(); 
   } else if(strcmp(message, "a") == 0) {
      driveBackwards(); 
   } else if(strcmp(message, "s") == 0) {
      driveRight(); 
   } else if(strcmp(message, "d") == 0) {
      driveLeft(); 
   } else {
      drive_speed(0,0);
   }    
}
  
void driveForward() {
   drive_speed(15,15);
   send("1\r");
} 

void driveRight() {
   drive_speed(15,-15);
   send("1\r");
} 
void driveLeft() {
   drive_speed(-15,15);
   send("1\r");
} 

void driveBackwards() {
   drive_speed(-15,-15);
   send("1\r");
} 

int main()
{
   board = fdserial_open(31,30,0,115200);
   wlan = fdserial_open(8,7,0,115200);
  
   char buffer[128];
   int index = 0;
   int printIndex2 = 0; 
  
   while(1) {
      high(26);
      printf("hanjo");
      char ch = fdserial_rxChar(wlan);  

      if(ch == '\r') {
         printIndex2 = 0; 
         while(printIndex2 < index) {
            fdserial_txChar(board, buffer[printIndex2]); 
            printIndex2++; 
         }

         order(buffer); 
         memset(buffer, 0, sizeof(buffer)); 

         printIndex2 = 0;
         index = 0;
      } 
         
      else if (ch != -1){
        buffer[index] = ch;
         index++;       
      }    
          
   }  
   fdserial_close(board); 
   fdserial_close(wlan);
   return 0; 
}