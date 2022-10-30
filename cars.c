#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h> 

typedef struct car{

    char licensePlate[7];

    clock_t enter;

    clock_t park;

    int level;

    int exit;

    int lpr_index;


    
}car_t;

typedef struct carpark{
    int s;

    car_t car[100];
}carpark_t;


