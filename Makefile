\CC=gcc
FLAGS=-std=c99 -Wall -Wundef -Wcast-align -Wpointer-arith -Wstrict-overflow=5 -Winit-self

life: life.c world.c
	$(CC) $(FLAGS) $^ -o $@
