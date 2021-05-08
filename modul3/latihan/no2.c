#include <stdlib.h>
#include <unistd.h>
int pid;
int pipe1[2];
int pipe2[2];
void main() {

  // create pipe1
  pipe(pipe1);

  
  pid = fork();
if (pid == 0) 
 exec1();
  // create pipe2
 pipe(pipe2);
 

pid = fork();

 if (pid == 0)
 exec2();

  // close unused fds
  close(pipe1[0]);
  close(pipe1[1]);


pid = fork();
  if (pid == 0) 
 exec3();
  
}


void exec1() {
  // input from stdin (already done)
  // output to pipe1
  dup2(pipe1[1], 1);
 
  close(pipe1[0]);
  close(pipe1[1]);
  
  execlp("ls", "ls", NULL);
}

void exec2() {
  // input from pipe1
  dup2(pipe1[0], 0);
  // output to pipe2
  dup2(pipe2[1], 1);

  close(pipe1[0]);
  close(pipe1[1]);
  close(pipe2[0]);
  close(pipe2[1]);

  execlp("head", "head", "-n","3", NULL);

}

void exec3() {
  // input from pipe2
  dup2(pipe2[0], 0);
  // output to stdout (already done)

  close(pipe2[0]);
  close(pipe2[1]);

  execlp("tail", "tail","-n", "1", NULL);
}
