#include <cassert>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include "buddyFunc.h"
#include "reqchannel.H"
#include "BoundedBuffer.H"
#include <gsl/gsl_histogram.h>

using namespace std;

void * workerThreadWork(void *param);
void * requestThreadWork(void *param);

struct Person{
	BoundedBuffer statistics, requests;
	std::string name;
	int numDataRequests, sizeBuffer;
	Person(std::string nm, int dataReq, int bufSize)
		:name(nm),numDataRequests(dataReq),sizeBuffer(bufSize){
			statistics = BoundedBuffer(bufSize);
			requests = BoundedBuffer(bufSize);
		}
};

RequestChannel *channel;

int main(int argc, char *argv[]){
	int dataReqPerPerson, boundedBufferSize, numWorkerThreads;
	if(argc < 7)
		fprintf(stderr, "not enough command line arguements");
	
	dataReqPerPerson = atof(argv[2]);
	boundedBufferSize = atof(argv[4]);
	numWorkerThreads = atof(argv[6]);
	
	pthread_t *JoeSmithWorkerThread, *doeWorkerThread, *otherWorkerThread;
	JoeSmithWorkerThread = (pthread_t *)calloc(numWorkerThreads,sizeof(pthread_t));
	
	doeWorkerThread = (pthread_t *)calloc(numWorkerThreads,sizeof(pthread_t));
	otherWorkerThread = (pthread_t *)calloc(numWorkerThreads,sizeof(pthread_t));
	
	pid_t childpid;
	if((childpid = fork()) == 0)
		execv("./dataserver",NULL);
	else 
		cout << "Establishing request channel..." << flush;
		RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
		channel = &chan;
		cout << "Request channel established\n";
		pthread_t JoeSmithStatThread, JoeSmithRequestThread, doeStatThread, doeRequestThread, otherStatThread, otherRequestThread;
		struct Person personA("Joe Smith", dataReqPerPerson, boundedBufferSize);
		if(pthread_create(&JoeSmithRequestThread,NULL,requestThreadWork,&personA)!=0)	//function begins filling request buffer
			std::cout << "Error creating Joe Smith request thread \n";
		for(int i = 0; i < numWorkerThreads; i++){
			if(pthread_create(JoeSmithWorkerThread + i,NULL,workerThreadWork,&personA)!=0)
				std::cout << "Error creating a joe smith worker thread\n";
		}
		pthread_join(JoeSmithRequestThread,NULL);
		for(int i = 0; i < numWorkerThreads; i++)
			pthread_join(*(JoeSmithWorkerThread+i),NULL);
		return 0;
		
		
		/*Worker threads
		//need to initialize the connection for each worker to buffer only once
		1) get data from request thread buffer
		2) send data request to server
		3) get data from server
		4) put data into statistics buffer
		5) repeat 1-4 until all data requests have been made
		
		Request thread
		-puts data into buffer if there is a free slot and does
		this until there are no more data requests. Should know
		how much data he has put into buffer
		
		stat thread 
		-gets data from stat buffer if there is data there and does
		this until all data requests have been entered. Should know
		how much data he has taken out of buffer
		
		once BOTH request thread and stat thread have delt with the N
		data requests, we can exit all threads
		*/	
	
}

void * workerThreadWork(void *param){
	struct Person *person = (struct Person *)param;
	string requestChannelName = channel->send_request("newthread");
	RequestChannel threadChannel(requestChannelName,RequestChannel::CLIENT_SIDE);
	Item *item;
	for(;;){ 
		item = person->requests.get_item();
		void *temp = (void *)item;
		string* str = (string *)temp;
		string personsName = *str;
		string dataItem = "data " + personsName;
		string reply = threadChannel.send_request(dataItem);
		Item statItem(reply);
		person->statistics.insert_item(&statItem);
	}
}

void * requestThreadWork(void *param){
	struct Person *person = (struct Person *)param;
	std::string name = person->name;
	Item item(name);
	int workToDo = person->numDataRequests;
	for(int i=0;i < workToDo;i++)
		person->requests.insert_item(&item);
}

//stat thread must convert from item to double/int

void * statThreadWork(void *param){
	struct Person *person = (struct Person *)param;
	int workToDo = person->numDataRequests;
	int numBins = numbBins(workToDo);
	gsl_histogram *h = gsl_histogram_alloc(numBins);
	Item *item;
	double *dbptr, dataPoint;
	for(int i=0; i < workToDo;i++){
		item = person->statistics.get_item();
		void *temp = (void *)item;
		dbptr = (double *)temp;
		dataPoint = *dbptr;
		//put item in histogram
	}
}
