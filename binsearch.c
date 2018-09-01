#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/wait.h>  /* for wait */
#include <sys/un.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "const.h"
#include "util.h"
#include <limits.h>
#include <pthread.h>

typedef struct{
  unsigned int *arr;
  int l, r ,x;
}data;

int serial_binsearch(int x, int val[], int n) {
    int low = 0, high = n-1, mid;

    while (low < high){
        mid = (low+high)/2;

        if (x < val[mid])
            high = mid - 1;
        else if (x > val[mid])
            low = mid + 1;
        else
            return mid;
    }
    return -1;
}

// TODO: implement
int parallel_binsearch() {
  data *info = args;
  int low;
  int mid;
  int high;
  int x
  x = info->x;
  low = info->1;
  job_waiting++;
  finish = 0;
  while(low < high && !found){
    mid = (low + high) /2;
    if(x < mid)(high = mid - 1);
    else if (x > mid)(low = mid + 1);
    else{
      finish = 1;
      pthread_exit(0);
    }
  }
  return 0;
}

int main(int argc, char** argv) {
    pthread_t thid;
    fprintf(stderr, "Acaaaaa\n");//aaaaaaaaaaaaaaaaaaaaaa
    /* TODO: move this time measurement to right before the execution of each binsearch algorithms
     * in your experiment code. It now stands here just for demonstrating time measurement. */
    clock_t cbegin = clock();

    printf("[binsearch] Starting up...\n");

    /* Get the number of CPU cores available */
    printf("[binsearch] Number of cores available: '%ld'\n",
           sysconf(_SC_NPROCESSORS_ONLN));

    /* TODO: parse arguments with getopt */
    char *Pflg, *Eflg, *Tflg;
    int T, c;
    while ((c = getopt (argc, argv, "T:E:P:")) != -1)
      switch (c){
        case 'E':
			Eflg = optarg;
			break;
        case 'P':
			Pflg = optarg;
			break;
        case 'T':
			Tflg = optarg;
			break;
    }
    printf("E flag: %s T flag: %s and P flag: %s\n", Eflg, Tflg, Pflg);
    T = atoi(Tflg);

    /* TODO: start datagen here as a child process. */
	pid_t pid;
    pid =fork();
    char* datagen_file[] = {"./datagen", NULL};

    if (pid < 0) {
      perror("Fork status");
      return(-1);
    }

    if (pid==0) { /* child process */
        execlp("./datagen", "./datagen", NULL);
    }
    /*BUFFERS*/
    char buf[10];
    int data_buf[1000];

    struct sockaddr_un addr;
    int fd,rc;
    strcpy(buf, "BEGIN S");
    char n;
    fprintf(n, T);
    strcat(buf, n);

    if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
      perror("socket error");
      exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, DSOCKET_PATH, sizeof(addr.sun_path)-1);

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
      perror("connect error");
      exit(-1);
    }

    if ((rc = write(fd, buf, sizeof(buf)))==-1){
        perror("write error\n");
        exit(-1);
    }

    //leer desde el socket
    int largo = pow(10, T);
    int data =

    /* TODO: implement code for your experiments using data provided by datagen and your
     * serial and parallel versions of binsearch.
     * */

    /* TODO: connect to datagen and ask for the necessary data in each experiment round.
     * Create a Unix domain socket with DSOCKET_PATH (see const.h).
     * Talk to datagen using the messages specified in the assignment description document.
     * Read the values generated by datagen from the socket and use them to run your
     * experiments
     * */


    //read();

    long unsigned int values = 0;
    size_t numval = pow(10, T);
    size_t reader = 0;

    UINT*nums = malloc(sizeof(UINT) * numval);
    while(reader < numval){
      reader = read(fd, numval + reader, sizeof(UINT)*1000);
      reader += reader / 4;
    }

    if (listen(fd, 5) == -1) {
      perror("listen error");
      exit(-1);
    }

    /* Probe time elapsed. */
    clock_t cend = clock();

    // Time elapsed in miliseconds.
    double time_elapsed = ((double) (cend - cbegin) / CLOCKS_PER_SEC) * 1000;

    printf("Time elapsed '%lf' [ms].\n", time_elapsed);

    exit(0);
}
