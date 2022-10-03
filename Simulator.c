
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

// deifnes
#define PARKING_SIZE 2920
#define LENGTH_LICENCEPLATE 5


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

//shared memory file descriptor and pointer to shared memory object
int shm_fd;
void *ptr;

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
//generates random licence plate numbers everytime it is run

time_t delay_ms(int seconds)
{
    int milli_sec = 1000 *seconds;

    clock_t start = clock();
    while(clock() < start + milli_sec);
}

void generate_plate_number()
{
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
            printf("%c",alphabet);
    
        }
    }
    printf("\n");
}


int main (void) 
{
    srand((time(NULL)));
    // generate between 1- 100ms
    for(int j = 0; j <= rand() % delay_ms(0.1); j++)
    {
        generate_plate_number();
    }


}
