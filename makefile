coba : coba.cpp
	g++ coba.cpp -o coba -lpthread 
receiver : udp_receiver.cpp
	g++ udp_receiver.cpp -o receiver -lpthread 

thread : thread.cpp
	g++ thread.cpp -o thread -lpthread