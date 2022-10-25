#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>


/* incrementing share clock by 10000000*/
int main(int argc, char *argv[])
{
    int i;

    char log_filename[100];
    sprintf(log_filename, "logfile.%d", p_id);

    time_t current_time;
    struct tm *loc_time;
    char msg[LEN];
    int seed = 0;

    semaphoreGrab();
    srand(time(NULL) + p_id + i); // seed for random sleep time
        
    /* ENTRY Section*/
      
    semaphoreWait();
    printf("process %d in the Queue\n", p_id);
        
    logfile = fopen(log_filename, "a");
    if (logfile == NULL){
        perror("slave: Error: logfile open failed");
    }
	
	 /* Critical Section */
    cstest = fopen("cstest", "a");
    if (cstest == NULL){
        perror("slave: Error: cstest open failed");
    }
    /* incrementing share clock by 10000000*/
    for (i =0; i < argc; i++){
  		paddr=paddr*10000000
  		printf("worker_pid %d",getpid());
  		printf(": Iteration %i",i);
  		printf(": Incrementing by %p",paddr);
  		printf("|%s| ", argv[i]);
	}
        
    current_time = time (NULL);  	
   	loc_time = localtime (&current_time);
    fprintf(logfile, "Process %d is in critical section, Timestamp:%s\n", p_id, asctime (loc_time));

    seed = (rand() % 5) + 1; 
    sleep(seed);
 	current_time = time (NULL);  	
   	loc_time = localtime (&current_time);
    strftime (msg, LEN, "%H:%M:%S", loc_time);
    fprintf(cstest, "%s Queue %d File modified by process number %d\n", msg, i, p_id);
    fprintf(logfile, "Process id %d writes on shared resource, Timestamp:, cstest %s\n", p_id, asctime (loc_time));

    seed = (rand() % 5) + 1; 
    sleep(seed);
	current_time = time (NULL);   	
   	loc_time = localtime (&current_time);
    fprintf(logfile, "Process id: %d exits critical section, Timestamp: %s\n", p_id, asctime (loc_time));
    fclose(logfile);
    fclose(cstest);
    logfile = NULL;
    cstest = NULL;
       
    /* EXIT Section */
    printf("process %d finished the job\n", p_id);
    semaphoreSignal();


	printf("\nChild is now ending.\n");
	sleep(3);
	return EXIT_SUCCESS;

}

