//libaries
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <stdbool.h>
#include <math.h>
#include <sys/shm.h> 
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>

//complied with -lrt
//gcc Temperature_Sim.c -o Temperature_Sim -lrt


//temperature and fire alram variables
#define Temperature_collecting_time 2 //in milliseconds
#define Temperature_storage_list_capacity  5 //This is the max amount of values the list is ment to hold 
#define Smoothed_Temperatures_list_capacity 30 //This is the max amount of values the list is ment to hold 


#define fire_alarm_trigger 0 //if changed, fire alarm is triggered

//Include header files

#include "Temperature_header.h"

//This code creates the two list which hold the collected temperature
// and the  calculated temperature 
void List_creation( Combined_Temperature_list_t* vec ) {

   vec->Collected_Temperature_capacity= Temperature_storage_list_capacity;
   vec->Collected_Temperature_size=0;
   vec->Collected_Temperature_storage= malloc(0);

   vec->Smoothe_Temperature_capacity= Smoothed_Temperatures_list_capacity;
   vec->Smoothe_Temperature_size=0;
   vec->Smoothe_Temperature_storage=malloc(50);
   
}


//This code is used to generate a random temperature 
int16_t generate_temperature(){
 
  int16_t random_temperture =rand() % 46+20; //does not say which 
  //temps are allowed but states it is stored in int16_t. used its value
  printf("%d generated number \n",random_temperture);
  return random_temperture;
}

//This code is used to place code into lists
int Temperature_System (Combined_Temperature_list_t* vec, int16_t Temperature ){
//List variables
 //size_t Current_collected_temperature_length = vec->Collected_Temperature_size;

 size_t New_collected_temperature_length = vec->Collected_Temperature_size +1;
 int16_t* Current_collected_temperature_data = vec->Collected_Temperature_storage;


// This section of code will focus on adding the collected temperature to
//the list

//This section is when the array begins
if(New_collected_temperature_length ==1){
    
vec->Collected_Temperature_storage[0]=Temperature;
vec->Collected_Temperature_size=New_collected_temperature_length;
printf("printing array\n");
printf("%d \n",vec->Collected_Temperature_storage[0]);
 printf("\n");
}


//This section is when the array is at its max length
else if(New_collected_temperature_length>=Temperature_storage_list_capacity){
    New_collected_temperature_length=Temperature_storage_list_capacity;

for(size_t i=New_collected_temperature_length;i>=1;i-- ){
     
vec->Collected_Temperature_storage[i]=Current_collected_temperature_data[i-1];

}
vec->Collected_Temperature_storage[0]=Temperature;

vec->Collected_Temperature_size=Temperature_storage_list_capacity;
printf("printing array\n");
for (size_t i = 0; i <= New_collected_temperature_length -1; i++)
{ 
    printf("%d \n",vec->Collected_Temperature_storage[i]);
}

 printf("\n");
}


//This section is when the array is not at its max length and has
//a vaule in it.
else {
    
    for(size_t i=New_collected_temperature_length;i>=1;i-- ){
    vec->Collected_Temperature_storage[i]=Current_collected_temperature_data[i-1];
    
}
vec->Collected_Temperature_storage[0]=Temperature;
vec->Collected_Temperature_size=New_collected_temperature_length;
 printf("printing array\n");
for (size_t i = 0; i <= New_collected_temperature_length -1; i++)
{ 
    printf("%d \n",vec->Collected_Temperature_storage[i]);
}
 printf("\n");
}






//This section now focues on creating the smooth temperature and placing
//in a List 
if (vec->Collected_Temperature_size >= Temperature_storage_list_capacity){
   
//varaibles

size_t Current_smoothe_temperature_length = vec->Smoothe_Temperature_size;

size_t New_smoothe_temperature_length =vec->Smoothe_Temperature_size+1;
int16_t* Current_smoothe_temperature_data = vec->Smoothe_Temperature_storage;


//calculate the smooth temperature
int16_t sum_of_CT_array =0;
for(int i=0;i<Temperature_storage_list_capacity ;i++){
 sum_of_CT_array=sum_of_CT_array+ Current_collected_temperature_data[i];
}
int16_t average_of_CT_array=sum_of_CT_array/Temperature_storage_list_capacity;
//This section of code is used to place the smooth temp in the array
printf("ST value is %d \n",average_of_CT_array);
printf("\n");

if (New_smoothe_temperature_length ==1){
    
     vec->Smoothe_Temperature_storage[0]=average_of_CT_array;
     printf("%d is ST \n", vec->Smoothe_Temperature_storage[0]);
     vec->Smoothe_Temperature_size = New_smoothe_temperature_length;
     printf("\n");
}



else if(New_smoothe_temperature_length<Smoothed_Temperatures_list_capacity) {
   
    for(size_t i=New_smoothe_temperature_length;i>=1;i-- ){
    vec->Smoothe_Temperature_storage[i]=Current_smoothe_temperature_data[i-1];
    
}
vec->Smoothe_Temperature_storage[0]=average_of_CT_array;
vec->Smoothe_Temperature_size=New_smoothe_temperature_length;

for (size_t i = 0; i <= New_smoothe_temperature_length -1; i++)
{ 
    printf("%d \n",vec->Smoothe_Temperature_storage[i]);
}
 printf("\n");
}

else if(New_smoothe_temperature_length >= Smoothed_Temperatures_list_capacity){ 
   
New_smoothe_temperature_length=Smoothed_Temperatures_list_capacity;

for(int i=New_smoothe_temperature_length; i>=1;i--){
vec->Smoothe_Temperature_storage[i]=Current_smoothe_temperature_data[i-1];
}
vec->Smoothe_Temperature_storage[0]=average_of_CT_array;
vec->Smoothe_Temperature_size = New_smoothe_temperature_length;
for (size_t i = 0; i <= New_smoothe_temperature_length -1; i++)
{ 
    printf("%d \n",vec->Smoothe_Temperature_storage[i]);
}
 printf("\n");
}

//Do fire alarms

//Fire alams 




//checking for fire dectection 2 (rate of rise)
if (New_smoothe_temperature_length >= Smoothed_Temperatures_list_capacity){
    printf("fire alarm armed \n");
int16_t* data = vec->Smoothe_Temperature_storage; 
if(data[Smoothed_Temperatures_list_capacity-1]-data[0] >=8 ){
    return 1;
}

//test


//Fixed temp   
double fire_trigger =0.9;
int fire_temp =58;
int Smooth_temps_above_fire_temp=0;
for(int i =0; i < Smoothed_Temperatures_list_capacity;i++){
if(data[i]>=fire_temp){
    Smooth_temps_above_fire_temp=Smooth_temps_above_fire_temp+1;
}
}
if(Smooth_temps_above_fire_temp/Smoothed_Temperatures_list_capacity >= fire_trigger){
return 1;
}

}

}
return 0;
}













