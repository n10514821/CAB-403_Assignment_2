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
#include <ctype.h>
#include <stdint.h>


// DEFINES

//SIZE OF PARKING LOT SHARED MEM
#define PARKING_SIZE 2920

//PERCENTAGE CHANCE THAT A GENERATED PLATE IS ON THE LIST
#define PLATEGETSIN 50

//DEFINES FOR THE QUEUE STRUCT
#define MAX 100
#define CAP 7

//MAXIMUM AMOUNT OF CARS GENERATED
#define max_cars_generated 50


//Shared memory
shared_memory_t shm;

//declaring some functions because i did them below other stuff

//function for generating license plate number
char *licenseplatenumbergen(int probability);

//generates random integer value 
int randomIntGenerator(int min, int max);

//generates a random license plate
char* random_license_plate_generator();

//reads text file into 2d char array
void textreader(char *file);


void print_plate_array();

//char array of license plates
char textplates[100][7];

//index of allowed plates into parking garage
int car_index;


typedef struct Queue
{
    char cont[MAX][CAP];

    int s;
} queue_t;

//public variable for a queue
queue_t queue_entrances[entrances];

//public variable for entrance mutex
pthread_mutex_t mutex_q[entrances];





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
    
    return true;

}

//setting Inter-process communication for the shared memory (page 7 of spec sheet)
void initSharedMem(shared_memory_t shm){
    //mutex
    pthread_mutexattr_t mute;
    //condition
    pthread_condattr_t con;
    pthread_mutexattr_init(&mute);
    pthread_condattr_init(&con);

    //setting both to shared as per assignment spec
    pthread_mutexattr_setpshared(&mute, PTHREAD_PROCESS_SHARED);
    pthread_condattr_setpshared(&con, PTHREAD_PROCESS_SHARED);

    //for each level initialise mutex & condt variables, 
    int i = 0;
    for (int i = 0; i < levels;i++){
        //lp sensors
        pthread_mutex_init(&shm.data->level[i].lpr.lock, &mute);
        pthread_cond_init(&shm.data->level[i].lpr.condition, &con);

        //setting temprature sensor and alarm per each level
        shm.data->level[i].alarm = '0';
        shm.data->level[i].tempsense = 24;
    }
    
    //For each entrance, init mutex & condt variables
    int k = 0;
    for (int k = 0;k < entrances;k++){

        // entrance lp sensors mutex and condition
        pthread_mutex_init(&shm.data->entrance[k].lpr.lock, &mute);
        pthread_cond_init(&shm.data->entrance[k].lpr.condition, &con);

        // entrance boomGates mutex and condition
        pthread_mutex_init(&shm.data->entrance[k].boomGateEn.lock, &mute);
        pthread_cond_init(&shm.data->entrance[k].boomGateEn.condition, &con);

        // entrance signs mutex and condition
        pthread_mutex_init(&shm.data->entrance[k].sign.lock, &mute);
        pthread_cond_init(&shm.data->entrance[k].sign.condition, &con);

        // entrance sets boomGates C or closed and set init lpr to xxxxxx
        shm.data->entrance[k].boomGateEn.status = 'C';
        strcpy(shm.data->entrance[k].lpr.licensePlate, "xxxxxx");
    }

    for (int j = 0;j < exits;j++){
        // exit lp sensors
        pthread_mutex_init(&shm.data->exit[j].lpr.lock, &mute);
        pthread_cond_init(&shm.data->exit[j].lpr.condition, &con);
        // exit boomGates
        pthread_mutex_init(&shm.data->exit[j].boomGateEx.lock, &mute);
        pthread_cond_init(&shm.data->exit[j].boomGateEx.condition, &con);

        // exit sets gates to 'C' / closed
        shm.data->exit[j].boomGateEx.status = 'C';
        strcpy(shm.data->exit[j].lpr.licensePlate, "xxxxxx");
    }

}



/*Car queue code*/


// create queue
void createqueue(queue_t *carQueue){
    //setting queue size s to 0
    int size = 0;
    carQueue->s = 0;

    size++;
    //for each spot in the queue
    for (int i = 0; i < MAX; i++){

    // emtpy each queue spot 
        strcpy(carQueue->cont[i], "blank");
    }
}

// Add selected licensnse plate to queue
void car_queues_up(queue_t* Queue, char * license){
    //saving old queue size
    int old_size = Queue->s;

    //adding +1 to the old sizeof the queue and adding license plate
    strcpy(Queue->cont[old_size], license);
    Queue->s = old_size + 1;
}

// remove plate at first index
void car_enters(queue_t* Queue){

    //saving old size of queue
    int old_size = Queue->s;

    int new_size = old_size;

    //saving old array of cars
    char old_data[MAX][CAP];

    //copying over old data to a new queue
    for (int i = 0; i < old_size; i++){
        strcpy(old_data[i], Queue->cont[i]);
        new_size++;
    }

    //adding + 1 to the index
    for (int i = 0; i < old_size - 1; i++){
        strcpy(Queue->cont[i], old_data[i + 1]);
    }
    //removing car from the queue
    Queue->s = old_size - 1;
}

// pop plate at random index
void popRandom(queue_t* carQueue, int index){
    int old_size = carQueue->s;
    char old_data[MAX][CAP];
    for (int i = 0; i < old_size; i++){
        strcpy(old_data[i], carQueue->cont[i]);
    }
    for (int i = 0; i < index; i++){
        strcpy(carQueue->cont[i], old_data[i]);
    }

    for (int i = index; i < old_size - 1; i++){
        strcpy(carQueue->cont[i], old_data[i + 1]);
    }
    carQueue->s = old_size - 1;
}



