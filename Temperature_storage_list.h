

#pragma once

#include <stdbool.h>
#include <math.h>

//create list for storing last 5 temperatures
typedef struct Temperature_storage_list{
	//size
	size_t TS_size;
	//Capacity
	size_t TS_capacity;
	//Store data
	int16_t* temperature_storage;

} Temperature_storage_list_t;



