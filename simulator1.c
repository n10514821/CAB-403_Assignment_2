
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





// DEFINES
#define PARKING_SIZE 2920
//generates random licence plate numbers everytime it is run
#define LENGTH_LICENCEPLATE 5

// GLOBALS
// create a file pointer for communucation between file a program
FILE *fileptr;
//create a mutex lock
pthread_mutex_t lock;


//linked list for cars
typedef struct car car_t;

struct car
{
    char *licence_plate;
};

void plate_print(car_t *p)
{
    printf("licence plate=%s\n", p->licence_plate);
}

typedef struct node node_t;

// a node in a linked list of people
struct node
{
    car_t *car;
    node_t *next;
};

// print all people in the list pointer to by head
// pre: true
// post: list of people printed to screen
void node_print(node_t *head)
{
    for (; head != NULL; head = head->next)
    {
        plate_print(head->car);
    }
}

// add a car to the list pointed to by head
// pre: head != NULL
// post (return == NULL AND failed to allocate memory for new linked list node)
//      OR (return == the new head of the list)
node_t *node_add(node_t *head, car_t *car)
{
    //DOUBLE CHECK IF MEMORY WAS ALLOCATED.
    //alocate memory for the new_head. 
    node_t *new_head  = malloc(sizeof(node_t)); 
    if(new_head == NULL)
    {
        return NULL;
    }
    new_head->car = car;
    //pre pending saves complexity and is faster than appending.
    new_head->next = head;
    return new_head;
}

// find person by name in list pointed to by head
// pre: head != NULL
// post (return == NULL AND name not found)
//      OR (return == node with person named name)
node_t *node_find_plate(node_t *head, char *licence_plate)
{
    for(; head != NULL; head = head->next)
    {
        if(strcmp(head->car->licence_plate, licence_plate) == 0)
        {
            return head;
        }
    }
    return NULL;
}

// delete a person by name in list pointed to by head
// pre: head != NULL
// post: return == the new head of the list
node_t *node_delete(node_t *head, char *licence_plate)
{
    node_t *prev;
    node_t *curr = head;
    for(; curr != NULL; curr = curr->next)
    {
        if(strcmp(curr->car->licence_plate, licence_plate)== 0)
        {
            //unlink this node
            //if head
            if(curr == head)
            {
                node_t *temp_head = head->next;
                free(head);
                return temp_head;
            }
            else
            {
                prev->next = curr->next;
                free(curr);
                return head;
            }
        }
        prev = curr;
    }
    return head;

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
void *generate_mix_plates(void *arg)
{
   // pthread_mutex_lock(&lock);
    char plate_num[7];
    int nLines = 0;
    char line[100];
    int randLine;
    //open file 
    fileptr = fopen("plates.txt","r +");

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
    exit(0);

}






    //declerations of thread condition variable
    pthread_cond_t cond1 = PTHREAD_COND_INITIALIZER;
    //DECLAR MUTEX
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;



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


// MAIN PROGRAM
int main (void) 
{
    time_t t;
    srand((unsigned)time(&t));
    pthread_t thread_id, thread_id1;


    // generates plate numbers at different enterances. 
    for (int i = 0; i < 5; i++)
    {
        printf("\nenterance %d\n",i);

        for (int k = 0; k < 5; k++)
        {
            pthread_create(&thread_id, NULL, generate_mix_plates, (void *)&thread_id);
            
        }   
         
    }
    pthread_exit(NULL);
    return 0;
  

}
