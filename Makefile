all:
	gcc -Wall main.c -o main


clean:
	@echo Cleaning...
	rm -f *.o main
