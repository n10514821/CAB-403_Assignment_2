
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>
#include "mem.h"


// DEFINES
#define PARKING_SIZE 2920
//generates random licence plate numbers everytime it is run
#define LENGTH_LICENCEPLATE 5
#define LEVELS 5
#define ENTRANCES 5
#define EXITS 5
// GLOBALS
// create a file pointer for communucation between file a program
FILE *fileptr;
//create a mutex lock
pthread_mutex_t lock;
//shared memory file descriptor and pointer to shared memory object
// int shm_fd;
 void *ptr;


// //STRUCTS
// // create the shared parking (not sure what should go in here)
// typedef struct shared_carpark
// {
//     sem_t Fire_alarm_semaphore;
//     sem_t Manager_sempahore;
//     sem_t Simulator_semaphore;

// }shared_carpark_t;

// // shared memory structure
 // PARKING struct


// //FUNCTIONS

 void create_shared_object(shared_carpark_t *shm, const char *share_name)
 {  
    int shm_fd;
    shm_fd = shm_open(share_name,O_CREAT|O_RDWR,0666); 

     //configure the size of the shared memory object
    ftruncate(shm_fd,PARKING_SIZE);

    ptr = mmap(0,PARKING_SIZE,PROT_READ, MAP_SHARED, shm_fd,0);
    
    return ptr;
    }


//linked list for cars
typedef struct car car_t;

struct car
{
    char *licence_plate;
};

void plate_print(car_t *p)
{
    printf("licence plate=%s\n", p->licence_plate);
}

typedef struct node node_t;

// a node in a linked list of people
struct node
{
    car_t *car;
    node_t *next;
};

//HASHTABLE
//Hashtable struct && variables 
typedef struct item item_t;
struct item
{
    char *key;
    int value;
    item_t *next;
};

// A hash table mapping a string to an integer.
typedef struct htab htab_t;
struct htab
{
    item_t **buckets;
    size_t size;
};

// print all people in the list pointer to by head
// pre: true
// post: list of people printed to screen
void node_print(node_t *head)
{
    for (; head != NULL; head = head->next)
    {
        plate_print(head->car);
    }
}

// add a car to the list pointed to by head
// pre: head != NULL
// post (return == NULL AND failed to allocate memory for new linked list node)
//      OR (return == the new head of the list)
node_t *node_add(node_t *head, car_t *car)
{
    //DOUBLE CHECK IF MEMORY WAS ALLOCATED.
    //alocate memory for the new_head. 
    node_t *new_head  = malloc(sizeof(node_t)); 
    if(new_head == NULL)
    {
        return NULL;
    }
    new_head->car = car;
    //pre pending saves complexity and is faster than appending.
    new_head->next = head;
    return new_head;
}

// find person by name in list pointed to by head
// pre: head != NULL
// post (return == NULL AND name not found)
//      OR (return == node with person named name)
node_t *node_find_plate(node_t *head, char *licence_plate)
{
    for(; head != NULL; head = head->next)
    {
        if(strcmp(head->car->licence_plate, licence_plate) == 0)
        {
            return head;
        }
    }
    return NULL;
}

// delete a person by name in list pointed to by head
// pre: head != NULL
// post: return == the new head of the list
node_t *node_delete(node_t *head, char *licence_plate)
{
    node_t *prev;
    node_t *curr = head;
    for(; curr != NULL; curr = curr->next)
    {
        if(strcmp(curr->car->licence_plate, licence_plate)== 0)
        {
            //unlink this node
            //if head
            if(curr == head)
            {
                node_t *temp_head = head->next;
                free(head);
                return temp_head;
            }
            else
            {
                prev->next = curr->next;
                free(curr);
                return head;
            }
        }
        prev = curr;
    }
    return head;

}

//time delay
time_t delay_ms(int seconds)
{
    int milli_sec = 1000 *seconds;

    clock_t start = clock();
    while(clock() < start + milli_sec);
}

//generate plate numbers
void generate_plate_number()
{
    pthread_mutex_lock(&lock);
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
            printf("%c", alphabet);
        }       
    }
    printf("\n");
    pthread_mutex_unlock(&lock);

   
}

