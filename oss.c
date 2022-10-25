#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>

void oss();
void worker();

#define PERMS 0644
struct my_msgbuf {
   long mtype;
   char mtext[200];
};
#define SHMKEY  859047     /* oss and worker agree on common key.*/
#define BUFF_SZ sizeof ( int )

int opt, flag = 0, max_proc = 18, def_time = 100, errno, pid = 0, * oss = NULL, * worker = NULL;
time_t start = 0;

int main(int argc, char * argv[]) {
  system("clear");

  signal(SIGINT, signal_abort); /*Abort for Ctrl+C*/ 
  signal(SIGALRM, signal_timer); /*Abort for end of termination time*/ 

  /* Parsing Command Line argument */
  while ((opt = getopt(argc, argv, "hn:")) != -1) {
    switch (opt) {
    case 'h':
      printInfo();
      break;
    case 'n':
      max_proc = atoi(argv[2]);
      if (max_proc > MAXPROC) {
        perror("Numbers of Processes can not exceed 18. Number of processes range: [1,18]");
        max_proc = 18;
      }
      break;
    case 's':
      max_child = atoi(argv[3]);
      if(max_child> 5 )
        perror("Numbers of child can not exceed 5. Number of child range: [1,5]");
      break;
    case 'm':
      flag = 1;
      def_time = atoi(optarg);
      if (def_time < 1) {
        perror("Termination time is not valid. Time Range to choose:[1,10000000]");
      }
      break;
    case 'f':
      fprintf(stderr, "\n\nSolution can be invoked using the following command: \n ./driver [-f] [logfile.txt]");
      break;

    case '?':
      if (opt == 'c')
        perror("Error: Option requires an argument");
      else
        perror("Error: Unknown option character");
      return 1;

    }

  }
  if (flag == 0) {
    max_proc = atoi(argv[1]);
    if (max_proc > MAXPROC) {
      perror("Warning: Numbers of Processes can not exceed 18");
      max_proc = 18;
    }
  }

  fprintf(stderr, "Timer value selected: %d\n", def_time);
  fprintf(stderr, "Number of processes: %d\n", max_proc);

  oss = malloc(sizeof(int) * max_proc);
  worker = malloc(sizeof(int) * max_proc);

  /* Creating a shared memory segment */
  const int sh_key = ftok("main.c",0);
  int shm_id = shmget( sh_key , sizeof(int) * 10 , IPC_CREAT | 0666  );
  if (shm_id <= 0) {
   fprintf(stderr,"Shared memory get failed\n");
   exit(1);
  }
  int *shm_ptr = shmat( shm_id , 0 , 0 );
  if (shm_ptr <= 0) {
   fprintf(stderr,"Shared memory attach failed\n");
   exit(1);
  }

  /* Forking child processes  */
  switch ( fork() )
    {
    case -1:
        fprintf(stderr,"Failed to fork\n");
        return ( 1 );
    case 0:
        worker();
        break;
    default:
        oss();
        break;
    }
    return ( 0 );
}
void oss()
{
    /* Get shared memory segment identifier*/
    int i;
    int shmid = shmget ( SHMKEY, BUFF_SZ, 0777 | IPC_CREAT );
    if ( shmid == -1 ) {
       fprintf(stderr,"oss: ... Error in shmget ...\n");
       exit (1);
    }
    /* Get the pointer to shared block*/
    char * paddr = ( char * )( shmat ( shmid, 0, 0 ) );
    int * pint = ( int * )( paddr );
    for ( i = 0; i < 10; i++ ) {
       sleep ( 2 );
       *pint = 10 * i ;             /* Write into the shared area. */
       printf("oss: Written Val.: = %d\n",*pint);
    }
    shmdt(pint);
    shmctl(shmid,IPC_RMID,NULL);
}
void worker()
{
    int i;
    sleep ( 5 );
    int shmid = shmget ( SHMKEY, BUFF_SZ, 0777 );
    if ( shmid == -1 ) {
       fprintf(stderr,"worker: ... Error in shmget ...\n");
       exit ( 1 );
    }
    int * cint = ( int * )( shmat ( shmid, 0, 0 ) );
    for ( i = 0; i < 10; i++ ) {
       sleep ( 1 );
       printf("worker: Read Val. = %d\n",*cint);
    }
    shmdt(cint);
}
/*exec*/
char* args[] = {"./worker", m,n, paddr};
/*execvp(args[0], args);*/
execlp(args[0],args[0],args[1],args[2],args[3]);
fprintf(stderr,"Exec failed, terminating\n");
exit(1);

  /* Deallocate Shared Memory */
  shmdt( shm_ptr );    /* Detach from the shared memory segment*/
  shmctl( shm_id, IPC_RMID, NULL ); /* Free shared memory segment shm_id*/
  exit(EXIT_SUCCESS);


/*Display information */
void printInfo() {
  printf("Command line arguments are as follows:\n");
  printf("-h\tDisplays this message and terminates.:\n");
  printf("-n x\tProgram creates n processes (default of 18, domain of [1,18]\n");
  printf("-m x\tEach child process will increment the clock by 10000000\n");
  
}

/* signal handle for time out */
void signal_timer(int signal){
     perror("master: Warning: timer ends");
    for (i=0; i < max_proc; i++) 
        kill(slave[i], signal);
     deallocate_shm();
     exit(EXIT_FAILURE);
  
}
/* signal handle for receiving CTRL + C */
void signal_abort(){ 
    perror("master: Warning: CTRL + C received, master is terminating");
    killpg((*master), SIGTERM);
    killpg((*slave), SIGTERM);
    for(i = 0; i < max_proc; i++){
    wait(NULL);
      }
    deallocate_shm();
    exit(EXIT_FAILURE);
}

/*send messages*/
void msg_snd(){
  struct my_msgbuf buf;
   int msqid;
   int len;
   key_t key;
   system("touch msgq.txt");
   
   if ((key = ftok("msgq.txt", 'B')) == -1) {
      perror("ftok");
      exit(1);
   }
   
   if ((msqid = msgget(key, PERMS | IPC_CREAT)) == -1) {
      perror("msgget");
      exit(1);
   }
   printf("message queue: ready to send messages.\n");
   printf("Enter lines of text, ^D to quit:\n");
   buf.mtype = 1; /* we don't really care in this case */
   
   while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) {
      len = strlen(buf.mtext);
      /* remove newline at end, if it exists */
      if (buf.mtext[len-1] == '\n') buf.mtext[len-1] = '\0';
      if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
      perror("msgsnd");
   }
   strcpy(buf.mtext, "end");
   len = strlen(buf.mtext);
   if (msgsnd(msqid, &buf, len+1, 0) == -1) /* +1 for '\0' */
   perror("msgsnd");
   
   if (msgctl(msqid, IPC_RMID, NULL) == -1) {
      perror("msgctl");
      exit(1);
   }
   printf("message queue: done sending messages.\n");
}


