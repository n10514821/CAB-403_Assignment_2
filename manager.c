#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
//#include <simulator1.c>
#include "mem.h"
#include "cars.c"
#include <string.h>
#include <math.h>

// -------------------------------------------------- vv GUI STUFF vv -----------------------------------------------------

#define Number_of_levels 5//Total amount of levels
#define Width 118 //Width of GUI
#define Height 4 *Number_of_levels -2*(Number_of_levels-6) -2 //Height of GUI 

#define X Width/2 //Middle of Width
#define Y Height/2 //Middle of Height 



//The following chars are the used to create the table headers
char  Table_Header_Entry_Boom_gate[]= {'|','E','n','t','r','y',' ','G','a','t','e'};
char  Table_Header_Exit_Boom_gate[]= {'|','E','x','i','t',' ','G','a','t','e'};
char  Table_Header_Entry_LPR[]= {'|','E','n','t','r','y',' ','L','P','R'};
char  Table_Header_Exit_LPR[]= {'|','E','x','i','t',' ','L','P','R'};
char  Table_Header_Digital_sign[]= {'|','D','i','g','i','t',' ','S','i','g','n'};
char  Table_Header_Number_of_cars[]= {'|','N','u','m','b','e','r',' ','O','f',' ','C','a','r','s'};
char  Table_Header_Car_Capacity[]= {'|','C','a','r',' ','C','a','p','a','c','i','t','y'};
char  Table_Header_State_of_fire_alarm[]= {'|','F','I','R','E',' ','A','L','A','R','M','|'};
char  Table_Header_status[]={'|',' ',' ','S','t','a','t','u','s',' ',' ','|' };
char  Table_Header_status_2[]={'|',' ','S','t','a','t','u','s',' ',' ','|' };
char  Table_Header_status_3[]={'|',' ','S','t','a','t','u','s',' ','|' };
char  Table_Header_Present_1[]={'|',' ',' ','i','n',' ','c','a','r','p','a','r','k',' ',' ','|'};
char  Table_Header_Present_2[]={'|',' ','o','f',' ','c','a','r','p','a','r','k',' ','|'};
char  Table_Header_Digital_sign_expression[]= {'|',' ','D','i','s','p','l','a','y',' ',' ','|'};
char  Table_Header_Temperature[] = {'T','e','m','p','e','r','a','t','u','r','e','|'};
char  Table_Header_Temperature_2[] = {'(','D',')','C','e','l','s','i','u','s',' ','|'};
char Total_revenue[] ={'T','O','T','A','L',' ','R','E','V','E','N','U','E',':','$'};

char  Table_Header_NP_reader[] ={'|',' ',' ','P','l','a','t','e',' ',' ','|' };;
char  Table_Header_NP_reader_2[]={'|',' ','P','l','a','t','e',' ',' ','|' };


char GUI[Height][Width];

void GUI_design(void);
void Show_GUI(void);


void GUI_design(){
int x,y;
for(y=0;y<Height;y++){
    for(x=0;x<Width;x++){
            GUI[y][x] = ' ';
    }
}
for(y=1;y<Height;y++){
    GUI[y][0] = '|';
    GUI[y][Width-1] = '|';
   }
for(x=0;x<Width;x++){
    GUI[0][x] = '-';
    GUI[Height-1][x] = '-';
   }
   
}



