# How-To: Run Tests

This guide provides practical instructions on how to execute the existing test suite.

> **Important:** The project must be built before you can run the tests. If you haven't built the project yet, please follow the [Getting Started Tutorial](../tutorials/GettingStarted.md) first.

## How to Run Existing Tests

If you have already built the project, you can run the tests by executing the binary produced by CMake.

1. Navigate to your `build` directory:
   ```bash
   cd build
   ```
2. Execute the test runner:
   ```bash
   ./run_tests
   ```

### Running Specific Tests

Sometimes you only want to run a specific subset of tests to save time. You can use the `--gtest_filter` flag to achieve this:

```bash
# Run all tests related to Tuples
./run_tests --gtest_filter=Tuple.t.*

# Run a specific test case
./run_tests --gtest_filter=Tuple.t.IsAPoint
```
