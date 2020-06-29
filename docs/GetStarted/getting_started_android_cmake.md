# Getting Started on Android with CMake

<!--
Notes to those updating this guide:

    * This document should be __simple__ and cover essential items only.
      Notes for optional components should go in separate files.
-->

This guide walks through cross-compiling IREE core runtime towards the Android
platform. Cross-compiling IREE compilers towards Android is not supported at the
moment.

Cross-compilation involves both a *host* platform and a *target* platform. One
invokes compiler toolchains on the host platform to generate libraries and
executables that can be run on the target platform.

## Prerequisites

### Set up host development environment

The host platform should have been set up for developing IREE. Right now Linux
and Windows are supported. Please make sure you have followed the steps for
[Linux](./getting_started_linux_cmake.md) or
[Windows](./getting_started_windows_cmake.md).

### Install Android NDK

Android NDK provides compiler toolchains for compiling C/C++ code to target
Android. You can download it
[here](https://developer.android.com/ndk/downloads). We recommend to download
the latest release; the steps in following sections may assume that.

Alternatively, if you have installed
[Android Studio](https://developer.android.com/studio), you can follow
[this guide](https://developer.android.com/studio/projects/install-ndk) to
install Android NDK.

After downloading, it is recommended to set the `ANDROID_NDK` environment
variable pointing to the directory. For Linux, you can `export` in your shell's
rc file. For Windows, you can search "environment variable" in the taskbar or
use `Windows` + `R` to open the "Run" dialog to run
`rundll32 sysdm.cpl,EditEnvironmentVariables`.


### Install Android Debug Bridge (ADB)

For Linux, search your the distro's package manager to install `adb`.
For example, on Ubuntu:

```shell
$ sudo apt install adb
```

For Windows, it's easier to get `adb` via Android Studio. `adb` is included in
the Android SDK Platform-Tools package. You can download this package with the
[SDK Manager](https://developer.android.com/studio/intro/update#sdk-manager),
which installs it at `android_sdk/platform-tools/`. Or if you want the
standalone Android SDK Platform-Tools package, you can
[download it here](https://developer.android.com/studio/releases/platform-tools).
You may also want to add the folder to the `PATH` environment variable.

## Configure and build

### Configure on Linux

```shell
# Assuming in IREE source root
$ cmake -G Ninja -B build-android  \
    -DCMAKE_TOOLCHAIN_FILE=$ANDROID_NDK/build/cmake/android.toolchain.cmake \
    -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-29 \
    -DIREE_BUILD_COMPILER=OFF -DIREE_BUILD_TESTS=OFF -DIREE_BUILD_SAMPLES=OFF \
    -DIREE_HOST_C_COMPILER=`which clang` -DIREE_HOST_CXX_COMPILER=`which clang++`
```

*   The above configures IREE to cross-compile towards 64-bit
    (`-DANDROID_ABI="arm64-v8a"`) Android 10 (`-DANDROID_PLATFORM=android-29`).
    This may require the latest Android NDK release. You can choose the suitable
    [`ANDROID_ABI`](https://developer.android.com/ndk/guides/cmake#android_abi)
    and
    [`ANDROID_PLATFORM`](https://en.wikipedia.org/wiki/Android_version_history)
    for your target device. You can also refer to Android NDK's
    [CMake documentation](https://developer.android.com/ndk/guides/cmake) for
    more toolchain arguments.
*   Building IREE compilers, tests, and samples for Android is not supported at
    the moment; they will be enabled soon.
*   We need to define `IREE_HOST_{C|CXX}_COMPILER` to Clang here because IREE
    does [not support](https://github.com/google/iree/issues/1269) GCC well at
    the moment.

### Configure on Windows

On Windows, we will need the full path to the `cl.exe` compiler. This can be
obtained by [opening a developer command prompt window](https://docs.microsoft.com/en-us/cpp/build/building-on-the-command-line?view=vs-2019#developer_command_prompt) and type
`where cl.exe`. Copy the path and substitue all `\` with `/` to get the
CMake-style path. Then in a command prompt (`cmd.exe`):

```cmd
REM Assuming in IREE source root
> cmake -G Ninja -B build-android  \
    -DCMAKE_TOOLCHAIN_FILE="%ANDROID_NDK%/build/cmake/android.toolchain.cmake" \
    -DANDROID_ABI="arm64-v8a" -DANDROID_PLATFORM=android-29 \
    -DIREE_BUILD_COMPILER=OFF -DIREE_BUILD_TESTS=OFF -DIREE_BUILD_SAMPLES=OFF \
    -DIREE_HOST_C_COMPILER="<cmake-style-path-to-cl.exe>" \
    -DIREE_HOST_CXX_COMPILER="<cmake-style-path-to-cl.exe>" \
    -DLLVM_HOST_TRIPLE="x86_64-pc-windows-msvc" \
    -DCROSS_TOOLCHAIN_FLAGS_NATIVE="-DCMAKE_C_COMPILER=\"<cmake-style-path-to-cl.exe>\";-DCMAKE_CXX_COMPILER=\"<cmake-style-path-to-cl.exe>\""
```

* See the Linux section in the above for explanations of the used arguments.
* We need to define `LLVM_HOST_TRIPLE` and `CROSS_TOOLCHAIN_FLAGS_NATIVE` in the
  above because LLVM cannot properly detect host triple under Android CMake
  toolchain file. This might be fixed later.

### Build all targets

```shell
$ cmake --build build-android/
```

## Test on Android

Make sure you
[enable developer options and USB debugging](https://developer.android.com/studio/debug/dev-options#enable)
for your Android device.

Connect your Android device to the development machine and make sure you can see
the device when:

```shell
$ adb devices

List of devices attached
XXXXXXXXXXX     device
```

### VMLA HAL backend

Translate a source MLIR into IREE module:

```shell
# Assuming in IREE source root
$ build-android/host/bin/iree-translate \
    -iree-mlir-to-vm-bytecode-module \
    -iree-hal-target-backends=vmla \
    iree/tools/test/simple.mlir \
    -o /tmp/simple-vmla.vmfb
```

Then push the IREE runtime executable and module to the device:

```shell
$ adb push build-android/iree/tools/iree-run-module /data/local/tmp/
$ adb shell chmod +x /data/local/tmp/iree-run-module
$ adb push /tmp/simple-vmla.vmfb /data/local/tmp/
```

Log into Android:

```shell
$ adb shell

android $ cd /data/local/tmp/
android $ ./iree-run-module -driver=vmla -input_file=simple-vmla.vmfb -entry_function=abs -inputs="i32=-5"

EXEC @abs
i32=5
```

### Vulkan HAL backend

Please make sure your Android device is Vulkan capable. Vulkan is supported on
Android since 7, but Android 10 is our primary target at the moment.

Translate a source MLIR into IREE module:

```shell
# Assuming in IREE source root
$ build-android/host/bin/iree-translate \
    -iree-mlir-to-vm-bytecode-module \
    -iree-hal-target-backends=vulkan-spirv \
    iree/tools/test/simple.mlir \
    -o /tmp/simple-vulkan.vmfb
```

Then push the IREE runtime executable and module to the device:

```shell
$ adb push build-android/iree/tools/iree-run-module /data/local/tmp/
$ adb shell chmod +x /data/local/tmp/iree-run-module
$ adb push /tmp/simple-vulkan.vmfb /data/local/tmp/
```

Log into Android:

```shell
$ adb shell

android $ cd /data/local/tmp/
android $ ./iree-run-module -driver=vulkan -input_file=simple-vulkan.vmfb -entry_function=abs -inputs="i32=-5"

EXEC @abs
i32=5
```

#### Common issues

##### Vulkan function `vkCreateInstance` not available

This can happen on Android devices with ARM Mali GPUs, where there is only one
monolithic driver (`/vendor/lib[64]/libGLES_mali.so`) and the vulkan vendor
driver (`/vendor/lib[64]/hw/vulkan.*.so`) is just a symlink to it. This causes
problems for Vulkan device enumeration under `/data/local/tmp/`. A known
workaround is to copy the `libGLES_mali.so` library under `/data/local/tmp/` and
rename it as `libvulkan.so` and then prefix `LD_LIBRARY_PATH=/data/local/tmp`
when invoking IREE executables.