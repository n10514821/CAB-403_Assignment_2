#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

#define Number_of_levels 5//Total amount of levels
#define Width 118 //Width of GUI
#define Height 4 *Number_of_levels -2*(Number_of_levels-6) -2 //Height of GUI 

#define X Width/2 //Middle of Width
#define Y Height/2 //Middle of Height 





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
char  Table_Header_Present_2[]={'|',' ','o','f',' ','c','a','r','p','a','r','k',' ','|'};

char  Table_Header_Digital_sign_expression[]= {'|',' ','D','i','s','p','l','a','y',' ',' ','|'};

char  Table_Header_Temperature[] = {'T','e','m','p','e','r','a','t','u','r','e','|'};
char  Table_Header_Temperature_2[] = {'(','D',')','C','e','l','s','i','u','s',' ','|'};
char Total_revenue[] ={'T','O','T','A','L',' ','R','E','V','E','N','U','E',':','$'};

for(int i=1;i<9+sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity) +sizeof(Table_Header_State_of_fire_alarm)+sizeof(Table_Header_Temperature);i++) {
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



//

int selected_level=1;
for(int k= 1; k<=Number_of_levels*2;k++){
char Print_Level[] = {'L','E','V','E','L',' ',selected_level+'0'}; 
if( k & 1==1){
    selected_level=selected_level+1; }


    else{
    for(int i=1;i<9+sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity) +sizeof(Table_Header_State_of_fire_alarm)+ sizeof(Table_Header_Temperature);i++) {
    GUI[5+k][i] = '_';
    }
    }
    

 //Insert coloumns and data into table 
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
    
    
    //INSERT DATA INTO COLOUMNS 


    //entry gate status
    
    GUI[5+k][10+4] ='t';
  
    //exit gate status
    
    GUI[5+k][9+sizeof(Table_Header_Entry_Boom_gate)+5] ='s';
    

    //entry LPR
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+2] ='a';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+3] ='a';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+4] ='a';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+5] ='a';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+6] ='a';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+7] ='a';

    //exit LPR
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+2] ='x';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+3] ='x';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+4] ='x';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+5] ='x';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+6] ='x';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+7] ='x';

    //digital sign
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+5] ='v';



    //# of cars
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+6] ='e';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+7] ='e';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+8] ='e';



    //Capacity of cars
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+5] ='k';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+6] ='k';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+7] ='k';



    //Fire Alarm
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+5] ='m';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+6] ='m';
 


    //Temperature
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+sizeof(Table_Header_State_of_fire_alarm)+4]  ='h';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+sizeof(Table_Header_State_of_fire_alarm)+5]  ='h';
    GUI[5+k][9 +sizeof(Table_Header_Entry_Boom_gate)+sizeof(Table_Header_Exit_Boom_gate)+sizeof(Table_Header_Entry_LPR)+ sizeof(Table_Header_Exit_LPR)+sizeof(Table_Header_Digital_sign)+sizeof(Table_Header_Number_of_cars)+sizeof(Table_Header_Car_Capacity)+sizeof(Table_Header_State_of_fire_alarm)+6]  ='h';


    //total revenue:
    GUI[5 +Number_of_levels*2 +2][9+sizeof(Total_revenue)]='t';
    GUI[5 +Number_of_levels*2 +2][9+sizeof(Total_revenue)+1]='t';
    GUI[5 +Number_of_levels*2 +2][9+sizeof(Total_revenue)+2]='.';
    GUI[5 +Number_of_levels*2 +2][9+sizeof(Total_revenue)+3]='t';
    GUI[5 +Number_of_levels*2 +2][9+sizeof(Total_revenue)+4]='t';
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
    int starter=1;
    while (starter=1)
    {
    system("clear");
    GUI_design();
    GUI_Insert_info();
    Show_GUI();
    sleep(50);//50 milliseconds
    }
    



}