#include <stdio.h>
#include <string.h>



//This define should be rewritten 
//to match the size of the number of cars 
#define number_of_cars 10 

//Insert hash table containing the cars entering.

//create billing file
int Create_billing_file(){

FILE *fp; //Create file pointer
//Create file name
char *file_name = "Billing.txt.txt"; 
//Select file mode (append mode =a)
char *mode ="a";
//create file and open file
fp = fopen(file_name,mode);
fclose(fp); //Close file 
}



//Insert data into file 
int Create_Billing_data(char* number_plate, int Total_time){

FILE *fp; //Create file pointer
//Create file name
char *file_name = "Billing.txt.txt"; 
//Select file mode (append mode =a)
char *mode ="a";
//create file and open file
fp = fopen(file_name,mode);

//Not certain how we want to create time
//if you want it to be done though storing time
//time it exit carpark - time it entered * 5 cents
//or other method 
float cost = 00.00;
//convert time to float/
//  divide by 1000 to convert to s
cost = ((float)Total_time/1000) *5;

//Test for writing

fprintf(fp, "%s : $%0.2f \n",number_plate,cost);
fclose(fp); //Close file 




}

int main(){
Create_billing_file();
// YOU WILL need to take the number plate from hash tables
//and Find time 

//test 
char *number_plate = "123ABC";
int Total_time =10; // 100 = 100 milliseconds
Create_Billing_data(number_plate,Total_time);
}