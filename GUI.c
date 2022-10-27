#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define Width 108 //Width of GUI
#define Height 20 //Height of GUI //THIS SHOULD BE CHANGED BASED OFF # OF LEVELS

#define X Width/2 //Middle of Width
#define Y Height/2 //Middle of Height 
#define Total_Number_of_levels 1//Total amount of levels

//Note need to expand the defines to include variables



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



void GUI_Insert_info(){

//Title 
char  Title[] ={'C','A','R',' ','P','A','R','K',' ','S','O','F','T','W','A','R','E'};
for(int i=0;i<sizeof(Title);i++){
  GUI[1][X-sizeof(Title)/2 + i] = Title[i];
   
}

//Set out table here
//Note
//1 entry and 1 exit for boom gate , LPR per level,digital sign for entry only
//# of cars per level and its capacity
//Current state of fire alarm 
//Total revenue 


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
char  Table_Header_Present_2[]={'|',' ','i','n',' ','c','a','r','p','a','r','k',' ','|'};

char Total_revenue[] ={'T','O','T','A','L',' ','R','E','V','E','N','U','E',':','$'};

for(int i=1;i<9+sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity) +sizeof(Table_Header_State_of_fire_alarm);i++) {
//This creates the table headers holders
if(i>8)
GUI[2][i] = '_';
GUI[5][i] = '-';
}
//This creates the table headers 
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
    if(i<sizeof(Table_Header_status_2))
    GUI[4][i+9+sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)] =  Table_Header_status_2[i];
    

    if(i<sizeof(Table_Header_Exit_LPR))
    GUI[3][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)] =  Table_Header_Exit_LPR[i];
    if(i<sizeof(Table_Header_status_3))
    GUI[4][i+9+sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)] =  Table_Header_status_3[i];


    if(i<sizeof(Table_Header_Digital_sign))
    GUI[3][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)] =  Table_Header_Digital_sign[i];
    if(i<sizeof(Table_Header_status))
    GUI[4][i+9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)] =  Table_Header_status[i];


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
    
    if(i<sizeof(Total_revenue))
    GUI[5 +12][9+i]=Total_revenue[i];
}


// For each level
//Note
//1 entry and 1 exit boom gate per level
//Note this should be changed to #define when incomporated
int Number_of_levels =5;

int selected_level=1;
for(int k= 1; k<=Number_of_levels*2;k++){
char Print_Level[] = {'L','E','V','E','L',' ',selected_level+'0'}; 
if( k & 1==1){
    selected_level=selected_level+1; }


    else{
    for(int i=1;i<9+sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity) +sizeof(Table_Header_State_of_fire_alarm);i++) {
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
    }
  
}

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
system("clear");

GUI_design();
GUI_Insert_info();
Show_GUI();


}