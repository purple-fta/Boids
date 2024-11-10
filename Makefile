LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lopengl32

SFML_INCLUDE_PATH = C:/SFML/SFML-2.6.1/include
SFML_LIB_PATH = C:/SFML/SFML-2.6.1/lib

run : main.exe
	./main.exe

main.exe : build

build : main.o
	g++ -o main main.o -LC:/SFML/SFML-2.6.1/lib $(LIBS)

main.o : src/main.cpp
	g++ -c -Wall -IC:/SFML/SFML-2.6.1/include -Iimgui -Iimgui-sfml src/main.cpp -o main.o

clear : 
	erase main.exe
