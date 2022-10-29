#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <simulator1.c>
#include "mem.h"

//opening shared memory for manager
bool create_shared_object_R( shared_memory_t* shm, const char* share_name ) {

    //assigning share name to shm->name
    shm->name = share_name;

    //create shared mem object
    if ((shm->fd = shm_open(share_name, O_RDWR, 0)) < 0){
        shm->data = NULL;
        return false;
    }

    //map mem segment using mmap
    if ((shm->data = mmap(0, sizeof(shared_carpark_t), PROT_WRITE, MAP_SHARED, shm->fd, 0)) == (void *)-1)
    {
        return false;
    }

    //if all of the above worked return true
    return true;
}

void managerSetup(){
    // read plates.txt into hash table
    // create billing.txt
    // open PARKING segement
        // if PARKING not present
            // print "ERROR: no parking segement found"

    // create manager threads
    pthread_create(NULL, NULL, entranceManager(), int num = 1);
    pthread_create(NULL, NULL, entranceManager(), int num = 2);
    pthread_create(NULL, NULL, entranceManager(), int num = 3);
    pthread_create(NULL, NULL, entranceManager(), int num = 4);
    pthread_create(NULL, NULL, entranceManager(), int num = 5);
    pthread_create(NULL, NULL, exitManager(), int num = 1);
    pthread_create(NULL, NULL, exitManager(), int num = 2);
    pthread_create(NULL, NULL, exitManager(), int num = 3);
    pthread_create(NULL, NULL, exitManager(), int num = 4);
    pthread_create(NULL, NULL, exitManager(), int num = 5);
}

bool OpenThenCloseBoomGate(char gateType, int gateNum){
    // Setup Variables for accessing memory
    if(gateType == "entrance"){
        int statusAddress = 184 + (gateNum-1)*288;
        int conditionAddress = 136 + (gateNum-1)*288;
        pthread_cond_t gateMutex = (pthread_cond_t *)(ptr + 96 + (gateNum-1)*288);
    }else if(gateType == "exit"){
        int statusAddress = 1624 + (gateNum-1)*192;
        int conditionAddress = 1576 + (gateNum-1)*192;
        pthread_cond_t gateMutex = (pthread_cond_t *)(ptr + 1536 + (gateNum-1)*192);
    } else {
        printf("ERROR: invalid gate type");
        return false;
    }

    // Open gate (takes 10 ms)
    // set character status to 'R'
    pthread_mutex_lock(gateMutex);
    (char *)(ptr + statusAddress) = "R";
    pthread_mutex_unlock(gateMutex);
    pthread_cond_signal((pthread_cond_t *)(ptr + conditionAddress));

    // wait 30ms (10 for gate to open, 20 for car to enter/exit)
    sleep(0.03);

    // Close gate (takes 10 ms)
    // set character status to 'L'
    pthread_mutex_lock(gateMutex);
    (char *)(ptr + statusAddress) = "L";
    pthread_mutex_unlock(gateMutex);
    pthread_cond_signal((pthread_cond_t *)(ptr + conditionAddress));
    
    return true;
}

// return true if car let in/out, false otherwise
bool carDetected(char gateType, int gateNum, char[] plate){
    if(gateType == "entrance"){
        // check plate against hash table
        if (true){  // if plate is not in hash table
            // dipslay 'X' and don't let car in
            pthread_mutex_lock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
            (char *)(ptr + 280 + 288*(gateNum-1)) = "X";
            pthread_mutex_unlock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
            pthread_cond_signal((pthread_cond_t *)(ptr + 232 + 288*(gateNum-1)));
            return false;
        } else {    // plate is in hash table
            // check for space on each level
            if (true){  // level with space is found
                char suggestedLevel = 1;
                // display suggested level and let car in
                pthread_mutex_lock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
                (char *)(ptr + 280 + 288*(gateNum-1)) = suggestedLevel;
                pthread_mutex_unlock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
                pthread_cond_signal((pthread_cond_t *)(ptr + 232 + 288*(gateNum-1)));
                return OpenThenCloseBoomGate(gateType, gateNum);
            }
            else {  // level with space is not found
                // display 'F' and don't let car in
                pthread_mutex_lock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
                (char *)(ptr + 280 + 288*(gateNum-1)) = "F";
                pthread_mutex_unlock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
                pthread_cond_signal((pthread_cond_t *)(ptr + 232 + 288*(gateNum-1)));
                return false;
            }
        }
    } else if(gateType == "exit"){
        // open billing.txt in append mode
        // append plate along with car's bill (5 cents * time spent in ms)
        // let car out
        return OpenThenCloseBoomGate(gateType, gateNum);
    } else {
        printf("ERROR: invalid gate type");
        return false;
    }
}


void * entranceManager(int gateNum){
    pthread_cond_t condLPR = (pthread_cond_t *)(ptr + 40 + (gateNum-1)*288);
    pthread_mutex_t mutexLPR = (pthread_mutex_t *)(ptr + (gateNum-1)*288);
    char[] plate = (char[] *)(ptr + 88 + (gateNum-1)*288);

    // wait for LPR sensor to detect a car 
    while(true){
        pthread_cond_wait(condLPR, mutexLPR);
        carDetected("entrance", gateNum, plate);
    }   
}

void * exitManager(int gateNum){
    pthread_cond_t condLPR = (pthread_cond_t *)(ptr + 1480 + (gateNum-1)*192);
    pthread_mutex_t mutexLPR = (pthread_mutex_t *)(ptr + 1440 + (gateNum-1)*192);
    char[] plate = (char[] *)(ptr + 1528 + (gateNum-1)*192);

    // wait for LPR sensor to detect a car    
    while(true){
        pthread_cond_wait(condLPR, mutexLPR);
        carDetected("exit", gateNum, plate);
    } 
}


// car detected on a floor
// update carpark struct


// update screen (must be called 'frequently')
    // current capacity of each level
    // current status of each boom gate
    // current status of signs
    // current values of temperature sensors
    // current staus of alarms
    // current total revenue