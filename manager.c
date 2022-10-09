#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include <Simulator.c>

// manager setup
    // read plates.txt into hash table
    // create billing.txt


// OpenThenCloseBoomGate()
    // Open gate
    // wait 30ms (10 for gate to open, 20 for car to enter/exit) 
    // Close gate (takes 10 ms)


// car detected at entrance
    // check plate against hash table
    // if plate doesn't match
        // dipslay 'X'
    // else (plate does match)
        // check for space on each level
        // if level with space is found
            // display directed level
            // OpenThenCloseBoomGate()
        // else (no levels with space)
            // display 'F'


// car detected at exit
    // open billing.txt in append mode
    // append plate along with car's bill (5 cents * time spent in ms)
    // OpenThenCloseBoomGate()


// car detected on a floor
// update carpark struct


// update screen (must be called 'frequently')
    // current capacity of each level
    // current status of each boom gate
    // current status of signs
    // current values of temperature sensors
    // current staus of alarms
    // current total revenue