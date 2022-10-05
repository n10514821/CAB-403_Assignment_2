
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


// DEFINES
#define PARKING_SIZE 2920
//generates random licence plate numbers everytime it is run
#define LENGTH_LICENCEPLATE 5

// GLOBALS
// create a file pointer for communucation between file a program
FILE *fileptr;
//create a mutex lock
pthread_mutex_t lock;
//shared memory file descriptor and pointer to shared memory object
int shm_fd;
void *ptr;


//STRUCTS
// create the shared parking (not sure what should go in here)
typedef struct shared_carpark
{
    sem_t Fire_alarm_semaphore;
    sem_t Manager_sempahore;
    sem_t Simulator_semaphore;

}shared_carpark_t;

// shared memory structure
typedef struct shared_memory
{
    const char *name;

    int fd;

    //address of shared carpark block
    shared_carpark_t *data;

}shared_memory_t;

// PARKING struct
typedef struct PARKING
{
    struct Exit;
    struct Level;
    struct Entrance;
} shared_carpark_t;


//FUNCTIONS

bool create_shared_object(shared_memory_t *shm, const char *share_name)
{
    shm_unlink(share_name);

    //insert share name to shm->name
    shm->name = share_name;

    //create shared memory object
    shm_fd = shm_open(share_name,O_CREAT|O_RDWR,0666);
    shm->fd = shm_fd; 

    //configure the size of the shared memory object
    ftruncate(shm_fd,PARKING_SIZE);

    ptr = mmap(0,PARKING_SIZE, MAP_SHARED, shm_fd,0);
    shm->data = ptr;

    return true;
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
    for(int i = 0; i <= LENGTH_LICENCEPLATE; i ++)
    {
        char number = num[rand() % (sizeof num - 1)];
        char alphabet = alpha[rand() % (sizeof alpha - 1)];
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
void read_file()
{
    pthread_mutex_lock(&lock);
    char plate_num[7];
    //open file 
    fileptr = fopen("plates.txt","r +");

    // if file does not exist
    if(fileptr != NULL)
    {
        while (!feof(fileptr))
        {
            fscanf(fileptr,"%s",&plate_num); 
            printf("txt %s\n",plate_num);                 
        }  
    }
    else
    {
        printf("Error in opening file");
        exit(1);
    }
    pthread_mutex_unlock(&lock);
}

void generate_mix_plates()
{
    pthread_mutex_lock(&lock);
    for (int i = 0; i <= rand()%delay_ms(0.1); i++)
    {
        if(i <= rand()%1)
        {
            read_file();             
        }
        else
        {
            generate_plate_number(); 
        } 
    }
    pthread_mutex_unlock(&lock);

    
}


int main (void) 
{
    time_t t;
    srand((unsigned)time(&t));

    generate_mix_plates();

    fclose(fileptr);
    return 0;

}
