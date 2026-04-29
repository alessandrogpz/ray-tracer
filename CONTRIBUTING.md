# Contributing to the Ray Tracer

## Code of Conduct

By participating in this project, you agree to abide by common open-source standards of respect and professionalism. Please be welcoming and constructive in your interactions with others.

## How Can I Contribute?

### Reporting Bugs
If you find a bug in the source code or a mistake in the documentation, you can help us by submitting an issue to our repository. Before submitting an issue, please search existing issues to see if it has already been reported.

### Suggesting Enhancements
Enhancement suggestions are tracked as GitHub issues. When creating an enhancement issue, please provide a clear and descriptive title and a detailed description of the proposed feature or improvement.

### Pull Requests
I gladly accept pull requests for bug fixes, new features, and documentation improvements.

1. **Fork** the repository and create your branch from `main`.
2. **Build and test** your changes locally. 
3. Ensure your code follows the established coding standards.
4. **Create a Pull Request** with a clear title and description.

## Development Setup

To set up your local development environment, please follow our step-by-step guide:
**[Getting Started Tutorial](documentation/tutorials/getting_started.md)**

### Prerequisites Refresher
- A modern C++ compiler supporting **C++23**
- **CMake** (3.28+ required for C++ Modules)
- **Ninja** build system (required for C++ Modules)
- **GoogleTest** (for running the test suite)

## Testing Guidelines

This project strictly follows **Test-Driven Development (TDD)** principles. 
- All new features and mathematical operations **must** be accompanied by comprehensive unit tests using GoogleTest.
- Before submitting a pull request, ensure all tests pass successfully. 

For detailed instructions on how to write and run tests, see:
**[How-To: Run Tests](documentation/how-to/run_tests.md)**

## Coding Standards

- **Language:** C++23.
- **Style:** We aim for clean, readable code. Please adhere to modern C++ best practices (e.g., using smart pointers where appropriate, `auto` for type deduction when it enhances readability, and `constexpr` for compile-time constants).
- **Parameter Passing:** Pass large objects (like `Canvas` or `Matrix`) by `const Type&` to avoid expensive memory copies. Pass fundamental types and small structs (like `Tuple`, `Point`, `Vector`, `Color`) by value, as copying tiny amounts of data is often faster and more cache-friendly for the CPU. More can be read at **[Reference: C++ Implementation Details](documentation/references/c++_implementation_details.md)**
- **Naming Conventions:** 
  - Classes and Structs: `PascalCase`
  - Functions: `camelCase`
  - Variables: `snake_case`
  - Macros/Constants: `UPPER_SNAKE_CASE`
- **Documentation:** If you add a complex algorithm or modify the core architecture, please update the corresponding conceptual documentation in `documentation/explanation/`.

## Git Commit Guidelines

We follow strict clean code practices for version control. Commits should be atomic (one logical change per commit).

### Commit Message Format
We use conventional commits. We expect a main title on the first line, followed by a blank line, and then a brief description in a separate line (or more, if there is a need for more than one description paragraph).

**Example:**

```bash
git commit -m "<type>: <title>" \
-m "<detailed description paragraph 1>" \
-m "<detailed description paragraph 2 (optional)>"
```

### Commit Types

**Core types (use these most):**
- `feat`: New feature for the user
- `fix`: Bug fix for the user
- `docs`: Documentation changes only
- `style`: Formatting, missing semicolons, etc. (no code change)
- `refactor`: Code restructuring (no feature change or bug fix)
- `test`: Adding or updating tests
- `chore`: Maintenance tasks (dependencies, build config, etc.)

**Additional types:**
- `perf`: Performance improvements
- `build`: Changes to build system or dependencies
- `ci`: Continuous integration changes
- `revert`: Reverting a previous commit

## Understanding the Architecture

Before making significant changes, we recommend reading our architectural overview to understand how the ray tracer pipeline operates:
**[Architecture Explanation](documentation/explanation/architecture.md)**

Thank you for contributing!
