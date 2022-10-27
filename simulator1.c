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



// GLOBALS
int car_index;
char textplates[100][7];
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

#define max_cars_generated 8

void *Car(void *args) {
    char* licensePlate;  
    int waitTime;

    // Initialise entrance queues
    for (int i = 0; i < entrances; i++){
        plateInit(&queue_entrances[i]);
    }
    
    for (int i = 0;i < max_cars_generated;i++){

        // Numberplate gen for car
        licensePlate = generatePlate(80);
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


void readtextfile(char *text){
     FILE* file = fopen(text, "r");

    // Fill array
    int i = 0;

    while (fgets(textplates[i], 10, file)) {
        textplates[i][strlen(textplates[i]) - 1] = '\0';
        i++;
    }
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


        pthread_cond_signal(&shm.data->entrance[i].lpr.condition);
        pthread_mutex_lock(&mutex_q[i]);
        popPlate(&queue_entrances[i]);
        pthread_mutex_unlock(&mutex_q[i]);

    }
}

char* license_plate_generator(int chance){

    int rand = randomIntGenerator(0, 100);

    if (rand <= chance){
        return textplates[car_index];
    }
    else{
        char *p = randomPlate();
        return p;
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


//public variazbles
shared_memory_t shm;
// MAIN PROGRAM
int main (void) 
{

    create_shared_memory(&shm, "PARKING");
    generate_mix_plates();
   // generate_mix_plates();
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
