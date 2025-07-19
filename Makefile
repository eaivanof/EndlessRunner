CF=-mwindows -c -DUNICODE
main=./code/main.cpp
maino=./obj/main.o
game=./code/game.cpp
gameo=./obj/game.o
gameh=./head/game.h
BitMap=./code/BitMap.cpp
BitMapo=./obj/BitMap.o
BitMaph=./head/BitMap.h
XyBitMap=./code/XyBitMap.cpp
XyBitMapo=./obj/XyBitMap.o
XyBitMaph=./head/XyBitMap.h
KeyB=./code/KeyB.cpp
KeyBo=./obj/KeyB.o
KeyBh=./head/KeyB.h
CARSKIN_SRC = ./code/CarSkin.cpp
CARSKIN_OBJ = ./obj/CarSkin.o
CARSKIN_HDR = ./head/CarSkin.h

Objects=$(maino) $(gameo) $(BitMapo) $(KeyBo) $(XyBitMapo) $(CARSKIN_OBJ)

EndlessRunner.exe: $(Objects)
	g++ -mwindows -DUNICODE $(Objects) -o EndlessRunner -lwinmm
$(maino): $(main) $(gameh) 
	g++ $(CF) $(main) -o $(maino)
$(gameo): $(game) $(gameh)
	g++ $(CF) $(game) -o $(gameo)
$(BitMapo): $(BitMap) $(BitMaph)
	g++ $(CF) $(BitMap) -o $(BitMapo)	
$(KeyBo): $(KeyB) $(KeyBh)
	g++ $(CF) $(KeyB) -o $(KeyBo)	
$(XyBitMapo): $(XyBitMap) $(XyBitMaph)
	g++ $(CF) $(XyBitMap) -o $(XyBitMapo)	
$(CARSKIN_OBJ): $(CARSKIN_SRC) $(CARSKIN_HDR)
	g++ $(CF) $(CARSKIN_SRC) -o $(CARSKIN_OBJ)
init:
	mkdir obj
clean: 
	del .\obj\*.o
#build:
#	g++ code/*.cpp -Ihead -o EndlessRunner.exe