void GUI_Create_table(){

//Title 
char  Title[] ={'C','A','R',' ','P','A','R','K',' ','S','O','F','T','W','A','R','E'};
for(int i=0;i<sizeof(Title);i++){
  GUI[1][X-sizeof(Title)/2 + i] = Title[i];
   
}

//Set out table here




for(int i=1;i<9+sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity) +sizeof(Table_Header_State_of_fire_alarm)+sizeof(Table_Header_Temperature);i++) {
//This creates the table headers holders
if(i>8)
GUI[2][i] = '_';
GUI[5][i] = '-';
}
//This creates the table headers on the terminal 
for (int i=0;i<sizeof(Table_Header_Present_1);i++){

    if(i<sizeof(Table_Header_Entry_Boom_gate))
    GUI[3][i+9] =  Table_Header_Entry_Boom_gate[i];
    if(i<sizeof(Table_Header_status))
    GUI[4][i+9] =  Table_Header_status[i];
  

    if (i<sizeof(Table_Header_Exit_Boom_gate))
    GUI[3][i+9 +sizeof(Table_Header_Entry_Boom_gate)] =  Table_Header_Exit_Boom_gate[i];
    if(i<sizeof(Table_Header_status_2))
    GUI[4][i+9+sizeof(Table_Header_Entry_Boom_gate)] =  Table_Header_status_2[i];
   

    if(i<sizeof(Table_Header_Entry_LPR))
    GUI[3][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)] =  Table_Header_Entry_LPR[i];
    if(i<sizeof(Table_Header_NP_reader))
    GUI[4][i+9+sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)] =  Table_Header_NP_reader[i];
    

    if(i<sizeof(Table_Header_Exit_LPR))
    GUI[3][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)] =  Table_Header_Exit_LPR[i];
    if(i<sizeof(Table_Header_NP_reader_2))
    GUI[4][i+9+sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)] =  Table_Header_NP_reader_2[i];


    if(i<sizeof(Table_Header_Digital_sign))
    GUI[3][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)] =  Table_Header_Digital_sign[i];
    if(i<sizeof(Table_Header_status))
    GUI[4][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)] =  Table_Header_Digital_sign_expression[i];


    if(i<sizeof(Table_Header_Number_of_cars))
    GUI[3][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)] =  Table_Header_Number_of_cars[i];
    if(i<sizeof(Table_Header_Present_1))
    GUI[4][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)] =  Table_Header_Present_1[i];

    if(i<sizeof(Table_Header_Car_Capacity))
    GUI[3][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)] =  Table_Header_Car_Capacity[i];
    if(i<sizeof(Table_Header_Present_2))
    GUI[4][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)] =  Table_Header_Present_2[i];

    if(i<sizeof(Table_Header_State_of_fire_alarm))
    GUI[3][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)] =  Table_Header_State_of_fire_alarm[i];
    if(i<sizeof(Table_Header_status))
    GUI[4][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)] =  Table_Header_status[i];
    
    
    if(i<sizeof(Table_Header_Temperature))
    GUI[3][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+sizeof(Table_Header_State_of_fire_alarm)] =  Table_Header_Temperature[i];
    if(i<sizeof(Table_Header_Temperature_2))
    GUI[4][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+sizeof(Table_Header_State_of_fire_alarm)] =  Table_Header_Temperature_2[i];

    if(i<sizeof(Total_revenue))
    GUI[5 +Number_of_levels*2 +2][9+i]=Total_revenue[i];
}




//This creates the level 
int selected_level=1;
for(int k= 1; k<=Number_of_levels*2;k++){
char Print_Level[] = {'L','E','V','E','L',' ',selected_level+'0'}; 
if( k & 1==1){
    selected_level=selected_level+1; }


    else{//This is used to create a line under each level
    for(int i=1;i<9+sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity) +sizeof(Table_Header_State_of_fire_alarm)+ sizeof(Table_Header_Temperature);i++) {
    GUI[5+k][i] = '_';
    }
    }
    

 //Insert coloumns into table 
for(int i=0;i<sizeof(Print_Level);i++){
    if( k & 1==1){
    GUI[5+k][1+i] = Print_Level[i];
    GUI[5+k][9] ='|';
    GUI[5+k][9+sizeof(Table_Header_Entry_Boom_gate)] ='|';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)] ='|';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)] ='|';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)] ='|';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)] ='|';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)] ='|';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)] ='|';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+sizeof(Table_Header_State_of_fire_alarm)-1]  ='|';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+sizeof(Table_Header_State_of_fire_alarm)+ sizeof(Table_Header_Temperature)-1]  ='|';
    

    }
}

}









}
 
