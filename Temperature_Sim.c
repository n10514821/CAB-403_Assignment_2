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
 printf("This is the New_collected_temperature_length %d \n",New_collected_temperature_length);
 int16_t* Current_collected_temperature_data = vec->Collected_Temperature_storage;


// This section of code will focus on adding the collected temperature to
//the list

//This section is when the array begins
if(New_collected_temperature_length ==1){
    printf("s %d \n",New_collected_temperature_length);
vec->Collected_Temperature_storage[0]=Temperature;
vec->Collected_Temperature_size=New_collected_temperature_length;
printf("printing array\n");
printf("%d \n",vec->Collected_Temperature_storage[0]);
 printf("\n");
}


//This section is when the array is at its max length
else if(New_collected_temperature_length>=Temperature_storage_list_capacity){
    New_collected_temperature_length=Temperature_storage_list_capacity;
printf("E %d \n" ,New_collected_temperature_length);
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
    printf("Middle %d \n",New_collected_temperature_length);
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
    printf("Start of ST %d \n",New_smoothe_temperature_length);
     vec->Smoothe_Temperature_storage[0]=average_of_CT_array;
     printf("%d is ST \n", vec->Smoothe_Temperature_storage[0]);
     vec->Smoothe_Temperature_size = New_smoothe_temperature_length;
     printf("\n");
}



else if(New_smoothe_temperature_length<Smoothed_Temperatures_list_capacity) {
    printf("Middle %d \n",New_smoothe_temperature_length);
    for(size_t i=New_smoothe_temperature_length;i>=1;i-- ){
    vec->Smoothe_Temperature_storage[i]=Current_smoothe_temperature_data[i-1];
    
}
vec->Smoothe_Temperature_storage[0]=average_of_CT_array;
vec->Smoothe_Temperature_size=New_smoothe_temperature_length;
 printf("printing array\n");
for (size_t i = 0; i <= New_smoothe_temperature_length -1; i++)
{ 
    printf("%d \n",vec->Smoothe_Temperature_storage[i]);
}
 printf("\n");
}

