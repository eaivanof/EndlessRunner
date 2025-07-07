CF=-mwindows -c -DUNICODE
main=./code/main.cpp
maino=./obj/main.o
game=./code/game.cpp
gameo=./obj/game.o
gameh=./head/game.h
BitMap=./code/BitMap.cpp
BitMapo=./obj/BitMap.o
BitMaph=./head/BitMap.h
XyBitMap=./code/xyBitMap.cpp
XyBitMapo=./obj/xyBitMap.o
XyBitMaph=./head/xyBitMap.h
Objects=$(maino) $(gameo) $(BitMapo) $(XyBitMapo) 

EndlessRunner.exe: $(Objects)
	g++ -mwindows -DUNICODE $(Objects) -o EndlessRunner
$(maino): $(main) $(gameh) 
	g++ $(CF) $(main) -o $(maino)
$(gameo): $(game) $(gameh)
	g++ $(CF) $(game) -o $(gameo)
$(BitMapo): $(BitMap) $(BitMaph)
	g++ $(CF) $(BitMap) -o $(BitMapo)
$(XyBitMapo): $(XyBitMap) $(XyBitMaph)
	g++ $(CF) $(XyBitMap) -o $(XyBitMapo)		
init:
	mkdir obj
clean: 
	del .\obj\*.o
#build:
#	g++ code/*.cpp -Ihead -o EndlessRunner.exe