//This is for inserting data
//int level,char entry_gate,char exit_gate,char entry_LPR[],char exit_LPR[], char Digital_sign,int Number_of_cars,int Car_capacity,char Fire_alarm, int16_t Temperature
void Insert_data_into_table(int level,char entry_gate,char exit_gate,char entry_LPR[],char exit_LPR[], char Digital_sign[],int Number_of_cars,int Car_capacity,char Fire_alarm[], int16_t Temperature ){
//set level

level = 4+level*2;

//entry gate status
GUI[level][10+4] =entry_gate;


//exit gate status
GUI[level][9+sizeof(Table_Header_Entry_Boom_gate)+5] =exit_gate;

//entry_LPR
//The reason for not using a for loop is due to moving the coloumn for some reason
if(entry_LPR!=NULL){
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+2] =entry_LPR[0];
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+3] =entry_LPR[1];
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+4] =entry_LPR[2];
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+5] =entry_LPR[3];
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+6] =entry_LPR[4];
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+7] =entry_LPR[5];
}

//exit LPR

if(exit_LPR!=NULL){
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+2] =exit_LPR[0];
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+3] =exit_LPR[1];
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+4] =exit_LPR[2];
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+5] =exit_LPR[3];
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+6] =exit_LPR[4];
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+7] =exit_LPR[5];
}


if(Digital_sign!=NULL){
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+5] =Digital_sign[0];
}


//# of cars

int number_of_digits = floor(log10(abs(Number_of_cars))) + 1;
char Nboc_convert_to_char[number_of_digits];
sprintf (Nboc_convert_to_char, "%i", Number_of_cars) ; 

for(int i=0;i<number_of_digits;i++){
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+6+i] =Nboc_convert_to_char[i];
}
 

 //capacity of cars
int number_of_digits_car_capacity = floor(log10(abs(Car_capacity))) + 1;
char coc_convert_to_char[number_of_digits_car_capacity];
sprintf (coc_convert_to_char, "%i", Car_capacity) ; 

for(int i=0;i<number_of_digits_car_capacity;i++){
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+5+i] =coc_convert_to_char[i];
}

//Fire_alarm
int size_of_entry = sizeof(Fire_alarm);
if(size_of_entry-1 >1){
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+5] =Fire_alarm[0];
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+6] =Fire_alarm[1];
}
else{
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+5] =Fire_alarm[0];

}



// Temperature
int number_of_digits_Temp = floor(log10(abs(Temperature))) + 1;
char Temp_convert_to_char[number_of_digits_Temp];
sprintf (Temp_convert_to_char, "%i", Temperature) ; 
for(int i=0;i<number_of_digits_Temp;i++){
GUI[level][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+sizeof(Table_Header_State_of_fire_alarm)+4+i]  =Temp_convert_to_char[i];
}

}






//float revenue
void Total_revenue_GUI(float revenue){

int number_of_digits_TR = floor(log10(abs(revenue))) + 1;
char TR_convert_to_char[number_of_digits_TR];
sprintf (TR_convert_to_char, "%f", revenue) ; 

for(int i=0;i<number_of_digits_TR+3;i++){
GUI[5 +Number_of_levels*2 +2][9+sizeof(Total_revenue)+i]=TR_convert_to_char[i];
}
}





void Show_GUI(){
int x,y;

    for(y=0;y<Height;y++)
    {
        for(x=0;x<Width;x++){
            putchar(GUI[y][x]);
        }
        putchar('\n');
    }




}

// --------------------------------------------  ^^ GUI Stuff ^^ -----------------------------------------------

#define Entrance 1
#define Exit 0

bool evacuate = false;


carpark_t* carpark1;


shared_memory_t shm;



char textplates[100][7];

pthread_mutex_t levellock[levels];


int carsinlevel[levels];


pthread_mutex_t parkingStore;


//borrowed from stackoverflow

