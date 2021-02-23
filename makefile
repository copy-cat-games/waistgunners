# i have no idea what the fuck i'm doing. help.

LIBS = -lm -lallegro -lallegro_acodec -lallegro_audio \
	-lallegro_font -lallegro_image -lallegro_primitives -lallegro_ttf
OBJECTS = obj/graphics.o obj/keyboard.o obj/main.o \
	obj/vector.o obj/game_state.o obj/event.o obj/mouse.o \
	obj/smoke.o obj/particle.o \
	obj/hud.o obj/bullet.o obj/gunner.o obj/engine.o obj/bomber.o \
	obj/enemy_fighter.o obj/enemy_imposter.o obj/enemy.o
COMPILER = gcc # you can also use clang, if you so desire

RELEASE_ARGS = -pie -O2 src/*.c src/*.h $(LIBS)
WINDOWS_ARGS = -B./lib -I./include $(RELEASE_ARGS)

BUILD = bin

ASSETS = build/spritesheet.png build/PressStart2P-Regular.ttf

.SILENT: build/waistgunners clean setup release release_windows \
	build/spritesheet.png build/PressStart2P-Regular.ttf

build/waistgunners: $(OBJECTS) $(ASSETS)
	echo "linking ..."
	$(COMPILER) -o build/waistgunners $(OBJECTS) $(LIBS)
	echo "done!"

build/spritesheet.png: ./spritesheet.png
	echo "copying spritesheet..."
	cp $< $@

build/PressStart2P-Regular.ttf: ./PressStart2P-Regular.ttf
	echo "copying font..."
	cp $< $@

obj/%.o: src/%.c
	@echo "compiling $< ..."
	@$(COMPILER) -g -c -o $@ $<

clean:
	rm -f obj/*.o
	rm -f build/*
	rm -f bin/*

setup:
	mkdir build
	mkdir obj

release: $(ASSETS)
	echo "building linux release..."
	gcc -o build/waistgunners $(RELEASE_ARGS)
	echo "done!"

release_windows:
	# allegro libs and headers not included. sorry! instructions in README.
	echo "building windows version..."
	x86_64-w64-mingw32-gcc -o bin/waistgunners.exe $(WINDOWS_ARGS)
	echo "copying assets..."
	cp spritesheet.png bin/spritesheet.png
	cp PressStart2P-Regular.ttf bin/PressStart2P-Regular.ttf
	echo "done!"
