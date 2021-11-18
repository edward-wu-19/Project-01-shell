all:
	gcc -x c -o shell shell.c

run:
	./shell

clean:
	rm -rf bin/
	rm shell
