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

//number of entrances



// GLOBALS

// create a file pointer for communucation between file a program
FILE *fileptr;
//create a mutex lock
pthread_mutex_t lock;
shared_memory_t shm;
char *generatePlate(int probability);
int randomIntGenerator(int min, int max);
char* random_license_plate_generator();

void readtextFile(char *file);
void printFile();
char textplates[100][7];
int car_index;
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
        shm.data->level[i].alarm = '0';
        shm.data->level[i].tempsense = 24;
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

/*Car queue code*/

#define MAX 100
#define CAP 7
typedef struct Queue
{
    char cont[MAX][CAP];

    int s;
} queue_t;


// create queue
void plateInit(queue_t *carQueue){
    carQueue->s = 0;
    for (int i = 0; i < MAX; i++){
        strcpy(carQueue->cont[i], "empty");
    }
}

// Add selected licensnse plate to queue
void addPlate(queue_t* carQueue, char * plate){
    int old_size = carQueue->s;
    strcpy(carQueue->cont[old_size], plate);
    carQueue->s = old_size + 1;
}

// pop plate at first index
void popPlate(queue_t* carQueue){
    int old_size = carQueue->s;
    char old_data[MAX][CAP];
    for (int i = 0; i < old_size; i++){
        strcpy(old_data[i], carQueue->cont[i]);
    }
    for (int i = 0; i < old_size - 1; i++){
        strcpy(carQueue->cont[i], old_data[i + 1]);
    }
    carQueue->s = old_size - 1;
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

//public variable for a queue
queue_t queue_entrances[entrances];

//public variable for entrance mutex
pthread_mutex_t mutex_q[entrances];

#define max_cars_generated 10

void *Car(void *args) {
    char* licensePlate;  
    int waitTime;

    // Initialise entrance queues
    for (int i = 0; i < entrances; i++){
        plateInit(&queue_entrances[i]);
    }
    
    for (int i = 0;i < max_cars_generated;i++){

        // Numberplate gen for car
        licensePlate = generatePlate(50);
        
        car_index++;

        // every 1 10 100 milliseconds a car is generated
        waitTime = randomIntGenerator(1,100) * 1000;
        usleep(waitTime);

        // Select an entrance to enter
        int randEntrance = randomIntGenerator(0,entrances - 1);


        printf("The plate %s is arriving at entrance %d\n",licensePlate,randEntrance + 1);


        // car creation thread
        pthread_mutex_lock(&mutex_q[randEntrance]);
        addPlate(&queue_entrances[randEntrance], licensePlate);
        pthread_mutex_unlock(&mutex_q[randEntrance]);
    }
    return 0;
} 

//Entrance Simulation

void eSim(void *arg){
    int i = *(int*) arg;

    //do until program stops
    for (;;){

        pthread_mutex_lock(&shm.data->entrance[i].lpr.lock);

        while(strcmp(shm.data->entrance[i].lpr.licensePlate, "000000")){
            pthread_cond_wait(&shm.data->entrance[i].lpr.condition, &shm.data->entrance[i].lpr.lock);
        }
        pthread_mutex_unlock(&shm.data->entrance[i].lpr.lock);



        while(queue_entrances[i].s <=0);

        pthread_mutex_lock(&shm.data->entrance[i].lpr.lock);
        pthread_mutex_lock(&mutex_q[i]);
        strcpy(shm.data->entrance[i].lpr.licensePlate, queue_entrances[i].cont[0]);
        pthread_mutex_unlock(&mutex_q[i]);
        pthread_mutex_unlock(&shm.data->entrance[i].lpr.lock);

        usleep(2000);

        pthread_cond_signal(&shm.data->entrance[i].lpr.condition);
        pthread_mutex_lock(&mutex_q[i]);
        popPlate(&queue_entrances[i]);
        pthread_mutex_unlock(&mutex_q[i]);
        
    }
}



// Constructs a random plate
char* random_license_plate_generator(){
    
    int first = randomIntGenerator(0, 9);
    int second = randomIntGenerator(0, 9);
    char third = randomIntGenerator(0, 9);

    char randomletter1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[randomIntGenerator(0, 25)];
    char randomletter2 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[randomIntGenerator(0, 25)];
    char randomletter3 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[randomIntGenerator(0, 25)];

    char *finstr = NULL;
    finstr = malloc(10);
    sprintf(&finstr[0], "%d", first);
    sprintf(&finstr[1], "%d", second);
    sprintf(&finstr[2], "%d", third);    
    finstr[3] = randomletter1;
    finstr[4] = randomletter2;
    finstr[5] = randomletter3;
    finstr[6] = '\0';

    return finstr;
}

int randomIntGenerator(int min, int max){
    int randomNumber = (rand() % (max - min + 1)) + min;
    return randomNumber;
}





#define FIRE 0

void *tempSensorSimulate(void *arg) {
    int i = *(int*) arg;
    int16_t temperature;
    int16_t currentTemp;

    for (;;) {
        usleep(2000);
        if (FIRE == 1) { // (Fixed temp fire detection data)
            // Generate temperatures to trigger fire alarm via Temps > 58 degrees
            temperature = (int16_t) randomIntGenerator(58, 65);
            shm.data->level[i].tempsense = temperature;
        }
        else if (FIRE == 2) { // (Rate-of-rise fire detection data)
            // Generate temperatures to trigger fire alarm via Rate-of-rise (Most recent temp >= 8 degrees hotter than 30th most recent)
            if (shm.data->level[i].tempsense > 58){
                currentTemp = 24;
            }
            else {
                currentTemp = shm.data->level[i].tempsense;
            }
            temperature = randomIntGenerator(currentTemp, currentTemp + 2);
            shm.data->level[i].tempsense = temperature;
        }
        else {
            // Generate normal temperatures to avoid setting off fire alarm
            temperature = (int16_t) 24;
            shm.data->level[i].tempsense = temperature;
        }    
    }
}

//public variazbles

// MAIN PROGRAM
int main (void) 
{
    

     car_index = 0;

    pthread_t Spawn_Car;
    pthread_t e_thread[entrances];
    pthread_t t_thread[levels];
    
    for (int i = 0; i < entrances; i++){
        pthread_mutex_init(&mutex_q[entrances], NULL);
    }

    time_t t;
    srand((unsigned) time(&t));

    create_shared_memory(&shm, "PARKING");
    
    initSharedMem(shm);

    readtextfile("plates.txt");

    
    int i;
    pthread_create(&Spawn_Car, NULL, &Car, NULL);
    

    for(i = 0; i<entrances; i++){
        int* point = malloc(sizeof(int));
        *point = i;
        pthread_create(&e_thread[i], NULL, &eSim, point);
    }

    for (i = 0; i < levels; i++){
        int* z = malloc(sizeof(int));
        *z = i;
        pthread_create(&t_thread[i], NULL, &tempSensorSimulate, z);
    }

    pthread_join(Spawn_Car, NULL);

    for (i = 0; i < entrances; i++){
        pthread_join(e_thread[i], NULL);
    }

     for (i = 0; i < levels; i++){
        pthread_join(t_thread[i],NULL);
    } 
    
    

}



char* generatePlate(int probability){
    int random = randomIntGenerator(0, 100);
    if (random <= probability){
        return textplates[car_index];
    }
    else{
        char *p = random_license_plate_generator();
        return p;
    }
}

void readtextfile(char *text){
      FILE* file = fopen(text, "r");

    // Fill array
    
     int i = 0;
    while (fgets(textplates[i], 10, file)) {
        textplates[i][strlen(textplates[i]) - 1] = '\0';
        i++;
    }
    
    
}

void printFile(){
	printf("\n The content of the file  are : \n");    
    for(int i = 0; i < 100; i++)
    {
        printf("%s, ", textplates[i]);
        
    }
    printf("\n");
}