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

int finish;
int arr[10];
bool fnd = false;
int found;
int job_waiting = 0;
int max;
int min;
int xx;
int n = sizeof (arr);
int max_threads;
int c = 0;
int position;
int size;

typedef struct{
  unsigned int *arr;
  int l, h ,x;
}data;

int serial_binsearch(unsigned int val[], int x, int n) {
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
int *binsearch(void *args) {
  data *info = args;
  int low;
  int mid;
  int high;
  int x;
  x = info->x;
  low = info->l;
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
void parallel_binsearch(unsigned int arr[], int l, int h, int x){
  pthread_t threads[max_threads];
  int thread_p = h / max_threads;
  int counter = 1;

  for(int i = 0; i < max_threads; i++){
    data *info = malloc(sizeof(data));
    info->arr = arr;
    info->l = l;
    info->x = position;
    info->h = (thread_p * counter) - 1;
    if (pthread_create(&threads[i], NULL, (void *)binsearch, info))
    {
        free(info);
    }
    l = thread_p * counter;
    counter++;
  }

}

int main(int argc, char** argv) {
    pthread_t thid;
    /* TODO: move this time measurement to right before the execution of each binsearch algorithms
     * in your experiment code. It now stands here just for demonstrating time measurement. */
    clock_t cbegin = clock();

    printf("[binsearch] Starting up...\n");

    /* Get the number of CPU cores available */
    printf("[binsearch] Number of cores available: '%ld'\n",
           sysconf(_SC_NPROCESSORS_ONLN));

    /* TODO: parse arguments with getopt */
    char *Pflg, *Eflg, *Tflg;
    int T, E, P, c;
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
    E = atoi(Eflg);
    P = atoi(Pflg);

    /* TODO: start datagen here as a child process. */
	pid_t pid;
    pid =fork();

    if (pid < 0) {
      perror("Fork status");
      return(-1);
    }

    if (pid==0) { /* child process */
        execlp("./datagen", "./datagen", NULL);
    }
    /*BUFFERS*/
    char buf[15];
    unsigned int data_buf[1000];

    struct sockaddr_un addr;
    int fd,rc;
    strcpy(buf, "BEGIN S");
    strcat(buf, Tflg);

    if ( (fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
      perror("socket error");
      exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, DSOCKET_PATH, sizeof(addr.sun_path)-1);

    while ( connect(fd, (struct sockaddr*)&addr, sizeof(addr)) == -1){
        perror("[binsearch] connect error");
    }

    if ( connect(fd, (struct sockaddr*)&addr, sizeof(addr)) != -1){
        printf("[binsearch] connected");
    }

    //leer desde el socket
    int token = 0;
    int i;

    while((rc = read(fd, data_buf, sizeof(data_buf))) >= 1000){
        if (token == 0){
            for (i = 0; i < 1000; i++){
                data_buf[i + token*1000] = data_buf[i + 1];
            }
        }
        if (token > 0){
            for (i = 0; i < 1000; i++){
                data_buf[i + token*1000] = data_buf[i];
            }
        }
        token++;
    }

    /* TODO: implement code for your experiments using data provided by datagen and your
     * serial and parallel versions of binsearch.
     * */

    struct timespec start, finish;
	double elapsed1 = 0;
	double elapsed2 = 0;
    int cantidad_valores = pow(10, T);
    for (i=0; i < E; i++){
        clock_gettime(CLOCK_MONOTONIC, &start);
		serial_binsearch(data_buf, P, pow(10,T));
		clock_gettime(CLOCK_MONOTONIC, &finish);
		elapsed1 = (finish.tv_sec - start.tv_sec);
		elapsed1 += (finish.tv_nsec - start.tv_nsec) / 1000000.0;

		clock_gettime(CLOCK_MONOTONIC, &start);
		parallel_binsearch(data_buf, 0, cantidad_valores, P);
		clock_gettime(CLOCK_MONOTONIC, &finish);
		elapsed2 = (finish.tv_sec - start.tv_sec);
		elapsed2 += (finish.tv_nsec - start.tv_nsec) / 1000000.0;
		printf("%d %d %lf %lf\n", i, T, elapsed1, elapsed2);
	}

    /* TODO: connect to datagen and ask for the necessary data in each experiment round.
     * Create a Unix domain socket with DSOCKET_PATH (see const.h).
     * Talk to datagen using the messages specified in the assignment description document.
     * Read the values generated by datagen from the socket and use them to run your
     * experiments
     * */

    /* Probe time elapsed. */
    clock_t cend = clock();

    // Time elapsed in miliseconds.
    double time_elapsed = ((double) (cend - cbegin) / CLOCKS_PER_SEC) * 1000;

    printf("Time elapsed '%lf' [ms].\n", time_elapsed);

    exit(0);
}
