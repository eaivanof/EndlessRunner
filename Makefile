init:
	mkdir obj

build:
	g++ code/*.cpp -Ihead -o EndlessRunner.exe
	
clean:
	del .\obj\*.o
