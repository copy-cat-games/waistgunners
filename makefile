# i have no idea what the fuck i'm doing. help.

LIBS = -lm -lallegro -lallegro_acodec -lallegro_audio \
	-lallegro_font -lallegro_image -lallegro_primitives -lallegro_ttf
OBJECTS = obj/graphics.o obj/keyboard.o obj/main.o obj/sounds.o \
	obj/vector.o obj/game_state.o obj/event.o obj/mouse.o \
	obj/smoke.o obj/particle.o \
	obj/hud.o obj/bullet.o obj/gunner.o obj/engine.o obj/bomber.o \
	obj/enemy_fighter.o obj/enemy_imposter.o obj/enemy_jet.o obj/enemy.o

# add more sounds
SOUND_DEST_DIR   = build
SOUND_SOURCE_DIR = sounds
SOUNDS           = $(SOUND_DEST_DIR)/enemy_fighter_die.flac \
	$(SOUND_DEST_DIR)/enemy_fighter_shoot.flac \
	$(SOUND_DEST_DIR)/enemy_imposter_die.flac \
	$(SOUND_DEST_DIR)/engine_dies.flac \
	$(SOUND_DEST_DIR)/gunner_shoot.flac \
	$(SOUND_DEST_DIR)/imposter_gunner_shoot.flac \
	$(SOUND_DEST_DIR)/powerup_pickup.flac

COMPILER = gcc # you can also use clang, if you so desire

RELEASE_ARGS = -O2 src/*.c src/*.h $(LIBS)
WINDOWS_ARGS = -B./lib -I./include $(RELEASE_ARGS)

BUILD = bin

ASSETS = build/spritesheet.png build/PressStart2P-Regular.ttf $(SOUNDS)

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

$(SOUND_DEST_DIR)/%.flac: $(SOUND_SOURCE_DIR)/%.flac
	@echo "copying sound $< ..."
	@cp $< $@

clean:
	rm -f obj/*.o
	rm -f build/*
	rm -f bin/waistgunners.exe

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
	echo "done! now copy the sounds over!"
