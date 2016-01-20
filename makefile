# erlang source file

.SUFFIXES: .erl .beam 
 
.erl.beam:
	erlc -W $<
 
ERL = erl -boot start_clean

MODS=src/biu_server/biu src/biu_server/biu_server biu_client/biu_client

all:compile biustorge create_data_folder change_folder_authority

compile: ${MODS:%=%.beam}

#test
change_folder_authority:
	@echo "change folder /Users/$(USER)/biudata as 777,$(USER),staff" ;\
		sudo chgrp staff /Users/$(USER)/biudata ;\
		sudo chown $(USER) /Users/$(USER)/biudata ;\
		sudo chmod 777 /Users/$(USER)/biudata
# c++ source file

biustorge:bitcask.o biu_api.o biu_comm.o biu_port.o
	g++ -g bitcask.o biu_api.o biu_comm.o biu_port.o -o biustorge
bitcask.o:src/biu_storage/bitcask.cpp src/biu_storage/bitcask.h
	g++ -std=c++11 -g -c src/biu_storage/bitcask.cpp
biu_api.o:src/biu_storage/biu_api.cpp src/biu_storage/biu_api.h src/biu_storage/bitcask.h
	g++ -std=c++11 -g -c src/biu_storage/biu_api.cpp
biu_comm.o:src/biu_storage/biu_comm.cpp src/biu_storage/biu_comm.h
	g++ -std=c++11 -g -c src/biu_storage/biu_comm.cpp
biu_port.o:src/biu_storage/biu_port.cpp src/biu_storage/biu_comm.h src/biu_storage/biu_api.h src/biu_storage/bitcask.h
	g++ -std=c++11 -g -c src/biu_storage/biu_port.cpp

create_data_folder:
	@echo "create data folder: /Users/$(USER)/biudata" ;\
		if [ ! -d /Users/$(USER)/biudata  ] ; then sudo mkdir /Users/$(USER)/biudata ; fi

clean:
	rm -rf *.o *.beam biustorge