else if(New_smoothe_temperature_length >= Smoothed_Temperatures_list_capacity){ 
     printf("End of ST %d \n",New_smoothe_temperature_length);
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







/*
void shared_memory(){
    const char *Level_one_fire_sensor_char = "Level_one_fire_sensor";
    const int Fire_sensor_level_one_byte_size =2;
    int Level_one_fire_sensor= shm_open(Level_one_fire_sensor_char, O_CREAT|O_RDWR , 0666);
    ftruncate(Level_one_fire_sensor, Fire_sensor_level_one_byte_size);
    shm_unlink(Level_one_fire_sensor_char); 

    const char *Level_one_fire_alarm_char = "Level_one_fire_alarm";
    const int Fire_alarm_level_one_byte_size =1;
    int Level_one_fire_alarm= shm_open(Level_one_fire_alarm_char, O_CREAT|O_RDWR , 0666);
    ftruncate(Level_one_fire_alarm, Fire_alarm_level_one_byte_size);
    shm_unlink(Level_one_fire_alarm_char);


    const char *Level_two_fire_sensor_char = "Level_two_fire_sensor";
    const int Fire_sensor_level_two_byte_size =2;
    int Level_two_fire_sensor= shm_open(Level_two_fire_sensor_char, O_CREAT|O_RDWR , 0666);
    ftruncate(Level_two_fire_sensor, Fire_sensor_level_two_byte_size);
    shm_unlink(Level_two_fire_sensor_char);

    const char *Level_two_fire_alarm_char = "Level_two_fire_alarm";
    const int Fire_alarm_level_two_byte_size =2;
    int Level_two_fire_alam= shm_open(Level_two_fire_alarm_char, O_CREAT|O_RDWR , 0666);
    ftruncate(Level_two_fire_alam, Fire_alarm_level_two_byte_size);
    shm_unlink(Level_two_fire_alarm_char);


    const char *Level_three_fire_sensor_char = "Level_three_fire_sensor";
    const int Fire_sensor_level_three_byte_size =2;
    int Level_three_fire_sensor= shm_open(Level_three_fire_sensor_char, O_CREAT|O_RDWR , 0666);
    ftruncate(Level_three_fire_sensor, Fire_sensor_level_three_byte_size);
    shm_unlink(Level_three_fire_sensor_char);

    const char *Level_three_fire_alarm_char = "Level_three_fire_alarm";
    const int Fire_alarm_level_three_byte_size =2;
    int Level_three_fire_alarm= shm_open(Level_three_fire_sensor_char, O_CREAT|O_RDWR , 0666);
    ftruncate(Level_three_fire_alarm, Fire_alarm_level_three_byte_size);
    shm_unlink(Level_three_fire_alarm_char);


    const char *Level_four_fire_sensor_char = "Level_four_fire_sensor";
    const int Fire_sensor_level_four_byte_size =2;
    int Level_four_fire_sensor= shm_open(Level_four_fire_sensor_char, O_CREAT|O_RDWR , 0666);
    ftruncate(Level_four_fire_sensor, Fire_sensor_level_four_byte_size);
    shm_unlink(Level_four_fire_sensor_char);

    const char *Level_four_fire_alarm_char = "Level_four_fire_alarm";
    const int Fire_alarm_level_four_byte_size =1;
    int Level_four_fire_alarm= shm_open(Level_four_fire_alarm_char, O_CREAT|O_RDWR , 0666);
    ftruncate(Level_four_fire_alarm, Fire_alarm_level_four_byte_size);
    shm_unlink(Level_four_fire_alarm_char);


    const char *Level_five_fire_sensor_char = "Level_five_fire_sensor";
    const int Fire_sensor_level_5_byte_size =2;
    int Level_five_fire_sensor= shm_open(Level_five_fire_sensor_char, O_CREAT|O_RDWR , 0666);
    ftruncate(Level_five_fire_sensor, Fire_sensor_level_5_byte_size);
    shm_unlink(Level_five_fire_sensor_char);
 

    const char *Level_five_fire_alarm_char = "Level_five_fire_alarm";
    const int Fire_alarm_level_5_byte_size =1;
    int Level_five_fire_alarm= shm_open(Level_five_fire_alarm_char, O_CREAT|O_RDWR , 0666);
    ftruncate(Level_five_fire_alarm, Fire_alarm_level_5_byte_size);
    shm_unlink(Level_five_fire_alarm_char);
    
}

*/
void print_all_Recored_temps(Combined_Temperature_list_t* vec){
size_t size = vec->Collected_Temperature_size;
for(int i=0;i<size;i++){
    printf("%d is recored temp\n",vec->Collected_Temperature_storage[i]);
}
}
void print_all_Smooth_temps(Combined_Temperature_list_t* vec){
size_t size = vec->Smoothe_Temperature_size;
for(int i=0;i<size;i++){
    printf("%d is smooth temp\n",vec->Smoothe_Temperature_storage[i]);
}
}






int main(int argc, char const *argv[])
{


//we want to create a 2 byte long variable for level 1 which contains the ability 
Combined_Temperature_list_t Level_1_Temperature;
List_creation(&Level_1_Temperature);
/*

 const char *Test = "Test";
    const int Fire_sensor_level_one_byte_size =2;
    int Level_one_fire_sensor= shm_open(Level_one_fire_sensor_char, O_CREAT|O_RDWR , 0666);
    ftruncate(Level_one_fire_sensor, Fire_sensor_level_one_byte_size);
    size_t test =2 ;
   void *mmap(NULL,test, PROT_READ | PROT_WRITE, MAP_SHARED, test, 0);



*/


int starter =1;
while (starter <36)
{
int16_t Temperature_record= generate_temperature();

Temperature_System(&Level_1_Temperature,Temperature_record);


if(starter >=6){
 
}

starter=starter+1;
  
}



}