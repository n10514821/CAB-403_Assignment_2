//Headers


//libaries
#include <stdio.h>
#include <stdlib.h>


//Car parking levels variables
#define Number_of_levels 5
#define Number_of_parks_per_level 20



//Boom gate variables (all in milliseconds)
#define Boom_gate_fully_opened_time 10
#define Boom_gate_fully_closed_time 10
#define Boom_gate_opened_for_time 20


//Car variables

#define Cars_in_queue  //alter this to form a list

//all in milliseconds
#define Front_of_queue_time 2
#define Driving_to_park_time 10
#define Driving_to_exit_time 10


//temperature and fire alram variables
#define Temperature_collecting_time 2 //in milliseconds
#define Last_five_Temperature_values //alter this to form a list (keep 5)
#define 30_Smoothed_Temperatures //alter this to form a list (keep 30)
#define Fire_Alarm_alter_time 20 //in milliseconds




//Read file;



//open file 
file(){
	FILE *fp;
	fp =fopen("plates.txt","r");
	if(fp == NULL)
	{
		printf("Error");
	}	
	
	
	
	
	
	int fclose( FILE *fp );
}



//main 
main(){
	file;
	
	
}












