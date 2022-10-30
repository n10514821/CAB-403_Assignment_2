#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

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








void main(){
    int starter=1;
//testing purposes
    int level=2;
    char entry_gate='A';
    char exit_gate ='O';
    char entry_LPR[] = "123ABC";
    char exit_LPR[]= "198CAT";
    char Digital_sign[]= "a";
    int Number_of_cars= 103;
    int Car_capacity =20;
    char Fire_alarm[]= "SB";//for Active ,SB for standby, NA for Not available
    int16_t Temperature= 42;
    float revenue =12.00;

    while (starter=1)
    {
    system("clear");
    GUI_design();
    GUI_Create_table();
    Insert_data_into_table(level,entry_gate,exit_gate,entry_LPR,exit_LPR,Digital_sign,Number_of_cars,Car_capacity,Fire_alarm,Temperature);
    Total_revenue_GUI(revenue);
    Show_GUI();
    sleep(50);//50 milliseconds
    }
    



}