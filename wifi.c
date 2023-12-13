
#include "simpletools.h"
#include "fdserial.h"
#include "abdrive360.h"           
#include "servo.h"      
#include "ping.h"
#include "string.h"

 fdserial *board;
  fdserial *wlan;
  
  int speed = 0; 
  
void sending()
{
  int printIndex = 0;
  char bufferSend[128] = "Bye!\r";  
  while(1){
    char chSend = fdserial_rxChar(board);
      if(chSend == '\r') { 
        while(printIndex < 5) {
          fdserial_txChar(wlan, bufferSend[printIndex]); 
          printIndex++; 
        }
        printIndex = 0; 
      }
  }     
}

void order(char message[]){
  //printf();
  if(strcmp(message, "up") == 0) {
    driveForward(); 
  }  
  else if(strcmp(message, "down") == 0) {
    driveBackwards(); 
  } else if(strcmp(message, "right") == 0) {
    driveRight(); 
  } else if(strcmp(message, "left") == 0) {
    driveLeft(); 
  } else {
    drive_speed(0,0);
  }    
}

void driveForward() {
  drive_speed(15,15);
} 

void driveRight() {
  drive_speed(15,-15);
} 
void driveLeft() {
  drive_speed(-15,15);
} 

void driveBackwards() {
  drive_speed(-15,-15);
} 

int main()
{
  board = fdserial_open(31,30,0,115200);
  wlan = fdserial_open(8,7,0,115200);
  
  char buffer[128];
  int index = 0;
  int printIndex2 = 0; 
  
  int* cog1 = cog_run(sending, 100); 
  
  while(1)
  {
     high(26);
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



