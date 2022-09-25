
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
//generates random licence plate numbers everytime it is run
#define LENGTH_LICENCEPLATE 5

time_t delay_ms(int seconds)
{
    int milli_sec = 1000 *seconds;

    clock_t start = clock();
    while(clock() < start + milli_sec);
}

void generate_plate_number()
{
    char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char num[] = "0123456789"; 
    for(int i = 0; i <= LENGTH_LICENCEPLATE; i ++)
    {
        char number = num[rand() % (sizeof num - 1)];
        char alphabet = alpha[rand() % (sizeof alpha - 1)];
        if(i <= 2)
        {
            printf("%c", number);
    
        }
        else
        {
            printf("%c",alphabet);
    
        }
    }
    printf("\n");
}


int main (void) 
{
    srand((time(NULL)));
    // generate between 1- 100ms
    for(int j = 0; j <= rand() % delay_ms(0.1); j++)
    {
        generate_plate_number();
    }


}
