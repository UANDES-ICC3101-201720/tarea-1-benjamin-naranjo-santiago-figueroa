#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include "types.h"
#include "const.h"
#include "util.h"

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

    return 0;
}
void quicksort(int arr[],int lo, int hi){
  if (lo < hi){
    int p = partition(arr, lo, hi);
    quicksort(arr, lo, p-1);
    quicksort(arr, p+1, hi);
  }
}
int partition(int arr[], int lo, int hi){
  int pivot = arr[hi];
  int i = lo;
  int aux;
  for(int j=lo; j < hi; j++){
    if (arr[j] < pivot){
      aux = arr[i];
      arr[i] = arr[j];
      arr[j] = aux;
      i++;
    }
    aux = arr[i];
    arr[i] = arr[hi];
    arr[j] = aux;
  }
  return i;
}


int main(int argc, char** argv) {
    /* TODO: move this time measurement to right before the execution of each binsearch algorithms
     * in your experiment code. It now stands here just for demonstrating time measurement. */
    clock_t cbegin = clock();

    printf("[binsearch] Starting up...\n");

    /* Get the number of CPU cores available */
    printf("[binsearch] Number of cores available: '%ld'\n",
           sysconf(_SC_NPROCESSORS_ONLN));

    /* TODO: parse arguments with getopt */
    if (argc == -1){
      fprintf("Error de argumentos" );
      exit(EXIT_FAILURE);
    }
    else{
      //E will be the second element and T the third and P the fourth
      int E = atoi(argv[1]);
      int T = atoi(argv[2]);
      int P = atoi(argv[3]);
      if ((E <= 1) or ((T <= 3)or(T >= 9)) or ((P <= 0)or(P >= pow (T, 10) - 1)){
        fprintf("Error de argumentos")
        exit(EXIT_FAILURE);
      }
    }

    /* TODO: start datagen here as a child process. */


    /* TODO: implement code for your experiments using data provided by datagen and your
     * serial and parallel versions of binsearch.
     * */

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