int randomIntGenerator(int min, int max){

    int randomNumber = (rand() % (max - min + 1)) + min;

    return randomNumber;

}


void initialisecarpark(carpark_t *carpark_space){
    carpark_space->s = 0;

    for (int i =0; i<100; i++){
        strcpy(carpark_space->car[i].licensePlate, "blank");
        carpark_space->car[i].enter = 0;
        carpark_space->car[i].park = 0;
        carpark_space->car[i].exit = 0;
        carpark_space->car[i].level = 1;
        carpark_space->car[i].lpr_index = 0;
    }
}

void evacmessage(char letter){

    //called as evacmessage(e), evacmessage(v), evacmessage(a) and so on

    for (int i = 0; i < 5; i++){

        pthread_mutex_lock(&shm.data->entrance[i].sign.lock);

        shm.data->entrance[i].sign.display = letter;

        pthread_mutex_unlock(&shm.data->entrance[i].sign.lock);

    }

    usleep(200000);

}

void firedetect(void){

    for(int i = 1; i <= 5; i++){

        if (shm.data->level[i].alarm == 1){

            evacuate = true;

        }

    }

}



void opencarparkup(){

    for (int i = 1; i <= entrances; ){

        pthread_mutex_lock(&shm.data->entrance[i].boomGateEn.lock);

        shm.data->entrance[i].boomGateEn.status ='R';

        pthread_mutex_unlock(&shm.data->entrance[i].boomGateEn.lock);

    usleep(10000);

        pthread_mutex_lock(&shm.data->entrance[i].boomGateEn.lock);

        shm.data->entrance[i].boomGateEn.status ='O';

        pthread_mutex_unlock(&shm.data->entrance[i].boomGateEn.lock);



        i++;

    }


    for (int i = 0; i < levels;){

        pthread_mutex_lock(&levellock[i]);

        carsinlevel[i] = 0;

        pthread_mutex_unlock(&levellock[i]);

        i++;

    }

}




void carUpdate(carpark_t* carpark_space, char* licensePlate, clock_t enter, clock_t park, int level){

    int capacity_update = carpark_space->s;

    memcpy(carpark_space->car[capacity_update].licensePlate, licensePlate, 6+1);

    carpark_space->car[capacity_update].enter = enter;

    carpark_space->car[capacity_update].park = park;

    carpark_space->car[capacity_update].level = level;

    carpark_space->car[capacity_update].exit = 0;

    carpark_space->car[capacity_update].lpr_index = 0;

    carpark_space->s = capacity_update + 1;

}

void carLeaves(carpark_t* carpark_space, char *licensePlate){

    int capacity_update = carpark_space->s;

    car_t old_car[100];

    int k;

    for (int i = 0; i < capacity_update; i++){
        memcpy(old_car[i].licensePlate, carpark_space->car[i].licensePlate, 6+1);
        old_car[i].enter = carpark_space->car[i].enter;
        old_car[i].park = carpark_space->car[i].park;
        old_car[i].level = carpark_space->car[i].level;
        old_car[i].exit = carpark_space->car[i].exit;
        old_car[i].lpr_index = carpark_space->car[i].lpr_index;
    }

    for (int i = 0; i < capacity_update; i++){
        if (memcmp(carpark_space->car[i].licensePlate, licensePlate, 6+1) == 0){
            k = i;
            break;
        }
    }

    for (int i = 0; i < k; i++){
        memcpy(carpark_space->car[i].licensePlate, old_car[i].licensePlate,6+1);
        carpark_space->car[i].enter = old_car[i].enter;
        carpark_space->car[i].park = old_car[i].park;
        carpark_space->car[i].level = old_car[i].level;
        carpark_space->car[i].exit = old_car[i].exit;
        carpark_space->car[i].lpr_index = old_car[i].lpr_index;
    }

    for (int i = k; i < capacity_update - 1; i++){
        memcpy(carpark_space->car[i].licensePlate, old_car[i + 1].licensePlate,6+1);
        carpark_space->car[i].enter = old_car[i + 1].enter;
        carpark_space->car[i].park = old_car[i + 1].park;
        carpark_space->car[i].level = old_car[i + 1].level;
        carpark_space->car[i].exit = old_car[i + 1].exit;
        carpark_space->car[i].lpr_index = old_car[i + 1].lpr_index;
    }

    carpark_space->s = capacity_update - 1;
}


