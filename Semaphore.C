#include "Semaphore.H"

Semaphore::Semaphore(int _val){
	value = _val;
	pthread_mutex_init(&m,NULL);
	pthread_cond_init(&c,NULL);
}

Semaphore::~Semaphore(){}

int Semaphore::P() {//down
	pthread_mutex_lock(&m);
	while(value == 0)
		pthread_cond_wait(&c,&m);
	value--;
	pthread_cond_signal(&c);
	pthread_mutex_unlock(&m);
	return value;
}

int Semaphore::V() {//up
	pthread_mutex_lock(&m);
	value++;
	pthread_cond_signal(&c);
	pthread_mutex_unlock(&m);
	return value;
}
