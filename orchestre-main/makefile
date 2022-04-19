.PHONY: demo sender

demo:
	gcc -o demo demo.c semaphore.c freealut/*.c -lalut -pthread -lopenal -lm -Wall -Wconversion -Wextra

sender:
	gcc -o sender sender.c -Wall -Wconversion -Wextra

play:
	./demo 1234

send:
	./sender 1234
