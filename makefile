all:
	gcc ./src/main.c ./src/screen.c ./src/keyboard.c ./src/timer.c -I./include -o go-horse

run:
	./go-horse

clean:
	rm go-horse