int main(int argc, char const *argv[])
{


//fire alarm system_1 test 
/*
int16_t data_2[30];
for(int i=0;i<30;i++){
    data_2[i]=30;
}
data_2[0]=48;
printf("%d   %d\n",data_2[0],data_2[Smoothed_Temperatures_list_capacity-1]);
if(data_2[Smoothed_Temperatures_list_capacity-1]-data_2[0] <=-8 ){
    printf("fire!!!\n");
   
}
*/

//Fire alarm system_2 test
//Fixed temp   
/*
int16_t data_3[30];
for(int i=0;i<30;i++){
    data_3[i]=75;
}
double fire_trigger =0.9;
int fire_temp =58;
int Smooth_temps_above_fire_temp=0;
for(int i =0; i < Smoothed_Temperatures_list_capacity;i++){
if(data_3[i]>=fire_temp){
    Smooth_temps_above_fire_temp=Smooth_temps_above_fire_temp+1;
}
}
if(Smooth_temps_above_fire_temp/Smoothed_Temperatures_list_capacity >= fire_trigger){
printf("fire!!!/n");

}
*/



//we want to create a 2 byte long variable for level 1 which contains the ability 
sleep(2);

Combined_Temperature_list_t Level_1_Temperature;
List_creation(&Level_1_Temperature);

int starter =1;
while (starter=1)
{
int16_t Temperature_record= generate_temperature();

int test =Temperature_System(&Level_1_Temperature,Temperature_record);
sleep(2);

}






}