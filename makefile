# i have no idea what the fuck i'm doing. help.

LIBS     = -lm -lallegro -lallegro_acodec -lallegro_audio -lallegro_font -lallegro_image -lallegro_primitives -lallegro_ttf
OBJECTS  = obj/graphics.o obj/keyboard.o obj/main.o \
	obj/vector.o obj/game_state.o obj/event.o obj/mouse.o \
	obj/smoke.o obj/particle.o \
	obj/hud.o obj/bullet.o obj/gunner.o obj/engine.o obj/bomber.o \
	obj/enemy_fighter.o obj/enemy.o
COMPILER = gcc

RELEASE_ARGS = -O2 src/*.c src/*.h $(LIBS)
WINDOWS_ARGS = -B./lib -I./include $(RELEASE_ARGS)

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
	gcc -o build/waistgunners $(RELEASE_ARGS)
	echo "done!"

release_windows:
	# allegro libs and headers not included. sorry! instructions in README.
	echo "building windows version..."
	x86_64-w64-mingw32-gcc -o bin/waistgunners.exe $(WINDOWS_ARGS)
	echo "done!"
