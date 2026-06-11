# How-To: Write Tests

This guide provides practical instructions on how to author new tests using the GoogleTest (gTest) framework.

## How to Write a New Test

Tests are located in the `tests/` directory. If you are adding a new feature or fixing a bug, you should write a test to verify the behavior.

1. **Create or Open a Test File**
   Open the appropriate `.cpp` file in the `tests/` directory (e.g., `tests/Tuple.t.cpp`).

2. **Include Dependencies**
   Ensure you include the GoogleTest header and the header for the code you are testing.
   ```cpp
   #include <gtest/gtest.h>
   import rt.tuple; // Replace with your module
   ```

3. **Write the Test Macro**
   Use the `TEST()` macro provided by GoogleTest. The first argument is the Test Suite Name (usually the class or module name), and the second is the Test Case Name.
   ```cpp
    TEST(TupleTest, IsAPoint) {
        // 1. Arrange (Set up the test data)
        tuple p = createPoint(4.3, -4.2, 3.1);

        // 2. Act & Assert (Verify the expected behavior)
        EXPECT_DOUBLE_EQ(p.w, 1.0);
    }
    ```

> [!WARNING]
> **Important:** Do not write a `main()` function in your test files. GoogleTest provides its own `main()` automatically via the `GTest::gtest_main` library linked in `CMakeLists.txt`.

### Pro-Tips for Floating Point Math

When testing mathematical operations, exact floating-point comparisons (`==`) are dangerous due to precision issues.

- Use `EXPECT_DOUBLE_EQ(val1, val2)` for simple double checks.
- Use `EXPECT_NEAR(val1, val2, 0.0001)` when checking complex math or matrix transformations to account for rounding errors.

## How to Add a New Test File to CMake

If you created an entirely new test file (e.g., `tests/NewFeatureTests.cpp`), you must tell CMake to compile it.

1. Open `CMakeLists.txt` in the root directory.
2. Locate the `add_executable(run_tests ...)` block.
3. Add your new file to the list:
   ```cmake
   add_executable(run_tests 
       tests/Tuple.t.cpp 
       # ... other files ...
       tests/NewFeature.t.cpp   # <-- Add your file here
   )
   ```
4. Re-run `cmake ..` in your build directory to apply the changes.
