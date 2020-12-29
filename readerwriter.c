#include <stdio.h>
#include <semaphore.h>

typedef struct _rwlock_t{
    int readers;
    int writers;
    int waiting;
    
    sem_t lock;
    sem_t writelock;
    sem_t writersMutex;
    sem_t readersMutex;
    sem_t availableWriter;
} rwlock_t;

void rwlock_init(rwlock_t *rw) {
    rw->readers = 0;
    rw->writers = 0;
    rw->waiting = 0;

    sem_init(&rw->lock, 0, 1);
    sem_init(&rw->writelock, 0, 1);
    sem_init(&rw->writersMutex, 0, 1);
    sem_init(&rw->readersMutex, 0, 1);
    sem_init(&rw->availableWriter, 0, 0);
}

void rwlock_acquire_readlock(rwlock_t *rw) {
    //This is used to fix the starving writers problem
    //availableWriter lock stops all incoming readers until all waiting writers are done writing
    sem_wait(&rw->readersMutex);
        if(rw->writers >= 1){
            rw->waiting = 1;
            sem_wait(&rw->availableWriter);
            rw->waiting = 0;
        }
    sem_post(&rw->readersMutex);

    //Original Statements from the Operating Systems: 3 Easy Pieces Book
    sem_wait(&rw->lock);
        rw->readers++;
        if (rw->readers == 1) // first reader gets writelock
            sem_wait(&rw->writelock);
    sem_post(&rw->lock);
}

void rwlock_release_readlock(rwlock_t *rw) {
    //Note: No changes made to this function from the 
    //original implementation shown in the book
    sem_wait(&rw->lock);
        rw->readers--;
        if (rw->readers == 0) // last reader lets it go
            sem_post(&rw->writelock);
    sem_post(&rw->lock);
}

void rwlock_acquire_writelock(rwlock_t *rw) {
        //Increment the number of waiting writers
        sem_wait(&rw->writersMutex);
            rw->writers++;
        sem_post(&rw->writersMutex);

        //Acquire writelock lock
        sem_wait(&rw->writelock);
}

void rwlock_release_writelock(rwlock_t *rw) {
    //Decrement the number of waiting writers
    sem_wait(&rw->writersMutex);
        rw->writers--;
    sem_post(&rw->writersMutex);

    //Call release on availableWriters and writelock semaphores accordingly
    sem_post(&rw->writelock);
    if(rw->waiting == 1)
        sem_post(&rw->availableWriter);
}