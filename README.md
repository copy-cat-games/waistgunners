# Waistgunners

A game, still buggy, still in development. Currently being ~~rebuilt with Electron~~ rewritten in C again.

![image screenshot](screenshot.png)

## Building from source

If something in these instructions doesn't work, feel free to submit an issue. I'll do my best to help.

### Linux

Make sure you have Allegro's development libraries installed. If you're using Debian/Ubuntu or their derivatives, you can grab them with `sudo apt install --install-suggests liballegro5-dev`. (if you're using another distro, you can check your package manager to see if `liballegro5-dev` is available)

Then clone this repository, and run `make setup && make release`. It'll automagically build the game. To start playing, run `build/waistgunners`.

### Windows

Euh... good luck... I have no idea how to compile from Windows. Please send help.

### Cross compiling from Linux for Windows

This would probably also work from within WSL for building for Windows

Make sure you have the cross compiler, `mingw64` or whatever, installed. If you're using Debian/Ubuntu or their derivatives, you can get them with `sudo apt install --install-suggests mingw-w64-x86-64-dev`. If you're using another distro, check if that package is available.

Then clone this repository.

Before you compile, you'll also need to set up the libraries and include files. Grab them from [here](https://github.com/liballeg/allegro5/releases) and extract them somewhere. Copy the `include`, `bin` and `lib` folders into this one. Finally, run `make release_windows`.

If you're doing this on an older computer like me, a madman, then go get some tea or something while the processor churns through the compilation process.

## Credits

- Art (c) Chaidan and (c) TheAyeStride

- programmming by clocks-in-a-cooler

- font is "Press Start 2P", from Google Fonts