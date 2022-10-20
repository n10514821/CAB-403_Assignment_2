#pragma once

typedef struct boomGate{
    pthread_mutex_t lock;
    pthread_cond_t condition;
    char status[1];
    char pad[7];


}boomGate;


struct lpr
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
    struct boomGate boomgate;
    struct sign sign;
    struct lpr lpr;

    char pad[5];
    
}level;

typedef struct entrance
{
    struct boomGate boomGate;
    struct lpr lpr;
    struct sign sign;

};

typedef struct exit
{
    struct boomGate boomGate;
    struct sign sign;
    
};

typedef struct PARKING
{
    /* data */

    struct entrance entrance;
    struct exit exit;
    struct level level;

    
}shared_carpark_t;

struct shm
{
    const char* name;

    int fd;

    struct PARKING parking;
    
};

