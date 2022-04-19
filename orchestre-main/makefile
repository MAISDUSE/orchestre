make : 
	gcc demo.c semaphore.c -o demo -lopenal -pthread -lalut -Wall -Wextra -Wconversion -Werror

server :
	./demo 1234

sound : 
	nc 127.0.0.1 1234 &