# Fish Fillets

https://github.com/wenhuanli/FishFillets

Fish Fillets is a puzzle game where the player has to guide a fish through a
series of obstacles to escape the maze. Fish Fillets features over 70 levels
of puzzles and a comforting soundtrack.

## Compile requirements

- CMake >= 3.16 — www.cmake.org
- SDL3 (video, input, sound) — libSDL3, www.libsdl.org
- SDL3_mixer (multiple sounds) — libSDL3_mixer, https://github.com/libsdl-org/SDL_mixer
- SDL3_image (PNG support) — libSDL3_image, https://github.com/libsdl-org/SDL_image
- SDL3_ttf (TrueType UTF-8 fonts) — libSDL3_ttf, https://github.com/libsdl-org/SDL_ttf
- Lua >= 5.5 (script language, used for the debug console, configuration, levels, ...) — www.lua.org
- FriBidi (optional, bidirectional text support) — libfribidi, https://github.com/fribidi/fribidi

## How to run

### I. Get the game

Build it from source:

```sh
git clone https://github.com/wenhuanli/FishFillets.git
cd FishFillets
cmake -S . -B build
cmake --build build
cmake --install build
```

Or, if you already have a pre-built `fillets` binary, skip straight to
step III below.

### II. Install data

- Download `fillets-ng-data` from http://fillets.sourceforge.net/download.php
- Extract `fillets-ng-data.tar.gz` to a `$datadir`.
  Default: `/usr/local/share/games/fillets/`

### III. Run it

```sh
./build/src/game/fillets systemdir=$datadir
```

(Using a pre-built binary instead? Run that binary in place of
`./build/src/game/fillets`.)

See also "How to Play" at `$datadir/doc/html/manual.html`.

## Controls

Up, down, left, and right arrow keys control the current fish.
Use the space bar to switch between fish.

| Action | Key |
|---|---|
| Restart level | Backspace |
| Undo | -/+ |
| Help | F1 |
| Save game | F2 |
| Load game | F3 |
| Show/hide move counter | F5 |
| Show/hide subtitles | F6 |
| Game menu | F10 |
| Fullscreen | F11 |
| Faster game | Hold Shift |
