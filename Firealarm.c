#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

volatile void *shm;
int alarm_active = 0;
pthread_mutex_t alarm_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t alarm_condvar = PTHREAD_COND_INITIALIZER;

#define LEVELS 5
#define ENTRANCES 5
#define EXITS 5

#define MEDIAN_WINDOW 5
#define TEMPCHANGE_WINDOW 30

struct boomgate
{
    pthread_mutex_t boomgate_m;
    pthread_cond_t boomgate_c;
    char s;
};
struct parkingsign
{
    pthread_mutex_t parkingsign_m;
    pthread_cond_t parkingsign_c;
    char display;
};

struct tempnode
{
    int temperature;
    struct tempnode *next;
};

// not sure how to apporach this function
struct tempnode *deletenodes(struct tempnode *templist, int after)
{
    if (templist->next)
    {
        templist->next = deletenodes(templist->next, after - 1);
    }
    if (after <= 0)
    {
        free(templist);
        return NULL;
    }
    return templist;
}

// not sure how to apporach this function
int compare(const void *first, const void *second)
{
    return *((const int *)first) - *((const int *)second);
}

// Temperatures are only counted once we have 5 samples
void tempcount(int count, struct tempnode *templist, int mediantemp)
{
    // Temperatures are only counted once we have 5 samples
    int sorttemp[5];
    count = 0;
    for (struct tempnode *t = templist; t != NULL; t = t->next)
    {
        sorttemp[count++] = t->temperature;
    }
    qsort(sorttemp, MEDIAN_WINDOW, sizeof(int), compare);
    mediantemp = sorttemp[(MEDIAN_WINDOW - 1) / 2];
}

// Add median temp to linked list
void addlinklist(struct tempnode *newtemp, int mediantemp, struct tempnode *medianlist)
{
    // Add median temp to linked list
    newtemp[sizeof(struct tempnode)];
    newtemp->temperature = mediantemp;
    newtemp->next = medianlist;
    medianlist = newtemp;
}

void countnode(int count, int hightemps, struct tempnode *medianlist, struct tempnode *oldesttemp)
{
    // Count nodes
    count = 0;
    hightemps = 0;

    for (struct tempnode *t = medianlist; t != NULL; t = t->next)
    {
        // Temperatures of 58 degrees and higher are a concern
        if (t->temperature >= 58)
        {
            hightemps++;
        }
        // Store the oldest temperature for rate-of-rise detection
        oldesttemp = t;
        count++;
    }
}

void activealarm(int count, int hightemps,struct tempnode *templist, struct tempnode *oldesttemp)
{
    if (count == TEMPCHANGE_WINDOW)
    {
        // If 90% of the last 30 temperatures are >= 58 degrees,
        // this is considered a high temperature. Raise the alarm
        // If the newest temp is >= 8 degrees higher than the oldest
        // temp (out of the last 30), this is a high rate-of-rise.
        // Raise the alarm
        if (hightemps >= TEMPCHANGE_WINDOW * 0.9 || templist->temperature - oldesttemp->temperature >= 8)
        {
            alarm_active = 1;
        }
    }
}

void tempmonitor(int level)
{
    struct tempnode *templist = NULL;
    struct tempnode *newtemp;
    struct tempnode *medianlist = NULL;
    struct tempnode *oldesttemp;
    int count;
    int addr;
    int temp;
    int mediantemp;
    int hightemps;

    for (;;)
    {
        // Calculate address of temperature sensor
        addr = 0150 * level + 2496;
        temp = *((int16_t *)(shm + addr));

        // Add temperature to beginning of linked list
        newtemp[sizeof(struct tempnode)];
        newtemp->temperature = temp;
        newtemp->next = templist;
        templist = newtemp;

        // Delete nodes after 5th
        deletenodes(templist, MEDIAN_WINDOW);

        // Count nodes
        count = 0;
        for (struct tempnode *t = templist; t != NULL; t = t->next)
        {
            count++;
        }

        if (count == MEDIAN_WINDOW)
        {
            // Temperatures are only counted once we have 5 samples
            tempcount(count, templist, mediantemp);
            // Add median temp to linked list
            addlinklist(newtemp, mediantemp, medianlist);
            // Delete nodes after 30th
            deletenodes(medianlist, TEMPCHANGE_WINDOW);
            // count node and checks if temp is higher than 58 and stores
            // oldest value for rate-of-rise detection
            countnode(count, hightemps, medianlist, oldesttemp);
            //activates alarm if 90% last 30 >= 58 degrees or newest temp 
            //is >= 8 oldest temp
            activealarm(hightemps, hightemps, templist,oldesttemp);
        }
        usleep(2000);
    }
}

void *openboomgate(void *arg)
{
    struct boomgate *bg = arg;
    pthread_mutex_lock(&bg->boomgate_m);
    for (;;)
    {
        if (bg->s == 'C')
        {
            bg->s = 'R';
            pthread_cond_broadcast(&bg->boomgate_c);
        }
        if (bg->s == 'O')
        {
        }
        pthread_cond_wait(&bg->boomgate_c, &bg->boomgate_m);
    }
    pthread_mutex_unlock(&bg->boomgate_m);
}

int main(void)
{
    int shm_fd;
    shm_fd = shm_open("PARKING", O_RDWR, 0);
    //Error handling
    if (shm_fd == -1)
    {
        return EXIT_FAILURE;
    } 
    //Error handling
    shm = mmap(0, 2920, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if( shm == MAP_FAILED ) 
    {
        return EXIT_FAILURE;
    }

    pthread_t threads[5];

    for (int i = 0; i < LEVELS; i++)
    {
        pthread_create(threads + i, NULL, (void *(*)(void *))tempmonitor, (void *)i);
    }

    for (;;)
    {
        if (alarm_active)
        {
            goto emergency_mode;
        }
        usleep(1000);
    }

    emergency_mode:
    fprintf(stderr, "*** ALARM ACTIVE ***\n");

    // Handle the alarm system and open boom gates
    // Activate alarms on all levels
    for (int i = 1; i <= LEVELS; i++)
    {
        int addr = 0150 * i + 2498;
        char *alarm_trigger = (char *)shm + addr;
        *alarm_trigger = 1;
    }

    // Open up all boom gates
    pthread_t boomgatethreads[10];
    for (int i = 1; i <= ENTRANCES; i++)
    {
        int entrance_addr = 288 * i + 96;
        volatile struct boomgate *bg = shm + entrance_addr;
        pthread_create(boomgatethreads + i, NULL, openboomgate, bg);
    }
    for (int i = 1; i <= EXITS; i++)
    {
        int exit_addr = 192 * i + 1536;
        volatile struct boomgate *bg = shm + exit_addr;
        pthread_create(boomgatethreads + ENTRANCES + i, NULL, openboomgate, bg);
    }

    // Show evacuation message on an endless loop
    for (;;)
    {
        char *evacmessage = "EVACUATE ";
        for (char *p = evacmessage; *p != '\0'; p++)
        {
            for (int i = 1; i <= ENTRANCES; i++)
            {
                int addr = 288 * i + 192;
                volatile struct parkingsign *sign = shm + addr;
                pthread_mutex_lock(&sign->parkingsign_m);
                sign->display = *p;
                pthread_cond_broadcast(&sign->parkingsign_c);
                pthread_mutex_unlock(&sign->parkingsign_m);
            }
            usleep(20000);
        }
    }

    for (int i = 1; i <= LEVELS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    munmap((void *)shm, 2920);
    close(shm_fd);
}
