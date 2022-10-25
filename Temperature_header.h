
//create list for storing all temperature values
#include <stdbool.h>
#include <math.h>
typedef struct Combined_Temperature_list{
    //Collected_Temperature
    size_t Collected_Temperature_size;
    //Capacity
    size_t Collected_Temperature_capacity;
	//Store data
	int16_t* Collected_Temperature_storage;

    //Calulated Temperater
	//size
	size_t Smoothe_Temperature_size;
	//Capacity
	size_t Smoothe_Temperature_capacity;
	//Store data
	int16_t* Smoothe_Temperature_storage;

} Combined_Temperature_list_t;