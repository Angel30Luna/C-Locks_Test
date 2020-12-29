#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <time.h>
#include "readerwriter.c"

//Declare Functions
void *writer(void *arg);
void *reader(void *arg);
void reading_writing(char c, unsigned int threadID);

//Locks and semaphores from "readerwriter.c"
rwlock_t rwlocks;

//Main
int main(){
    pthread_t rw_threads[15];
    char threadType, readChar;
    int i = 0, j = 0, caseNumber = 0, newLineCount = 0, lineCount = 0;
    
    //Initialize the locks struct
    rwlock_init(&rwlocks);

    //Set the random seed
    srand(time(0));

    //Open the file and check if it opened correctly
    FILE *file = fopen("scenarios.txt", "r");
    if(file == NULL){
	    printf("Error. The file does not exist, or is corrupted.\n");
	    return 1;
    }

    //Get the number of '\n' characters to know when to stop reading the text file
    while((readChar = getc(file)) != EOF){
        if(readChar == '\n')
            newLineCount++;
    }

    //Reset file pointer to the begining of the text file
    fseek(file, 0, SEEK_SET);

    //Read text file and create threads according to the character returned by getc()
    while((threadType = getc(file)) != EOF){
        short int skipIncrement = 0;

        //Output format
        if(caseNumber == 0){
            caseNumber++;
            printf("Scenario 1: \n");
        }

        //Create and join threads according to the character read from the text file
        if(threadType == 'r')
            pthread_create(&rw_threads[i], NULL, reader, &threadType);
        else if(threadType == 'w')
            pthread_create(&rw_threads[i], NULL, writer, &threadType);
        else if(threadType == '\n'){
            i = 0;
            j = 0;
            skipIncrement = 1;
            caseNumber++;
            lineCount++;

            //If this is the last '\n' character on the file, join threads outside while loop and exit
            if(lineCount == newLineCount)
                break;

            //Wait for all threads to end and mark new scenario
            for(; j < 15; j++)
                pthread_join(rw_threads[j], NULL);

            printf("\nScenario %d: \n", caseNumber);
        }
        
        //If a '\n' character is detected, wait for one loop cycle before incrementing
        if(skipIncrement == 0)
            i++;
    }

    //Wait for all threads to end on the final scenario
    for(; j < 15; j++)
        pthread_join(rw_threads[j], NULL);

    return 0;
}

//Writer Thread Function
void *writer(void *arg){
    rwlock_acquire_writelock(&rwlocks);
        reading_writing('w', pthread_self());
    rwlock_release_writelock(&rwlocks);
    return NULL;
}

//Reader Thread Function
void *reader(void *arg){
    rwlock_acquire_readlock(&rwlocks);
        reading_writing('r', pthread_self());
    rwlock_release_readlock(&rwlocks);
    return NULL;
}

//Read/Write Simulation Function
void reading_writing(char c, unsigned int threadID){
    int rwTime = rand() % 101;
    int i, j;

    //Actual "Read/Write"
    for(i = 0; i < rwTime; i++){
        for(j = 0; j < rwTime; j++);
        if((i % 10) == 0){
            if(c == 'r')
                printf("Thread %u is reading.\n", threadID);
            else if(c == 'w')
                printf("Thread %u is writing.\n", threadID);
        }
    }
    return;
}