//opening shared memory for manager
bool create_shared_object_R( shared_memory_t* shm, const char* share_name ) {

    //assigning share name to shm->name
    shm->name = share_name;

    //create shared mem object
    if ((shm->fd = shm_open(share_name, O_RDWR, 0)) < 0){
        shm->data = NULL;
        return false;
    }

    //map mem segment using mmap
    if ((shm->data = mmap(0, sizeof(shared_carpark_t), PROT_WRITE, MAP_SHARED, shm->fd, 0)) == (void *)-1)
    {
        return false;
    }

    //if all of the above worked return true
    return true;
}


void OpenEntranceBoomGate(void *arg){

    // Setup Variables for accessing memory

    int i = *(int*) arg;

    while(evacuate == true){

        evacmessage('E');

        evacmessage('V');

        evacmessage('A');

        evacmessage('C');

        evacmessage('U');

        evacmessage('A');

        evacmessage('T');

        evacmessage('E');

        firedetect();

        return 0;

    }

    while(evacuate != true){

        pthread_mutex_lock(&shm.data->entrance[i].boomGateEn.lock);

        while (shm.data->entrance[i].boomGateEn.status == 'C')

        {

            pthread_cond_wait(&shm.data->entrance[i].boomGateEn.condition, &shm.data->entrance[i].boomGateEn.lock);

        }

        pthread_mutex_unlock(&shm.data->entrance[i].boomGateEn.lock);


        //setting gate open
        pthread_mutex_lock(&shm.data->entrance[i].boomGateEn.lock);

        shm.data->entrance[i].boomGateEn.status = 'O';

        pthread_mutex_unlock(&shm.data->entrance[i].boomGateEn.lock);

        pthread_cond_signal(&shm.data->entrance[i].boomGateEn.condition);



        //wait for car to enter

        usleep(20000);



        pthread_mutex_lock(&shm.data->entrance[i].boomGateEn.lock);

        shm.data->entrance[i].boomGateEn.status = 'C';

        pthread_mutex_unlock(&shm.data->entrance[i].boomGateEn.lock);

    }

}

void OpenExitBoomGate(void *arg){

    // Setup Variables for accessing memory

    int i = *(int*) arg;



    while(evacuate == true){

        evacmessage('E');

        evacmessage('V');

        evacmessage('A');

        evacmessage('C');

        evacmessage('U');

        evacmessage('A');

        evacmessage('T');

        evacmessage('E');

        firedetect();

        return 0;

    }



    while(evacuate != true){

        pthread_mutex_lock(&shm.data->exit[i].boomGateEx.lock);

        while (shm.data->exit[i].boomGateEx.status == 'C')

        {

            pthread_cond_wait(&shm.data->exit[i].boomGateEx.condition, &shm.data->exit[i].boomGateEx.lock);

        }

        pthread_mutex_unlock(&shm.data->exit[i].boomGateEx.lock);

        
        //setting gate open

        pthread_mutex_lock(&shm.data->exit[i].boomGateEx.lock);

        shm.data->exit[i].boomGateEx.status = 'O';

        pthread_mutex_unlock(&shm.data->exit[i].boomGateEx.lock);

        pthread_cond_signal(&shm.data->exit[i].boomGateEx.condition);



        //wait for car to enter

        usleep(20000);



        pthread_mutex_lock(&shm.data->exit[i].boomGateEx.lock);

        shm.data->exit[i].boomGateEx.status = 'C';

        pthread_mutex_unlock(&shm.data->exit[i].boomGateEx.lock);

    }

}