//reads a file.
void generate_mix_plates()
{
    pthread_mutex_lock(&lock);
    char plate_num[7];
    int nLines = 0;
    char line[100];
    int randLine;
    //open file 
    fileptr = fopen("plates.txt","r +");

    // reads lines in the file.
    if(fileptr != NULL)
    {
        while(!feof(fileptr)) 
        {
            fgets(line, sizeof(fileptr), fileptr);
            nLines++;
        }
        randLine = rand() % nLines;

        fseek(fileptr, 0, SEEK_SET);
        for(int j = 0; j <= rand()%delay_ms(0.1); j++)
        {
            for (int i = 0; !feof(fileptr) && i <= randLine; i++)
            {
     
                if(i <= randLine/2)
                {
                    fgets(line, sizeof(fileptr), fileptr);
                    printf(" %s",line);
                }
                else
                {
                    generate_plate_number();
                }
                
            }
        } 
    }
    else
    {
        printf("Error in opening file");
        exit(1);
    }
    fclose(fileptr);
    pthread_mutex_unlock(&lock);
}

// HASHTABLE

// Initialise a new hash table with n buckets.
// pre: true
// post: (return == false AND allocation of table failed)
//       OR (all buckets are null pointers)
bool htab_init(htab_t *h, size_t n)
{
    h->size = n;
    h->buckets = (item_t **)calloc(n, sizeof(item_t *));
    return h->buckets != 0;
}

// The Bernstein hash function.
// A very fast hash function that works well in practice.
size_t djb_hash(char *s)
{
    size_t hash = 5381;
    int c;
    while ((c = *s++) != '\0')
    {
        // hash = hash * 33 + c
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

// Calculate the offset for the bucket for key in hash table.
size_t htab_index(htab_t *h, char *key)
{
    return djb_hash(key) % h->size;
}

// Find pointer to head of list for key in hash table.
item_t *htab_bucket(htab_t *h, char *key)
{
    return h->buckets[htab_index(h, key)];
}

// Find an item for key in hash table.
// pre: true
// post: (return == NULL AND item not found)
//       OR (strcmp(return->key, key) == 0)
item_t *htab_find(htab_t *h, char *key)
{
    for (item_t *i = htab_bucket(h, key); i != NULL; i = i->next)
    {
        if (strcmp(i->key, key) == 0)
        { // found the key
            return i;
        }
    }
    return NULL;
}

// Add a key with value to the hash table.
// pre: htab_find(h, key) == NULL
// post: (return == false AND allocation of new item failed)
//       OR (htab_find(h, key) != NULL)
bool htab_add(htab_t *h, char *key)
{
    // allocate new item
    item_t *newhead = (item_t *)malloc(sizeof(item_t));
    if (newhead == NULL)
    {
        return false;
    }
    newhead->key = key;
   

    // hash key and place item in appropriate bucket
    size_t bucket = htab_index(h, key);
    newhead->next = h->buckets[bucket];
    h->buckets[bucket] = newhead;
    return true;
}

void item_print(item_t *i)
{
    printf("key=%s", i->key);
}
// Print the hash table.
// pre: true
// post: hash table is printed to screen
void htab_print(htab_t *h)
{
    printf("hash table with %d buckets\n", h->size);
    for (size_t i = 0; i < h->size; ++i)
    {
        printf("bucket %d: ", i);
        if (h->buckets[i] == NULL)
        {
            printf("empty\n");
        }
        else
        {
            for (item_t *j = h->buckets[i]; j != NULL; j = j->next)
            {
                item_print(j);
                if (j->next != NULL)
                {
                    printf(" -> ");
                }
            }
            printf("\n");
        }
    }
}

void read_to_list(size_t buckets, htab_t h)
{
    FILE *fileptr = fopen("plates.txt.txt","r");
    if(fileptr == NULL)
    {
        exit(1);
    }

    char val[99];

    while(fscanf(fileptr, "%s", &val)>0)
    {
        htab_add(&h, val);  
        htab_print(&h);
        printf("\n");   
       
    }
     fclose(fileptr); 

}

// MAIN PROGRAM
int main (void) 
{
    time_t t;
    srand((unsigned)time(&t));
    int enter;

    printf("creating hashtable:\n");
    size_t buckets = 50;
    htab_t h;

    if(!htab_init(&h, buckets))
    {
        printf("failed to initalise\n");
        return EXIT_FAILURE;
    }

    
    read_to_list(buckets,h);

    shared_carpark_t* park;
    
    

    create_shared_object(park, 'park');
    

    
    //generate_mix_plates();  
    

    // while(rand() % delay_ms(0.1))
    // {
    //     for (int i = 0; i <= enter; i++)
    //     {
    //         enter = rand()%5;
    //     }
    
    //     if(enter <= 5)   
    //     {
    //         printf("ENTERANCE %d\n", enter);
    //         generate_mix_plates();
    //     }
    // }
    
    return 0;

}
