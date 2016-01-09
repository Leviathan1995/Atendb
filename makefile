## erlang source file

.SUFFIXES: .erl .beam 
 
.erl.beam:
	erlc -W $<
 
ERL = erl -boot start_clean

MODS=biu/biu biu/biu_client biu/biu_server

all:compile biustorge

compile: ${MODS:%=%.beam}

## c++ source file

biustorge:bitcask.o biu_api.o biu_comm.o biu_port.o
	g++ -g bitcask.o biu_api.o biu_comm.o biu_port.o -o biustorge
bitcask.o:biu/bitcask.cpp biu/bitcask.h
	g++ -std=c++11 -g -c biu/bitcask.cpp
biu_api.o:biu/biu_api.cpp biu/biu_api.h biu/bitcask.h
	g++ -std=c++11 -g -c biu/biu_api.cpp
biu_comm.o:biu/biu_comm.cpp biu/biu_comm.h
	g++ -std=c++11 -g -c biu/biu_comm.cpp
biu_port.o:biu/biu_port.cpp biu/biu_comm.h biu/biu_api.h biu/bitcask.h
	g++ -std=c++11 -g -c biu/biu_port.cpp

clean:
	rm -rf *.o *.beam
