#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include <math.h>
#include <stddef.h>
#include "mem.h"




// DEFINES
#define PARKING_SIZE 2920
//generates random licence plate numbers everytime it is run
#define LENGTH_LICENCEPLATE 5

//number of entrances
#define ENTRANCES 5


// GLOBALS
// create a file pointer for communucation between file a program
FILE *fileptr;
//create a mutex lock
pthread_mutex_t lock;

//creating shared memory for simulator
bool create_shared_memory(shared_memory_t* shm, const char* name){
    //removing previous instances of the shared memory object
    shm_unlink(name);

    //assigning share name to shm->name
    shm->name = name;

    //create shared mem object

    if ((shm->fd = shm_open(name, O_CREAT | O_RDWR, 0666)) < 0)
    {
        shm->data = NULL;
        return false;
    }
    
    // Set the capacity of the shared memory object using ftruncate
    
    if ((ftruncate(shm->fd, PARKING_SIZE)) < 0){
        shm->data = NULL;
        return false;
    }

    //map mem segment using mmap
     if ((shm->data = mmap(0, PARKING_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm->fd, 0)) == (void *)-1){
      return false;
    }

    //if all of the above worked return true 
    printf("aaa");
    return true;

}

//setting Inter-process communication for the shared memory (page 7 of spec sheet)
void initSharedMem(shared_memory_t shm){
    pthread_mutexattr_t mute;
    pthread_condattr_t con;
    pthread_mutexattr_init(&mute);
    pthread_condattr_init(&con);
    pthread_mutexattr_setpshared(&mute, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&con, PTHREAD_PROCESS_SHARED);

    //for each level initialise mutex & condt variables, 
    for (int i = 0; i < levels;i++){
        //lp sensors
        pthread_mutex_init(&shm.data->level[i].lpr.lock, &mute);
        pthread_cond_init(&shm.data->level[i].lpr.condition, &con);
        shm.data->level[i].alarm.on = '0';
        shm.data->level[i].tempsense.temp = 24;
    }
    
    //For each entrance, init mutex & condt variables
    for (int i = 0;i < entrances;i++){

        // lp sensors
        pthread_mutex_init(&shm.data->entrance[i].lpr.lock, &mute);
        pthread_cond_init(&shm.data->entrance[i].lpr.condition, &con);

        // boomGates
        pthread_mutex_init(&shm.data->entrance[i].boomGateEn.lock, &mute);
        pthread_cond_init(&shm.data->entrance[i].boomGateEn.condition, &con);

        // signs
        pthread_mutex_init(&shm.data->entrance[i].sign.lock, &mute);
        pthread_cond_init(&shm.data->entrance[i].sign.condition, &con);

        // sets boomGates C or closed
        shm.data->entrance[i].boomGateEn.status = 'C';
        strcpy(shm.data->entrance[i].lpr.licensePlate, "xxxxxx");
    }

    for (int i = 0;i < exits;i++){
        // lp sensors
        pthread_mutex_init(&shm.data->exit[i].lpr.lock, &mute);
        pthread_cond_init(&shm.data->exit[i].lpr.condition, &con);
        // boomGates
        pthread_mutex_init(&shm.data->exit[i].boomGateEx.lock, &mute);
        pthread_cond_init(&shm.data->exit[i].boomGateEx.condition, &con);

        // sets gates to 'C' / closed
        shm.data->exit[i].boomGateEx.status = 'C';
        strcpy(shm.data->exit[i].lpr.licensePlate, "xxxxxx");
    }

}


//time delay
time_t delay_ms(int seconds)
{
    int milli_sec = 1000 *seconds;

    clock_t start = clock();
    while(clock() < start + milli_sec);
}

//generate plate numbers
void generate_plate_number()
{
    pthread_mutex_lock(&lock);
    char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char num[] = "0123456789"; 
    char number;
    char alphabet;
    for(int i = 0; i <= LENGTH_LICENCEPLATE; i ++)
    {
        number = num[rand() % (sizeof num - 1)];
        alphabet = alpha[rand() % (sizeof alpha - 1)];
        if(i <= 2)
        {
            printf("%c", number);
        }
        else
        {   
            printf("%c", alphabet);


        }   
      
    }
    printf("\n");
    pthread_mutex_unlock(&lock);
    
   
}

//reads a file.
void *generate_mix_plates()
{
   // pthread_mutex_lock(&lock);
    char plate_num[7];
    int nLines = 0;
    char line[100];
    int randLine;
    //open file 
    fileptr = fopen("plates.txt.txt","r +");

    // reads lines in the file.
    if(fileptr != NULL)
    {
        while(!feof(fileptr)) 
        {
            fgets(line, sizeof(fileptr), fileptr);
            nLines++;
        }
        randLine = rand() % nLines;

        fseek(fileptr, 0, SEEK_SET);
        for(int j = 0; j <= rand()%delay_ms(0.1); j++)
        {
            for (int i = 0; !feof(fileptr) && i <= randLine; i++)
            {
                if(i <= randLine/2)
                {   
                    fgets(line, sizeof(fileptr), fileptr);
                    //printf(" %s",line);
                }
                else
                {
                    generate_plate_number();
                }
                
                
            }
        }
        sleep(0.1); 
    }
    else
    {
        printf("Error in opening file");
        exit(1);
    }
    fclose(fileptr);
   // pthread_mutex_unlock(&lock);

}






    



// create simulation for open boomgates
void *RasingBoomGate(void *threadID)
{
    //10ms to fully open gate 
    sleep(0.01);
    printf("gates is fully open\n");
    exit(0);  
}

// create simulation for closing boomgates
void *FallingBoomGate(void *threadID)
{
    //10ms to fully open gate 
    sleep(0.01);
    printf("gate is fully closed\n");
    exit(0);  
}

//leaving carpark 
void *LeavingCarpark()
{
    //takes it 10ms to reach exit and trigger LPR sensor
    sleep(0.01);
    printf("at exit boomgates");
    exit(0);
}

// car is parked
void *CarParked()
{
    //1000ms car parked. 
    printf("car is parked\n");
    for (int i = 0; i <= 10; i++)
    {
        sleep(10);
    }
    LeavingCarpark();
}

// enter parking
void *EnterParkings()
{
    //10ms for car to be parked
    sleep(0.01); 
    printf("car is parked\n");
    CarParked();
}

// front of queue
void *TriggerLPR()
{
    //2ms before triggering. 
    sleep(0.02);
    //trigger LPR 
}

//public variazbles
shared_memory_t shm;
// MAIN PROGRAM
int main (void) 
{

    create_shared_memory(&shm, "PARKING");
  //  time_t t;
  //  srand((unsigned)time(&t));
 //   pthread_t thread_id;
  //  struct node *head = NULL;
    
    // creating file pointer to work with files
 //   FILE* test = freopen("print.txt", "w a+", stdout);
 //   while(1)
 //   {
//        generate_mix_plates();
 //   }
 //   fclose(test);

    
    // // generates plate numbers at different enterances. 
    // for (int i = 0; i < ENTRANCES; i++)
    // {
    //     printf("\nenterance %d\n",i);

    //     for (int k = 0; k < ENTRANCES; k++)
    //     {
    //         pthread_create(&thread_id, NULL, generate_mix_plates, NULL);            
    //     }   
        
    // }



}
