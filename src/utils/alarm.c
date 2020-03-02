#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void sig_alarm() 
{ 
  exit(0); 
} 
int main(int argc, char *argv[]) 
{ 
  signal(SIGALRM, sig_alarm); 
  alarm(10); 
  sleep(5); 
  printf("Hello World!\n"); 
  return 0; 
}