/*Car spawning method*/


void *Car(void *args) {
    char* license_number;   
    int waitTime;

     // setup entrances with createqueue
    for (int i = 0; i < entrances; i++){

        createqueue(&queue_entrances[i]);
    }
    
    //for the max amount of cars set to be generated
    for (int i = 0;i < max_cars_generated;i++){

        // call licenseplatenumbergen with the chance of a plate being generated being in the list as an arg
        license_number = licenseplatenumbergen(PLATEGETSIN);

        //add +1 to the index of the cars entered (used for string array indexing)
        car_index++;

        // wait 1 - 100 
        waitTime = randomIntGenerator(1,100) * 1000;
        usleep(waitTime);

        // select an entrance to queue up at 
        int randEntrance = randomIntGenerator(0,entrances - 1);

        //print out (mainly for testing)
        printf("Car %s is pulling up to entrance %d\n",license_number,randEntrance + 1);
        


        // car creation threads
        pthread_mutex_lock(&mutex_q[randEntrance]);
        car_queues_up(&queue_entrances[randEntrance], license_number);
        pthread_mutex_unlock(&mutex_q[randEntrance]);
        
    }
    return 0; 

    
} 

//Entrance Simulation

void eSim(void *arg){
    int i = *(int*) arg;

    //do forever
    for (;;){

        //lock thread
        pthread_mutex_lock(&shm.data->entrance[i].lpr.lock);

        while(strcmp(shm.data->entrance[i].lpr.licensePlate, "000000")){
            pthread_cond_wait(&shm.data->entrance[i].lpr.condition, &shm.data->entrance[i].lpr.lock);
        }
        pthread_mutex_unlock(&shm.data->entrance[i].lpr.lock);


        //if plate is in the queue
        while(queue_entrances[i].s <=0);


        //take queued up car and put it into the shm
        pthread_mutex_lock(&shm.data->entrance[i].lpr.lock);
        pthread_mutex_lock(&mutex_q[i]);
        strcpy(shm.data->entrance[i].lpr.licensePlate, queue_entrances[i].cont[0]);
        pthread_mutex_unlock(&mutex_q[i]);
        pthread_mutex_unlock(&shm.data->entrance[i].lpr.lock);

        //delaying lpr
        usleep(2000);

        //trigger lpr and let manager know about new car
        pthread_cond_signal(&shm.data->entrance[i].lpr.condition);
        pthread_mutex_lock(&mutex_q[i]);
        car_enters(&queue_entrances[i]);
        pthread_mutex_unlock(&mutex_q[i]);
        
    }
}



// Constructs a random plate
char* random_license_plate_generator(){

    //generating the first three letters
    char first = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[randomIntGenerator(0, 25)];
    char second = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[randomIntGenerator(0, 25)];
    char third = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[randomIntGenerator(0, 25)];
    
    //generating the first three numbers
    int one = randomIntGenerator(0, 9);
    int two = randomIntGenerator(0, 9);
    char three = randomIntGenerator(0, 9);

    

    char *new_randplatearray = NULL;
    new_randplatearray = malloc(10);

    //adding the numbers first
    sprintf(&new_randplatearray[0], "%d", one);
    sprintf(&new_randplatearray[1], "%d", two);
    sprintf(&new_randplatearray[2], "%d", three);   

    //adding letters onto the numbers
    new_randplatearray[3] = first;
    new_randplatearray[4] = second;
    new_randplatearray[5] = third;

    //adding a null terminator to the end so it gets picked up properly
    new_randplatearray[6] = '\0';

    return new_randplatearray;
}

//borrowed from stackoverflow
int randomIntGenerator(int min, int max){
    int randomNumber = (rand() % (max - min + 1)) + min;
    return randomNumber;
}








int main (void) 
{
    //setting index of added cars to 0
    car_index =0;

    //setting up threads for car spawing, entrances and the temprature
    pthread_t Spawn_Car;
    pthread_t e_thread[entrances];


    //for each entrance, setup mutex
    for (int i = 0; i < entrances; i++){
        pthread_mutex_init(&mutex_q[entrances], NULL);
    }


    //create shm
    create_shared_memory(&shm, "PARKING");
    
    //initalise shared mem
    initSharedMem(shm);

    //read the text file and put plates into array
    textreader("plates.txt");

    
    int i;
    pthread_create(&Spawn_Car, NULL, &Car, NULL);
    

    for(i = 0; i<entrances; i++){
        int* point = malloc(sizeof(int));
        *point = i;
        pthread_create(&e_thread[i], NULL, &eSim, point);
    }


    pthread_join(Spawn_Car, NULL);

    for (i = 0; i < entrances; i++){
        pthread_join(e_thread[i], NULL);
    }

     
    
    

}

void textreader(char *filename){
    FILE* file = fopen(filename, "r");

    // Fill array
    int i = 0;
    while (fgets(textplates[i], 10, file)) {
        textplates[i][7 - 1] = '\0';
        i++;
    }
    
}

// Prints contents of numberplate file (USED FOR TESTING)
void print_plate_array(){
	printf("\n The content of the file  are : \n");    
    for(int i = 0; i < 100; i++)
    {
        printf("%s, ", textplates[i]);
    }
    printf("\n");
}

// Generates a plate (Random/Allowed)
char* licenseplatenumbergen(int probability){
    int random = randomIntGenerator(0, 100);
    if (random <= probability){
        return textplates[car_index];
    }
    else{
        char *p = random_license_plate_generator();
        return p;
    }
}