# i have no idea what the fuck i'm doing. help.

# found this
# https://makefiletutorial.com/

LIBS    = -lm -lallegro -lallegro_acodec -lallegro_audio -lallegro_font -lallegro_image -lallegro_primitives -lallegro_ttf
OBJECTS = $(patsubst src/%.c,obj/%.o,$(wildcard src/*.c))

# add more sounds
SOUND_DEST_DIR   = build
SOUND_SOURCE_DIR = sounds
SOUNDS           = $(patsubst sounds/%.flac,build/%.flac,$(wildcard sounds/*.flac))
SOUNDS_WINDOWS   = $(patsubst sounds/%.flac,bin/%.flac,$(wildcard sounds/*.flac))

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

bin/%.flac: $(SOUND_SOURCE_DIR)/%.flac
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
	gcc -o build/waistgunners $(RELEASE_ARGS) -pie
	echo "done!"

release_windows: $(SOUNDS_WINDOWS)
	# allegro libs and headers not included. sorry! instructions in README.
	echo "building windows version..."
	x86_64-w64-mingw32-gcc -o bin/waistgunners.exe $(WINDOWS_ARGS)
	echo "copying assets..."
	cp spritesheet.png bin/spritesheet.png
	cp PressStart2P-Regular.ttf bin/PressStart2P-Regular.ttf
	echo "done!"
