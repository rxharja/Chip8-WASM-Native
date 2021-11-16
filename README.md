# Chip8-WASM-Native
A CHIP-8 Emulator built entirely in the C programming language which can be compiled for both web assembly web applications and native MacOS Intel ones currently.

![](https://github.com/rxharja/Chip8-WASM-Native/blob/main/media/invaders.gif)

## Building Locally

Currently the only supported native build for the emulator is for MacOS

###Dependencies:
The main dependency for the project is SDL2. Installing SDL2 on MacOS can be done via

```
brew install sdl2
```

Building for MacOS:

```
git clone git@github.com:rxharja/Chip8-WASM-Native.git
cd Chip8-WASM-Native
make
```

Running the Native Application and play Pong:
```
./bin/Chip8 ./bin/c8games/PONG2
```

## Building for Web Browsers
This repo already includes the web assembly binary prebuilt which can be accessed by both React and Plain HTML
![](https://github.com/rxharja/Chip8-WASM-Native/blob/main/media/plain_html.png?raw=true)
To rebuild it, assuming the current working directory is the repo root:

```
make wasm
```

## To Do
- [x] MacOS Support
- [x] Web Support
- [ ] Windows Support
