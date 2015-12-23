all: biu client

biu:bitcask.o biu_api.o biuserver.o
	g++ -g bitcask.o biu_api.o biuserver.o -o biu
biuserver.o:biuserver.cpp biu_api.h
	g++ -g -c biuserver.cpp
biu_api.o:biu_api.cpp biu_api.h
	g++ -std=c++11 -g -c biu_api.cpp
bitcask.o:bitcask.cpp bitcask.h
	g++ -std=c++11 -g -c bitcask.cpp

client:biuclient.o		
	g++ -g biuclient.o -o client
biuclient.o:biuclient.cpp
	g++  -g -c biuclient.cpp
clean:	
	rm -f biu client  *.o
