

#pragma once
//create list for storing last 30 Smoothed_Temperatures
#include <stdbool.h>
#include <math.h>
typedef struct Smoothed_Temperatures_storage_list{
	//size
	size_t ST_size;
	//Capacity
	size_t ST_capacity;
	//Store data
	int16_t* Smoothed_Temperatures_storage;

} Smoothed_Temperatures_storage_list_t;