void e_lpr(void *arg){

    int i = *(int*) arg;

    while (evacuate != true){

        pthread_mutex_lock(&shm.data->entrance[i].lpr.lock);

        strcpy(shm.data->entrance[i].lpr.licensePlate, "000000");

        pthread_mutex_unlock(&shm.data->entrance[i].lpr.lock);

        pthread_cond_signal(&shm.data->entrance[i].lpr.condition);

        pthread_mutex_lock(&shm.data->entrance[i].lpr.lock);
        //waiting for a car to show up

        while (strcmp(shm.data->entrance[i].lpr.licensePlate, "000000") != true){

            pthread_cond_wait(&shm.data->entrance[i].lpr.condition, &shm.data->entrance[i].lpr.lock);

        }
        pthread_mutex_unlock(&shm.data->entrance[i].lpr.lock);
        int car_allowed = 0;
        int k = 1;

        for(k; k < 101; k++){

        pthread_mutex_lock(&shm.data->entrance[i].lpr.lock);

        int getplateatindex = 0;

        getplateatindex = strcmp(shm.data->entrance[i].lpr.licensePlate, textplates[i]);

        pthread_mutex_unlock(&shm.data->entrance[i].lpr.lock);

        if (getplateatindex == 1){

            car_allowed = 1;

            break;
        }
        }

        int cap = 0;

        if (car_allowed != 0){

            pthread_mutex_lock(&parkingStore);

            cap = carpark1->s;

            pthread_mutex_unlock(&parkingStore);



            while(cap == 100){

                car_allowed = 1;

            }

        }
        if(car_allowed != 1){

        }

        clock_t parkingduration = (clock_t)randomIntGenerator(100, 10000) *1000;

        pthread_mutex_lock(&levellock);

        pthread_mutex_lock(&shm.data->entrance[i].lpr.lock);

        carUpdate(&carpark1, shm.data->entrance->lpr.licensePlate, clock(), parkingduration, i);

        pthread_mutex_unlock(&levellock);

        pthread_mutex_unlock(&shm.data->entrance[i].lpr.lock);

    }


}


// return true if car let in/out, false otherwise
/* bool carDetected(int gateType, int gateNum, char plate[]){
    if(gateType == Entrance){
        // check plate against hash table
        if (true){  // if plate is not in hash table
            // dipslay 'X' and don't let car in
            pthread_mutex_lock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
            (char *)(ptr + 280 + 288*(gateNum-1)) = "X";
            pthread_mutex_unlock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
            pthread_cond_signal((pthread_cond_t *)(ptr + 232 + 288*(gateNum-1)));
            return false;
        } else {    // plate is in hash table
            // check for space on each level
            if (true){  // level with space is found
                char suggestedLevel = 1;
                // display suggested level and let car in
                pthread_mutex_lock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
                (char *)(ptr + 280 + 288*(gateNum-1)) = suggestedLevel;
                pthread_mutex_unlock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
                pthread_cond_signal((pthread_cond_t *)(ptr + 232 + 288*(gateNum-1)));
                return OpenThenCloseBoomGate(gateType, gateNum);
            }
            else {  // level with space is not found
                // display 'F' and don't let car in
                pthread_mutex_lock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
                (char *)(ptr + 280 + 288*(gateNum-1)) = "F";
                pthread_mutex_unlock((pthread_cond_t *)(ptr + 192 + (gateNum-1)*288));
                pthread_cond_signal((pthread_cond_t *)(ptr + 232 + 288*(gateNum-1)));
                return false;
            }
        }
    } else if(gateType == Exit){
        // open billing.txt in append mode
        // append plate along with car's bill (5 cents * time spent in ms)
        // let car out
        return OpenThenCloseBoomGate(gateType, gateNum);
    } else {
        printf("ERROR: invalid gate type");
        return false;
    }
} */


