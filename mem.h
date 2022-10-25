#pragma once

#define entrances 5
#define levels 5
#define exits 5


typedef struct boomGate{
    pthread_mutex_t lock;
    pthread_cond_t condition;
    char status;
    char pad[7];


}boomGate;

typedef struct tempSense
{
    int temp;
}tempSense;

typedef struct alarm
{
    char on;
}alarm1;


typedef struct lpr
{
    pthread_mutex_t lock;
    pthread_cond_t condition;
    char licensePlate[6];
    char pad[2];

}lpr;

typedef struct sign
{
    pthread_mutex_t lock;
    pthread_cond_t condition;
    char display[1];
    char pad[7];
}sign;

typedef struct level
{
    lpr lpr;
    
    tempSense tempsense;

    alarm1 alarm;

    char pad[5];
    
}shared_level_t;

typedef struct entrance
{
    boomGate boomGateEn;
    lpr lpr;
    sign sign;

}shared_entrance_t;

typedef struct exit
{
    lpr lpr;
    boomGate boomGateEx;
    sign sign;
    
}shared_exit_t;

typedef struct PARKING
{
    /* data */

    shared_entrance_t entrance[entrances];
    shared_exit_t exit[exits];
    shared_level_t level[levels];

    
}shared_carpark_t;

typedef struct shm
{
    const char* name;

    int fd;

    shared_carpark_t* data;
    
}shared_memory_t;

