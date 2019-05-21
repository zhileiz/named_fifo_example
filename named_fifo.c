#include "hellofunc.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>


int main() {

  char * child_to_parent_pipe = "/tmp/child_to_parent_pipe";
  char * parent_to_child_pipe = "/tmp/parent_to_child_pipe";

  int fd_out = -1;
  int fd_in = -1;

    /**
     * According to Linux ManPage:
     * When opening a FIFO with O_RDONLY or O_WRONLY set:
     * If O_NONBLOCK is set, 
     *    an open() for reading-only shall return without delay, 
     *    an open() for writing-only shall return an error if no process currently has the file open for reading.
     * If O_NONBLOCK is clear, 
     *    an open() for reading-only shall block the calling thread until a thread opens the file for writing. 
     *    An open() for writing-only shall block the calling thread until a thread opens the file for reading.
     */ 

  /* MARK: SLAVE */
  #ifdef SLAVE
    /* open pipe as file descriptors */
    myPrintHelloMake();
    if ((fd_out = open(child_to_parent_pipe, O_WRONLY | O_NONBLOCK)) < 0 ) { perror("SLAVE ERROR open out"); }
    if ((fd_in = open(parent_to_child_pipe, O_RDONLY | O_NONBLOCK)) < 0 ) { perror("SLAVE ERROR open in"); }

    /* write content to master*/
    write(fd_out, "Message From Slave~~", sizeof("Message From Slave~~"));
    
    char buf[1024];
    while (1) {
      /* reset buffer */
      memset(buf, 0, sizeof buf);
      if (read(fd_in, buf, 1024) < 0 || strlen(buf) < 1) { continue; } 
      else {
        /* print result */
        printf("Slave Received: %s.\n", buf);
        /* exit on 'ACK' */
        if (strcmp(buf, "ACK") == 0) {
          printf("[SLAVE EXIT]\n");
          write(fd_out, "EOS", sizeof("EOS"));
          exit(1);
        }
      }
    };
  #endif
  
  /* MARK: MASTER */
  #ifdef MASTER
    /* prepare */
    myPrintHelloMake();
    unlink(child_to_parent_pipe);
    unlink(parent_to_child_pipe);

    /* make pipe */
    if (mkfifo(child_to_parent_pipe, 0666) < 0) { perror("Master ERROR mkfifo 1"); }
    if (mkfifo(parent_to_child_pipe, 0666) < 0) { perror("Master ERROR mkfifo 1"); }

    /* Can only open read at this point, see above ManPage Explanation */
    if ((fd_in = open(child_to_parent_pipe, O_RDONLY | O_NONBLOCK)) < 0 ) { perror("Master ERROR open in"); }

    char buf[1024];
    while (1) {
      /* reset buffer */
      memset(buf, 0, sizeof buf);
      if (read(fd_in, buf, 1024) < 0 || strlen(buf) < 1) { continue;} 
      else {
        printf("Master Received: %s.\n", buf);
        /* Exit on receiving EOS */
        if (strcmp(buf, "EOS") == 0) {
          printf("[MASTER EXIT]\n");
          exit(1);
        }
        /* Open out file descriptor */
        if (fd_out < 0) {
          if ((fd_out = open(parent_to_child_pipe, O_WRONLY | O_NONBLOCK)) < 0 ) { perror("Master ERROR open out"); }
        }
        write(fd_out, "ACK", sizeof("ACK"));
      }
    };
  #endif

  return(0);
}