void * entranceManager(int gateNum){
    pthread_cond_t condLPR = (pthread_cond_t *)(ptr + 40 + (gateNum-1)*288);
    pthread_mutex_t mutexLPR = (pthread_mutex_t *)(ptr + (gateNum-1)*288);
    char plate[] = (char *)(ptr + 88 + (gateNum-1)*288);

    // wait for LPR sensor to detect a car 
    while(true){
        pthread_cond_wait(condLPR, mutexLPR);
        carDetected(Entrance, gateNum, plate);
    }   
}

void * exitManager(int gateNum){
    pthread_cond_t condLPR = (pthread_cond_t *)(ptr + 1480 + (gateNum-1)*192);
    pthread_mutex_t mutexLPR = (pthread_mutex_t *)(ptr + 1440 + (gateNum-1)*192);
    char plate[] = (char[] *)(ptr + 1528 + (gateNum-1)*192);

    // wait for LPR sensor to detect a car    
    while(true){
        pthread_cond_wait(condLPR, mutexLPR);
        carDetected(Exit, gateNum, plate);
    } 
}

void * levelManager(int levelNum){
    pthread_cond_t condLPR = (pthread_cond_t *)(ptr + 2440 + (levelNum-1)*104);
    pthread_mutex_t mutexLPR = (pthread_mutex_t *)(ptr + 2400 + (levelNum-1)*104);

    // wait for sensor to detect car on a level
    while(true){
        pthread_cond_wait(condLPR, mutexLPR);
        if(true){ // car is entering that level
            carUpdate();
            // update carpark struct (add car to level)
        } else{ // car is leaving that level
            carLeaves();
        }
    } 
 
}

void * guiManager(){
    int level;
    char entry_gate;
    char exit_gate;
    char entry_LPR[1];
    char exit_LPR[1];
    char Digital_sign[1];
    int Number_of_cars;
    int Car_capacity;
    char Fire_alarm[1];
    int16_t Temperature;

    while(true){
    // update screen (must be called 'frequently')
        system("clear");
        GUI_design();
        GUI_Create_table();
        for(int i=1; i <= Number_of_levels; i++){
            level = i;
            entry_gate = "A";
            exit_gate = "B";
            entry_LPR[1] = "C";
            exit_LPR[1] = "D";
            Digital_sign[1] = "E";
            Number_of_cars = 42;
            Car_capacity = 42;
            Fire_alarm[1] = "F";
            Temperature = 42;
            Insert_data_into_table(level,entry_gate,exit_gate,entry_LPR,exit_LPR,Digital_sign,Number_of_cars,Car_capacity,Fire_alarm,Temperature);
        }
        int revenue = 100;
        Total_revenue_GUI(revenue);
        Show_GUI();
        sleep(0.05); // refresh every 50ms
    }
}

void managerSetup(){
    // read plates.txt into hash table
    // create billing.txt
    // open PARKING segement
        // if PARKING not present
            // print "ERROR: no parking segement found"

    // create manager threads
    int err = pthread_create(NULL, NULL, &entranceManager, NULL);
    pthread_create(NULL, NULL, entranceManager(2), NULL);
    pthread_create(NULL, NULL, entranceManager(3), NULL);
    pthread_create(NULL, NULL, entranceManager(4), NULL);
    pthread_create(NULL, NULL, entranceManager(5), NULL);
    pthread_create(NULL, NULL, exitManager(1), NULL);
    pthread_create(NULL, NULL, exitManager(2), NULL);
    pthread_create(NULL, NULL, exitManager(3), NULL);
    pthread_create(NULL, NULL, exitManager(4), NULL);
    pthread_create(NULL, NULL, exitManager(5), NULL);
    pthread_create(NULL, NULL, levelManager(1), NULL);
    pthread_create(NULL, NULL, levelManager(2), NULL);
    pthread_create(NULL, NULL, levelManager(3), NULL);
    pthread_create(NULL, NULL, levelManager(4), NULL);
    pthread_create(NULL, NULL, levelManager(5), NULL);
    pthread_create(NULL, NULL, guiManager(), NULL);
}
