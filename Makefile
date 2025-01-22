CC = gcc
CFLAGS = -Wall -Wextra -ggdb -std=c11 -O0 -DDEBUG

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

ifeq ($(OS), Windows_NT)
INC = -I libs/raylib-5.5_win64_mingw-w64/include -I libs/raygui-4.0/src/
LIB = -L libs/raylib-5.5_win64_mingw-w64/lib -l:libraylib.a -lgdi32 -lopengl32 -lwinmm
RM = del
else
INC = -I libs/raylib-5.5_linux_amd64/include -I libs/raygui-4.0/src/
LIB = -L libs/raylib-5.5_linux_amd64/lib -l:libraylib.a -lm
RM = rm -rf
endif

all: game.exe

game.exe: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)

$(OBJ):
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

anim.o: anim.c anim.h
main.o: main.c res.h anim.h map.h player.h pathfinder.h util.h test.h \
	 render.h ray.h
map.o: map.c map.h util.h
nobuild.o: nobuild.c nobuild.h
pathfinder.o: pathfinder.c pathfinder.h map.h util.h
player.o: player.c player.h map.h util.h ray.h
ray.o: ray.c ray.h map.h player.h
render.o: render.c player.h map.h ray.h
res.o: res.c res.h anim.h
test.o: test.c test.h res.h anim.h map.h player.h render.h ray.h util.h

ifeq ($(OS), Windows_NT)
getdeps:
	mkdir libs & cd libs &\
	       	curl -L -O https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_win64_mingw-w64.zip &\
		curl -L -O https://github.com/raysan5/raygui/archive/refs/tags/4.0.zip &\
	       	tar xf raylib-5.5_win64_mingw-w64.zip &\
	       	tar xf 4.0.zip &\
	       	del /S /Q *.zip
else
getdeps:
	mkdir -p libs;\
		cd libs;\
		curl -L -O https://github.com/raysan5/raylib/releases/download/5.5/raylib-5.5_linux_amd64.tar.gz;\
		curl -L -O https://github.com/raysan5/raygui/archive/refs/tags/4.0.tar.gz &\
		tar xvf raylib-5.5_linux_amd64.tar.gz;\
		tar xvf 4.0.tar.gz;\
		rm -rf *.tar.gz
endif

clean:
	$(RM) *.o *.exe
