
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//generates random licence plate numbers everytime it is run
#define LENGTH_LICENCEPLATE 5
int main (void) 
{
    srand((time(NULL)));
    char alpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char num[] = "0123456789";        

    for(int i = 0; i <= 50; i++)
    {
        for(int i = 0; i <= LENGTH_LICENCEPLATE; i ++)
        {
            if(i <= 2)
            {
                printf("%c", num[rand() % (sizeof num - 1)]);
            }
            else
            {
                printf("%c", alpha[rand() % (sizeof alpha - 1)]);
            }
            
        }
        printf("\n");

    }

}