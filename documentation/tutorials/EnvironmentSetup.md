# Tutorial: Development Setup & System Requirements

## Base Requirements
*   **Compiler**: A C++23 compiler whose standard library ships a `std` module (`import std;`):
    *   **GCC 15+** (libstdc++)
    *   **Clang 18.1.2+** with libc++ (the default on macOS, or `-stdlib=libc++` on Linux), or Clang with GCC 15+'s libstdc++
    *   **MSVC** from Visual Studio 2022 17.6+ (on Windows, use MSVC rather than Clang)
*   **Build System**: CMake 3.28 or newer (the first version with stable C++ modules support)
*   **Generator**: Ninja
*   **Test Suite**: GoogleTest (gtest)
*   **(Optional) Multithreading**: OpenMP (libomp). See the [Multithreading Guide](MultithreadingGuide.md) for instructions on how to install and enable it.

> **How `import std;` works here:** CMake's own `import std` support is still experimental, so the project builds the standard library module itself. [`cmake/StdModule.cmake`](../../cmake/StdModule.cmake) asks your compiler where its `std` module source lives (via the modules manifest the toolchain ships), compiles it as the `std_module` target, and links it into everything. Nothing needs configuring, but this is why the compiler versions above matter: older toolchains don't ship a `std` module at all.

> **Note on GoogleTest:** You do not need to manually install GoogleTest (gtest) on your system. The project's `CMakeLists.txt` is configured to fetch and build GoogleTest automatically when you configure and build the project using CMake.

---

## Operating System Guides

### 1. Windows

#### 1.1 Visual Studio

The most reliable environment for C++ modules on Windows is **Visual Studio 2022** (version 17.6 or newer, which ships the `std` module source alongside the MSVC toolset).

1. Download and run the Visual Studio 2022 Installer (the free Community edition is sufficient).
2. Under the "Workloads" tab, select **Desktop development with C++**.
3. In the "Installation details" panel on the right, ensure the following components are checked:
   *   `MSVC v143 - VS 2022 C++ x64/x86 build tools` (or newer)
   *   `C++ CMake tools for Windows` *(Note: This conveniently includes both CMake and Ninja).*

*Note: OpenMP is built natively into MSVC. No extra package installation is required for multithreading.*

#### 1.2. Standalone Terminal Setup
If you prefer not to install the full Visual Studio IDE, you can install the MSVC compiler on its own (the **Build Tools for Visual Studio 2022**) plus CMake and Ninja via the command line.

> [!NOTE]
> Use MSVC on Windows, not a standalone LLVM/Clang install. Clang on Windows uses Microsoft's standard library, and its `std` module can only be built by MSVC, so CMake stops with an error if Clang is selected.

1. **Install the Toolchain**: Open PowerShell as an Administrator and run:

   - Install the MSVC compiler (Build Tools with the C++ workload)
   ```powershell
   winget install Microsoft.VisualStudio.2022.BuildTools --override "--add Microsoft.VisualStudio.Workload.VCTools --includeRecommended --passive"
   ```

   - Install CMake (Build system)
   ```powershell
   winget install Kitware.CMake
   ```
   
   - Install Ninja (Generator)
   ```powershell
   winget install Ninja-build.Ninja
   ```

2. **Build from a Developer shell**: open **Developer PowerShell for VS 2022** from the Start menu before running CMake. It puts `cl.exe` on the `PATH` and sets `VCToolsInstallDir`, which is how the build locates the `std` module.

### macOS

Apple's default compiler (Apple Clang) often lags behind in its support for C++23 modules, and it does not ship with OpenMP by default. It is highly recommended to use Homebrew to install the official LLVM toolchain, CMake, Ninja, and OpenMP.

1. Install the necessary toolchain and libraries using Homebrew:
   ```bash
   brew install llvm cmake ninja libomp
   ```
2. **Important**: You must export LLVM to your path before running CMake to override the default Apple Clang compiler. Add the following to your shell profile (e.g., `~/.zshrc`) or run them in your terminal before building:
   ```bash
   export CC=/opt/homebrew/opt/llvm/bin/clang
   export CXX=/opt/homebrew/opt/llvm/bin/clang++
   ```
   Homebrew's Clang uses libc++, which the build detects automatically; no `-stdlib` flag is needed.

### Linux

#### Ubuntu / Debian

Default package managers in Ubuntu/Debian often contain outdated versions of CMake that do not support C++23 modules. You must use the official Kitware repository to get CMake 3.28+.

1. Install the GCC 15 compiler, Ninja, and the OpenMP development library (GCC 15 is the first release whose libstdc++ ships the `std` module):
   ```bash
   sudo apt update && sudo apt install g++-15 ninja-build libomp-dev
   ```
   If your Ubuntu release does not package `g++-15` yet, add the toolchain PPA first with `sudo add-apt-repository ppa:ubuntu-toolchain-r/test`. Then point CMake at it, e.g. `cmake -B build -G Ninja -DCMAKE_CXX_COMPILER=g++-15`.
2. Add the Kitware repository signature and list to get the latest CMake:
   ```bash
   wget -O - [https://apt.kitware.com/keys/kitware-archive-latest.asc](https://apt.kitware.com/keys/kitware-archive-latest.asc) 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
   
   sudo apt-add-repository "deb [https://apt.kitware.com/ubuntu/](https://apt.kitware.com/ubuntu/) $(lsb_release -cs) main"
   ```
3. Install CMake:
   ```bash
   sudo apt update && sudo apt install cmake
   ```

#### Fedora

Fedora generally keeps its packages up to date, making the installation straightforward.

1. Install the complete toolchain and OpenMP:
   ```bash
   sudo dnf install gcc-c++ cmake ninja-build libomp
   ```

#### Arch Linux

Arch Linux repositories are rolling-release and already contain the necessary up-to-date versions of all required tools.

1. Install the complete toolchain and OpenMP:
   ```bash
   sudo pacman -S gcc cmake ninja openmp
   ```
