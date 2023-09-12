# Tic-Tac-Toe using Raylib

Tic-Tac-Toe written in C++. Uses Raylib for rendering.

# Running locally

Compiled WASM file is included in `app/`.
Run a local server,
```
cd app
python -m http.server --bind 127.0.0.1
```

# Building
Run the build script,
```
chmod +x build.sh
./build.sh
```
To also build for web,
```
./build.sh -w
```
Or,
```
mkdir -p build/desktop
cd build/desktop
cmake ../..
cmake --build .
```

The compiled binary is in `build/ttt`.

For web,
```
mkdir -p build/web
cd build/web
emcmake cmake ../..
cmake --build .
```

To run a local server for testing, copy `ttt.js` and `ttt.wasm` from `build/` to `app/`, which is done by `build.sh`,

## Requirements to build for web
- Emscripten


# TODOs
- Make it look better
- AI opponent
- Versus play online
