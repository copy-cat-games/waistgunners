# i have no idea what the fuck i'm doing. help.

LIBS     = -lm -lallegro -lallegro_acodec -lallegro_audio -lallegro_font -lallegro_image -lallegro_primitives -lallegro_ttf
OBJECTS  = obj/graphics.o obj/keyboard.o obj/main.o \
	obj/vector.o obj/game_state.o obj/event.o obj/mouse.o \
	obj/hud.o obj/engine.o obj/bomber.o obj/bullet.o
COMPILER = gcc

RELEASE_ARGS = -O2
WINDOWS_ARGS = -L./libs $(RELEASE_ARGS)

.SILENT: build/waistgunners clean setup release release_windows

build/waistgunners: $(OBJECTS)
	echo "linking ..."
	$(COMPILER) -o build/waistgunners $(OBJECTS) $(LIBS)
	echo "done!"

obj/%.o: src/%.c
	@echo "compiling $< ..."
	@$(COMPILER) -c -o $@ $<

clean:
	rm obj/*.o
	rm build/*

setup:
	mkdir build
	mkdir obj

release:
	echo "building release..."
	gcc -O2 -o build/waistgunners src/*.c src/*.h $(LIBS)
	echo "done!"

release_windows:
	# allegro libs and headers not included. sorry! instructions in README.
	echo "building windows version..."
	x86_64-w64-mingw32-gcc -o bin/waistgunners.exe -O2 -I./include -B./lib src/*.c src/*.h $(LIBS)
	echo "cleaning out junk files..."
	echo "done!"
