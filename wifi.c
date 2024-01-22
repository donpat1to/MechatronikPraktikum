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
int botBusy = 0;


char* m_direction;
//m_direction = (char *) malloc(3 * sizeof(char));
int speed = 0; 
int fire = 0;
int gunPos = 0;

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


int main()
{
   board = fdserial_open(30,31,0,115200);
   wlan = fdserial_open(8,7,0,115200);
   
   
   char buffer[128];
   
   int index = 0;
   int printIndex2 = 0; 
  
   while(1) {
      space(10);
      char ch = fdserial_rxChar(wlan);  

      if(ch == '\r') {
         //buffer = calloc(100 ,sizeof(char));
         //printf("hanjo\n");
         printIndex2 = 0; 
         while(printIndex2 < index) {
            fdserial_txChar(board, buffer[printIndex2]); 
            printIndex2++; 
         }
         printf("Received: %s\n", buffer);
         stringSeparation(buffer, ";");
         //if (botBusy == 0) 
         order();
         memset(buffer, 0, sizeof(buffer)); 
         //send("1\r");
         //printf("send aufgerufen worden\n");
         printIndex2 = 0;
         index = 0;
         //free(buffer);
      } 
         
      else if (ch != -1){
         buffer[index] = ch;
         index++;
         //printf("bruder muss lost%c\n",buffer[index]);
      }    
          
   }
   fdserial_close(board); 
   fdserial_close(wlan);
   
   free(m_direction);
   return 0; 
}

void send(char bufferSend[128])
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
}


/*** static sending protocol {[String];[int];[int];[int]}
   * splitting and saving data in received
   */
void stringSeparation(char* buffer, char* delimiter)
{
   m_direction = calloc(5 ,sizeof(char));
   if (buffer == NULL || delimiter == NULL) {
      printf("Error: NULL pointer detected.\n");
      return;
   }
   //printf("in stringSeparation drin\n");
   int counter = 0;
   char* token;
   token = strtok(buffer, delimiter);
   
   while(counter < 4 && token != NULL) {
      if (counter == 0)
         m_direction = token;
      if (counter == 1)
         speed = atoi(token);
      if (counter == 2)
         fire = atoi(token);
      if (counter == 3)
         gunPos = atoi(token);
      counter++;
      token = strtok(NULL, delimiter);
   }
   
   printf("Direction: %c\n",m_direction[0]);
   printf("Speed: %d\n",speed);
   printf("Fire: %d\n",fire);
   printf("Gun Position: %d\n",gunPos);
   free(m_direction);
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
      //drive_speed(0,0);
      //send("stopping\r");
   }
   //printf("nix mit order gemacht\n");   
}

void space(int n){
   for (int temp = 0; temp < n; temp++)
      printf("\n");
}   

void driveForward() {
   //drive_speed(speed,speed);
   //printf("drive_speed: %d\n", speed);
   send("forw\r");
   //printf("send aufgerufen\n");
} 

void driveRight() {
   //drive_speed(speed,-speed);
   send("right\r");
} 
void driveLeft() {
   //drive_speed(-speed,speed);
   send("left\r");
} 

void driveBackwards() {
   //drive_speed(-speed,-speed);
   send("back\r");
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

