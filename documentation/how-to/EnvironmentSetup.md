# How-To: Development Setup & System Requirements

## Base Requirements
*   **Compiler**: A C++23 compatible compiler (MSVC, Clang 16+, or GCC 13+)
*   **Build System**: CMake (3.28 or newer)
*   **Generator**: Ninja
*   **Test Suit** GoogleTest (gtest).

> **Note on GoogleTest:** You do not need to manually install GoogleTest (gtest) on your system. The project's `CMakeLists.txt` is configured to fetch and build GoogleTest automatically when you configure and build the project using CMake.

---

## Operating System Guides

### Windows

The most reliable environment for C++ modules on Windows is **Visual Studio 2022**.

1. Download and run the Visual Studio 2022 Installer (the free Community edition is sufficient).
2. Under the "Workloads" tab, select **Desktop development with C++**.
3. In the "Installation details" panel on the right, ensure the following components are checked:
   *   `MSVC v143 - VS 2022 C++ x64/x86 build tools` (or newer)
   *   `C++ CMake tools for Windows` *(Note: This conveniently includes both CMake and Ninja).*

### macOS

Apple's default compiler (Apple Clang) often lags behind in its support for C++23 modules. It is highly recommended to use Homebrew to install the official LLVM toolchain alongside CMake and Ninja.

1. Install the necessary toolchain using Homebrew:
   ```bash
   brew install llvm cmake ninja
   ```
2. **Important**: You must export LLVM to your path before running CMake to override the default Apple Clang compiler. Add the following to your shell profile (e.g., `~/.zshrc`) or run them in your terminal before building:
   ```bash
   export CC=/opt/homebrew/opt/llvm/bin/clang
   export CXX=/opt/homebrew/opt/llvm/bin/clang++
   ```

### Linux

#### Ubuntu / Debian

Default package managers in Ubuntu/Debian often contain outdated versions of CMake that do not support C++23 modules. You must use the official Kitware repository to get CMake 3.28+.

1. Install the GCC 13 compiler and Ninja:
   ```bash
   sudo apt update && sudo apt install g++-13 ninja-build
   ```
2. Add the Kitware repository signature and list to get the latest CMake:
   ```bash
   wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
   
   sudo apt-add-repository "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
   ```
3. Install CMake:
   ```bash
   sudo apt update && sudo apt install cmake
   ```

#### Arch Linux

Arch Linux repositories are rolling-release and already contain the necessary up-to-date versions of all required tools.

1. Install the complete toolchain:
   ```bash
   sudo pacman -S gcc cmake ninja
   ```
