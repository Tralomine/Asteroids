SFML := -lsfml-graphics -lsfml-window -lsfml-system
SFML-AUDIO :=  -lsfml-audio $(SFML)

all:
	g++ -c -fPIC functions.cpp -o functions.o $(SFML) -m64
	g++ -c -fPIC shop.cpp -o shop.o $(SFML) -m64
	g++ -c -fPIC objects.cpp -o objects.o $(SFML) -m64
	g++ -c -fPIC player.cpp -o player.o $(SFML) -m64
	g++ main.cpp json/json.o functions.o shop.o objects.o player.o $(SFML-AUDIO) -m64
	./a.out

debug:
	g++ -g -c -fPIC functions.cpp -o functions.o $(SFML) -m64
	g++ -g -c -fPIC shop.cpp -o shop.o $(SFML) -m64
	g++ -g -c -fPIC objects.cpp -o objects.o $(SFML) -m64
	g++ -g -c -fPIC player.cpp -o player.o $(SFML) -m64
	g++ -g main.cpp json/json-debug.o functions.o shop.o objects.o player.o $(SFML-AUDIO) -m64
	gdb ./a.out

main-only:
	g++ main.cpp json/json.o functions.o shop.o objects.o player.o $(SFML-AUDIO) -m64
	./a.out

clean:
	rm *.o

json:
	g++ -c -fPIC -m64 json/json.cpp -o json/json.o
	g++ -g -c -fPIC -m64 json/json.cpp -o json/json-debug.o

#\
COMP := x86_64-w64-mingw32-g++.exe\
SFML := -DSFML_STATIC -I"C:\SFML-2.4.2\include"\
LIBS := -static -static-libstdc++ -static-libgcc -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lmingw32 -luser32 -lgdi32 -lwinmm -ldxguid -mwindows -lopengl32 -lfreetype -ljpeg\
FLAGS := -c -m64 -Wall -O2\
\
ALL := $(FLAGS) $(SFML)\
\
windows:\
	$(COMP) $(ALL) functions.cpp -o functions.o\
	$(COMP) $(ALL) shop.cpp -o shop.o\
	$(COMP) $(ALL) objects.cpp -o objects.o\
	$(COMP) $(ALL) player.cpp -o player.o\
	$(COMP) $(ALL) main.cpp -o main.o\
	$(COMP) -DSFML_STATIC -LC:\SFML-2.4.2\lib main.o functions.o shop.o objects.o player.o  -o asteroids.exe $(LIBS)\
