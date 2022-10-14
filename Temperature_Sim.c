//libaries
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> 
#include <stdbool.h>
#include <math.h>


//temperature and fire alram variables
#define Temperature_collecting_time 2 //in milliseconds
#define Temperature_storage_list_inital_capacity  5 //alter this to form a list (keep 5)
#define Smoothed_Temperatures_list_inital_capacity 30 //alter this to form a list (keep 30)
#define DV_GROWTH_FACTOR 2

#define Temperature_storage_limit  5
#define Smoothed_Temperatures_limit 30


//Include header files
#include "Temperature_storage_list.h"
#include "Smoothed_Temperatures_storage_list.h"


//This code initals a list to store the randomly generated temperature values
void TS_init( Temperature_storage_list_t* vec ) {
	vec->TS_capacity = Temperature_storage_list_inital_capacity;
	vec->TS_size = 0;
	
    vec->temperature_storage = malloc(100);
  
}

//This code pushs a randomly generated value into the first poistion of the list
void TS_push( Temperature_storage_list_t* vec, int16_t new_temp) {
    //sets variables 
    int16_t* old_data = vec->temperature_storage;
    size_t new_size = vec->TS_size +1; 
    size_t old_size = vec->TS_size; 

//This checks if the size of list is over 5
//if so it will remove the last value from the list 
    if (new_size>Temperature_storage_limit)
    {
       for(int i = old_size;i>=1;i--){
    vec->temperature_storage[i] = old_data[i-1];
     }
    vec->temperature_storage[0] = new_temp;
     }
    //This code is used to determine if it is first value to be added to the list
     else if(new_size == 1){
        vec->TS_size = new_size;
        vec->temperature_storage[0] = new_temp;
     }
     //This code is used there is already one value in the list and does not 
     //already contain five values 
    else{
    vec->TS_size = new_size;
     for(int i = old_size;i>=0;i--){
    vec->temperature_storage[i] = old_data[i-1];
     }
    vec->temperature_storage[0] = new_temp;
    }
}

//This prints all values sotrd in the list
void print_all_temps(Temperature_storage_list_t* vec){
size_t size = vec->TS_size;
for(int i=0;i<size;i++){
    printf("%d \n",vec->temperature_storage[i]);
}
}


//This code initals a list to store the calulated Smoothed_Temperatures
void ST_init( Smoothed_Temperatures_storage_list_t* vec ) {
    vec->ST_capacity = Smoothed_Temperatures_list_inital_capacity;
	vec->ST_size =0 ;
    vec->Smoothed_Temperatures_storage = malloc(100);
}

//Create and push Smooth Temperature
//input values are the vectors for ST and TS
void Calulate_and_Push_ST(Smoothed_Temperatures_storage_list_t* vec_1,Temperature_storage_list_t* vec_2){
//retrive and store data from each vector
int16_t* ST_storage = vec_1->Smoothed_Temperatures_storage;
int16_t* TS_storage= vec_2->temperature_storage;

size_t ST_storage_size = vec_1->ST_size;
size_t TS_storage_size= vec_2->TS_size;

//create average TS values
int16_t Smooth_Temp_value =0; //set inital value;
for(int i=0; i<Temperature_storage_limit;i++){
   Smooth_Temp_value=TS_storage[i]+Smooth_Temp_value;
}
//creates average value. Total values / number of values (the limit)
Smooth_Temp_value=Smooth_Temp_value/Temperature_storage_limit; 

if(vec_1->ST_size=0){
    printf("tes1t\n");
vec_1->Smoothed_Temperatures_storage[0] = Smooth_Temp_value;
vec_1->ST_size=1;
}
else if(ST_storage_size >=Smoothed_Temperatures_limit){
    printf("tes13t\n");
 for(int i = TS_storage_size;i>=1;i--){
    vec_1->Smoothed_Temperatures_storage[i] = ST_storage[i-1];
     }
    vec_1->Smoothed_Temperatures_storage[0] = Smooth_Temp_value;
    vec_1->ST_size= ST_storage_size +1;
}
else{
    printf("tes1t2\n");
    vec_1->ST_size = ST_storage_size+1;
     for(int i = TS_storage_size;i>=0;i--){
    vec_1->Smoothed_Temperatures_storage[i] = ST_storage[i-1];
     }
    vec_1->Smoothed_Temperatures_storage[0] = Smooth_Temp_value;
    
    }
}


//This prints all values sotrd in the list
void print_all_STtemps(Smoothed_Temperatures_storage_list_t* vec){
size_t size = vec->ST_size;
printf("%d this is size \n",size);
for(int i=0;i<size;i++){
    
    printf("%d \n",vec->Smoothed_Temperatures_storage[i]);
}
}


//generates a random temperature
int16_t generate_temperature(){
 // srand(time(0)); 
  int16_t random_temperture =rand() % 65; //does not say which 
  //temps are allowed but states it is stored in int16_t. used its value
  printf("%d generated number \n",random_temperture);
  return random_temperture;
}



//checking for fire dectection 1 (fixed temp)
int fixed_temp_alarm(Smoothed_Temperatures_storage_list_t* vec){
int16_t* data = vec->Smoothed_Temperatures_storage;
int holder =0;
for(int i=0; i<Smoothed_Temperatures_limit;i++){
     if(data[i] >= 58){
      holder= holder+1;
     }
     if(holder>=Smoothed_Temperatures_limit *0.9){
        //trigger fire alarm
        return 1;
     }
     return 0;
}
}


//checking for fire dectection 2 (rate of rise)
int Rate_Of_Rise_alarm(Smoothed_Temperatures_storage_list_t* vec){
int16_t* data = vec->Smoothed_Temperatures_storage; 
if(data[Smoothed_Temperatures_limit-1]-data[0] >=8 ){
    return 1;
}
return 0;

}


int main(int argc, char const *argv[])
{
	Temperature_storage_list_t vector;
	TS_init(&vector);

    Smoothed_Temperatures_storage_list_t vector_2;
    ST_init(&vector_2);


	TS_push(&vector,generate_temperature());
    printf("1st printed numbers\n");
    print_all_temps(&vector);
    printf("\n");

    TS_push(&vector,generate_temperature());
    printf("2nd printed numbers n");
    print_all_temps(&vector);
    printf("\n");

    TS_push(&vector,generate_temperature());
    printf("3rd printed numbers\n");
    print_all_temps(&vector);
    printf("\n");

    TS_push(&vector,generate_temperature());
    printf("4th printed numbers\n");
    print_all_temps(&vector);
    printf("\n");
    
    TS_push(&vector,generate_temperature());
    printf("5th printed numbers\n");
    print_all_temps(&vector);
    printf("\n");

   printf("calulate ST\n");
   Calulate_and_Push_ST(&vector_2,&vector);
   print_all_STtemps(&vector_2);
   printf("\n");
   


    TS_push(&vector,generate_temperature());
    printf("6th printed numbers\n");
    print_all_temps(&vector);
    printf("\n");

     printf("calulate ST\n");
   Calulate_and_Push_ST(&vector_2,&vector);
   print_all_STtemps(&vector_2);
   printf("\n");


  
}