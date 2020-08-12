#define _POSIX_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>


#include <errno.h>

/**
 * Name...: alarm.c
 * Author.: not Xanadrel
 * Desc...: Stops a cmd ran through it if the time exceeds <secs>
 * License.: Unknown
 */

// gcc -Wall -O3 -o alarm alarm.c

// global
pid_t pid = -1;

void kill_child() {
  printf("Killed process\n");

  kill(pid, SIGKILL);

  exit(142);
}

int main(int argc, char *argv[]) {
  if(argc < 3) {
    printf("Usage %s <secs> <cmd>\n", argv[0]);
    exit(-1);
  }

  pid = fork();

  // error
  if (pid < 0) {

    fprintf(stderr, "can't fork, error %d\n", errno);

    exit(EXIT_FAILURE);

  // child
  } else if (pid == 0){

    //sleep(5);

    char **newargv = argv + 2;

    execvp(newargv[0], newargv);

    _exit(0);

  // parent
  } else {

    signal(SIGALRM, (void (*)(int))kill_child);

    struct itimerval old, new;

    new.it_interval.tv_usec = 0;
    new.it_interval.tv_sec = 0;
    new.it_value.tv_usec = 0;
    new.it_value.tv_sec = (unsigned int) atoi(argv[1]);

    setitimer (ITIMER_REAL, &new, &old);

    wait(NULL);
  }

  exit(0);
}