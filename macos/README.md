# macOS Xcode project

This directory contains an Xcode project that wraps the CMake build of **RecoilEngine** with Metal support enabled.

## Regenerating the build files
`regenerate.sh` is executed automatically by a build phase when a new configuration is built or when the CMake files change. It configures `build/Debug-Metal` and `build/Release-Metal` with `-DUSE_METAL=ON`. The script can still be run manually:

```sh
cd macos
./regenerate.sh -DUSE_METAL=ON
```

## Building in Xcode
1. Open `RecoilEngine.xcodeproj`.
2. Select the **Debug-Metal** or **Release-Metal** scheme.
3. Build or run. The project will:
   - Regenerate the CMake build tree when required.
   - Compile `.metal` shader sources under `cont/base/springcontent/shaders/MSL` using `metal` and `metallib`.
   - Invoke `xcode-build.sh` with `-DUSE_METAL=ON` to build the engine.

## Debugging
Launch the application from Xcode and use **Product > Capture GPU Frame** to debug with Metal's frame capture tools.
