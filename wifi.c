#include "simpletools.h"
#include "fdserial.h"
#include "abdrive360.h"           
#include "servo.h"      
#include "ping.h"
#include "string.h"

fdserial *board;  
fdserial *wlan;
  
int speed = 0; 
char[128] m_direction;
int fire;
int ready;


void send(char bufferSend[128])
{
  int printIndex = 0;
  bufferSend = "Bye!\r";  
  char chSend = fdserial_rxChar(board);
  if(chSend == '\r') { 
    while(printIndex < sizeof(bufferSend)) {
      fdserial_txChar(wlan, bufferSend[printIndex]); 
      printIndex++; 
    }
    printIndex = 0; 
  }   
}

void order(char message[]){
  if(strcmp(message, "up") == 0) {
    driveForward(); 
  } else if(strcmp(message, "down") == 0) {
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
  send("1\r");
} 

int main()
{
  board = fdserial_open(31,30,0,115200);
  wlan = fdserial_open(8,7,0,115200);
  
  char buffer[128];
  int index = 0;
  int printIndex2 = 0; 
  
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



