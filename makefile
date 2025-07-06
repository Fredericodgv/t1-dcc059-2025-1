compile: 
	g++ src/*.cpp -o exec.exe


run: compile
	./exec.exe $(word 2, $(MAKECMDGOALS))