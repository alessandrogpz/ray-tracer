# Ray Tracer

This is a C++ implementation of a 3D Ray tracer, built from scratch following test-driven development principles.

## Overview

This project simulates light rays to render 3D scenes. It's built for performance and educational purposes, adhering strictly to modern C++ standards.

## Documentation

Documentation of this project is structured using the [Diátaxis framework](https://diataxis.fr/).

- **Tutorial**: A step-by-step guide to build and run the Ray tracer.
    - [Getting Started](documentation/tutorials/GettingStarted.md)
    - [Environment Setup](documentation/tutorials/EnvironmentSetup.md)
    - [Multithreading Guide](documentation/tutorials/MultithreadingGuide.md)
- **How-To Guides**: Practical guides for specific tasks.
    - [Run Tests](documentation/how-to/RunTests.md)
    - [Write Tests](documentation/how-to/WriteTests.md)
    - [Profile Code](documentation/how-to/ProfileCode.md)
- **References**: Technical descriptions of the underlying APIs, code style, and mathematical structures.
    - [Core Mathematics](documentation/references/CoreMath.md)
    - [C++ Implementation Details](documentation/references/C++ImplementationDetails.md)
    - [C++ Module & Build Architecture](documentation/references/C++Architecture.md)
    - [Docstring Guidelines](documentation/references/DocstringGuidelines.md)
- **Explanation**: High-level architectural overview and explanations of algorithms and concepts used.
    - [Architecture](documentation/explanation/Architecture.md)
    - [Ray-Sphere Intersection](documentation/explanation/RaySphereIntersectionAlgorithm.md)
    - [Phong Reflection Model](documentation/explanation/PhongReflectionModel.md)
    - [Data-Oriented Design (DOD)](documentation/explanation/DataOrientedDesign.md)

## Contributing

Contributions are welcome! Please refer to [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.

## Tech Stack
- **Language**: C++23
- **Build System**: CMake (3.28+) and Ninja
- **Testing**: GoogleTest (gTest)
- (Optional) **Multithreading**: OpenMP (libomp)
