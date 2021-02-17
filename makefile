# i have no idea what the fuck i'm doing. help.

LIBS     = -lm -lallegro -lallegro_acodec -lallegro_audio -lallegro_font -lallegro_image -lallegro_primitives
OBJECTS  = obj/graphics.o obj/keyboard.o obj/main.o obj/vector.o obj/game_state.o obj/event.o obj/mouse.o obj/hud.o obj/engine.o obj/bomber.o
COMPILER = gcc

RELEASE_ARGS = -O2
WINDOWS_ARGS = -L./libs $(RELEASE_ARGS)

build/waistgunners: $(OBJECTS)
	$(COMPILER) -o build/waistgunners $(OBJECTS) $(LIBS)

obj/%.o: src/%.c
	$(COMPILER) -c -o $@ $<

clean:
	rm obj/*.o
	rm build/*

setup:
	mkdir build
	mkdir obj

release:
	gcc -O2 -o build/waistgunners src/*.c src/*.h $(LIBS)