/* 
    File: semaphore.H

    Author: R. Bettati
            Department of Computer Science
            Texas A&M University
    Date  : 08/02/11

*/

#ifndef _semaphore_H_                   // include file only once
#define _semaphore_H_

/*--------------------------------------------------------------------------*/
/* DEFINES */
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* INCLUDES */
/*--------------------------------------------------------------------------*/

#include <pthread.h>
#include <stdlib.h>
#include <iostream>

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */ 
/*--------------------------------------------------------------------------*/

/* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CLASS   S e m a p h o r e  */
/*--------------------------------------------------------------------------*/

class Semaphore {
private:
  /* -- INTERNAL DATA STRUCTURES
     You may need to change them to fit your implementation. */

  int             value;
  pthread_mutex_t m;	//pthread mutex
  pthread_cond_t  c;	//pthread conditional

public:

  /* -- CONSTRUCTOR/DESTRUCTOR */

  Semaphore(int _val);
  
  Semaphore(){
  value = 0;
  pthread_mutex_init(&m,NULL);
  pthread_cond_init(&c,NULL);
  }
  
  ~Semaphore();

  /* -- SEMAPHORE OPERATIONS */

  int P(); // down-use up a wakeup and decrement count
  int V(); // up-increment count and then system chooses a random process/thread to wake up
};


#endif


