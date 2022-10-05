
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


// DEFINES
#define PARKING_SIZE 2920
//generates random licence plate numbers everytime it is run
#define LENGTH_LICENCEPLATE 5

// GLOBALS
// create a file pointer for communucation between file a program
FILE *fileptr;
//create a mutex lock
pthread_mutex_t lock;
//shared memory file descriptor and pointer to shared memory object
int shm_fd;
void *ptr;


//STRUCTS
// create the shared parking (not sure what should go in here)
typedef struct shared_carpark
{
    sem_t Fire_alarm_semaphore;
    sem_t Manager_sempahore;
    sem_t Simulator_semaphore;

}shared_carpark_t;

// shared memory structure
typedef struct shared_memory
{
    const char *name;

    int fd;

    //address of shared carpark block
    shared_carpark_t *data;

}shared_memory_t;

// PARKING struct
typedef struct PARKING
{
    struct Exit;
    struct Level;
    struct Entrance;
} shared_carpark_t;

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


//FUNCTIONS

bool create_shared_object(shared_memory_t *shm, const char *share_name)
{
    shm_unlink(share_name);

    //insert share name to shm->name
    shm->name = share_name;

    //create shared memory object
    shm_fd = shm_open(share_name,O_CREAT|O_RDWR,0666);
    shm->fd = shm_fd; 

    //configure the size of the shared memory object
    ftruncate(shm_fd,PARKING_SIZE);

    ptr = mmap(0,PARKING_SIZE, MAP_SHARED, shm_fd,0);
    shm->data = ptr;

    return true;
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
    char plate[5];
    for(int i = 0; i <= LENGTH_LICENCEPLATE; i ++)
    {
        char number = num[rand() % (sizeof num - 1)];
        char alphabet = alpha[rand() % (sizeof alpha - 1)];
        if(i <= 2)
        {
            //printf("%c", number);
            strncat(plate, number, 5);
        }
        else
        {
            //printf("%c", alphabet);
            strncat(alphabet, number, 5);

        }    
        // could just add htab_add(&h, ) per loop iteration
    }
    printf("\n");
    pthread_mutex_unlock(&lock);

   
}

//reads a file.
void read_file()
{
    pthread_mutex_lock(&lock);
    char plate_num[7];
    //open file 
    fileptr = fopen("plates.txt","r +");

    // if file does not exist
    if(fileptr != NULL)
    {
        while (!feof(fileptr))
        {
            fscanf(fileptr,"%s",&plate_num); 
            printf("txt %s\n",plate_num);                 
        }  
    }
    else
    {
        printf("Error in opening file");
        exit(1);
    }
    pthread_mutex_unlock(&lock);
}

void generate_mix_plates()
{
    pthread_mutex_lock(&lock);
    for (int i = 0; i <= rand()%delay_ms(0.1); i++)
    {
        if(i <= rand()%1)
        {
            read_file();             
        }
        else
        {
            generate_plate_number(); 
        } 
    }
    pthread_mutex_unlock(&lock);

    
}

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
bool htab_add(htab_t *h, char *key, int value)
{
    // allocate new item
    item_t *newhead = (item_t *)malloc(sizeof(item_t));
    if (newhead == NULL)
    {
        return false;
    }
    newhead->key = key;
    newhead->value = value;

    // hash key and place item in appropriate bucket
    size_t bucket = htab_index(h, key);
    newhead->next = h->buckets[bucket];
    h->buckets[bucket] = newhead;
    return true;
}


int main (void) 
{
    time_t t;
    srand((unsigned)time(&t));

    generate_mix_plates();

    fclose(fileptr);
    return 0;

}
