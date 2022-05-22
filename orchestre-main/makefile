.PHONY: maestro musician

maestro:
	gcc -o maestro maestro.c sockets/semaphore.c audio/*.c menu/maestro_menu.c sockets/maestro_sockets.c helpers/helpers.c freealut/*.c automation/*.c -lalut -pthread -lopenal -lm -Wall -Wconversion -Wextra

musician:
	gcc -o musician musician.c menu/musician_menu.c sockets/musician_sockets.c helpers/helpers.c -Wall -Wconversion -Wextra

all:
	make maestro && make musician