all:bius client

bius:bitcask.o biu_api.o biuserver.o
	g++ -g bitcask.o biu_api.o biuserver.o -o bius
biuserver.o:biu/biuserver.cpp biu/biu_api.h
	g++ -std=c++11 -g -c biu/biuserver.cpp
biu_api.o:biu/biu_api.cpp biu/biu_api.h
	g++ -std=c++11 -g -c biu/biu_api.cpp
bitcask.o:biu/bitcask.cpp biu/bitcask.h
	g++ -std=c++11 -g -c biu/bitcask.cpp

client:biuclient.o		
	g++ -g biuclient.o -o client
biuclient.o:biu/biuclient.cpp
	g++  -std=c++11 -g -c biu/biuclient.cpp
clean:	
	rm -f bius client  *.o
