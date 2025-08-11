# macOS Xcode project

This directory contains a minimal Xcode project that wraps the CMake build of **RecoilEngine** as an external build system.

## Regenerating the build files
Run `./regenerate.sh` from this directory to (re)configure the CMake build trees used by Xcode:

```sh
cd macos
./regenerate.sh
```

This creates `build/Debug-Metal` and `build/Release-Metal` directories at the repository root.

## Building in Xcode
1. Open `RecoilEngine.xcodeproj`.
2. Choose either the **Debug-Metal** or **Release-Metal** scheme.
3. Build or run to invoke CMake in the corresponding build directory.
4. Rerun `./regenerate.sh` whenever `CMakeLists.txt` files change to update the build files.
