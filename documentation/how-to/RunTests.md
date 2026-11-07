# How-To: Run Tests

This guide provides practical instructions on how to execute the existing test suite.

> **Important:** The project must be built before you can run the tests. If you haven't built the project yet, please follow the [Getting Started Tutorial](../tutorials/GettingStarted.md) first.

## How to Run Existing Tests

If you have already built the project, you can run the tests by executing the binary produced by CMake.

1. Navigate to your build directory (`build`, `build-debug` or `build-release`, depending on how you configured):
   ```bash
   cd build
   ```
2. Execute the test runner:
   ```bash
   ./run_tests
   ```

### Running Specific Tests

Sometimes you only want to run a specific subset of tests to save time. You can use the `--gtest_filter` flag to achieve this:

Filters match `Suite.Test`, the two names given to `TEST(Suite, Test)`:

```bash
# Run every test in one suite
./run_tests --gtest_filter='TupleCreation.*'

# Run a specific test case
./run_tests --gtest_filter='TupleCreation.CreatePointSetsWToOne'

# Run several suites at once (e.g. everything about refraction and Fresnel)
./run_tests --gtest_filter='Refraction.*:Fresnel.*'

# List every suite and test without running them
./run_tests --gtest_list_tests
```
