# makefile

clientLibs :=  -lpthread -lgsl -lgslcblas -lm 

all: dataserver Client

reqchannel.o: reqchannel.H reqchannel.C
	g++ -c reqchannel.C
	
Semaphore.o: Semaphore.H Semaphore.C
	g++ -c $^
	
BoundedBuffer.o: BoundedBuffer.H BoundedBuffer.C Item.H
	g++ -c $^
	
logop.o: logop.c buddyFunc.h
	g++ -c $^
	
Client.o: Client.C 
	g++ -c Client.C

dataserver: dataserver.C reqchannel.o 
	g++ -o dataserver $^ -lpthread  

Client: Client.o BoundedBuffer.o Semaphore.o reqchannel.o logop.o
	g++ -o Client $^ $(clientLibs)
	
clean:
	rm dataserver Client Client.o logop.o BoundedBuffer.o Semaphore.o reqchannel.o
