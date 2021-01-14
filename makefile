# i have no idea what the fuck i'm doing. help.

LIBS     = -lm -lallegro -lallegro_acodec -lallegro_audio -lallegro_font -lallegro_image -lallegro_primitives
OBJECTS  = obj/graphics.o obj/keyboard.o obj/main.o obj/vector.o
COMPILER = gcc

RELEASE_ARGS = -O2
WINDOWS_ARGS = -L./libs $(RELEASE_ARGS)

build/waistgunners: $(OBJECTS)
	$(COMPILER) -o build/waistgunners $(OBJECTS) $(LIBS)

obj/graphics.o: src/graphics.c
	$(COMPILER) -c -o obj/graphics.o src/graphics.c

obj/keyboard.o: src/keyboard.c
	$(COMPILER) -c -o obj/keyboard.o src/keyboard.c

obj/vector.o: src/vector.c
	$(COMPILER) -c -o obj/vector.o src/vector.c

obj/main.o: src/main.c
	$(COMPILER) -c -o obj/main.o src/main.c

clean:
	rm obj/*.o
	rm build/*

setup:
	mkdir build
	mkdir obj