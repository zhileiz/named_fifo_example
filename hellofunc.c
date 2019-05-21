#include <stdio.h>
#include "hellofunc.h"

void myPrintHelloMake(void) {

  #ifdef MASTER
    printf("MASTER: Hello\n");
  #endif

  #ifdef SLAVE
    printf("SLAVE: Hello\n");
  #endif

  return;
}