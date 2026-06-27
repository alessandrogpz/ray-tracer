# How-To: Profile Code

This guide provides practical instructions on how to profile your C++ ray tracer on Linux using the kernel-level `perf` tool and its integration inside JetBrains CLion.

---

## 1. CLion Profiler Setup & Integration

CLion leverages the Linux `perf` tool to display graphical call trees and **Flame Graphs** natively within the IDE.

### System Configuration (Run Once)
For security reasons, Linux restricts user-level profiling by default. Run the following commands in your terminal to grant CLion the necessary permissions:
```bash
# Allow users to collect performance counters and call stacks (paranoid level 1)
sudo sysctl -w kernel.perf_event_paranoid=1

# Allow users to view kernel pointer addresses (symbol resolution)
sudo sysctl -w kernel.kptr_restrict=0
```

> [!TIP]
> **Make permissions permanent across reboots:**
> Run:
> `sudo sh -c 'echo "kernel.perf_event_paranoid=1" >> /etc/sysctl.d/99-perf.conf'`
> `sudo sh -c 'echo "kernel.kptr_restrict=0" >> /etc/sysctl.d/99-perf.conf'`
> Then apply: `sudo sysctl --system`

### Configuring CLion
1. Open settings: **File ➡️ Settings** (or `Ctrl + Alt + S`).
2. Navigate to **Build, Execution, Deployment ➡️ Dynamic Analysis Tools ➡️ Profilers**.
3. Set **Perf executable** to `/usr/bin/perf` (or run `which perf` to find your path).
4. **Important:** Check **Show inlined functions** to trace inline-optimized math equations.
5. Check **Delete file(s) on exit** to automatically clear profiler temp files when exiting CLion.

---

## 2. Command-Line `perf` Reference

### `perf stat` (Quick Hardware Summary)
Displays high-level CPU hardware performance counters for a single program run.

```bash
# 1. Standard hardware counter summary
perf stat ./cmake-build-release/6.MultipleSpherePhongReflections

# 2. Detailed cache-miss analysis (L1 vs Last Level Cache)
perf stat -e cache-references,cache-misses,L1-dcache-loads,L1-dcache-load-misses,LLC-loads,LLC-load-misses ./cmake-build-release/6.MultipleSpherePhongReflections

# 3. For AMD Zen/Ryzen architectures (if LLC counters return <not supported>):
perf stat -e l1-dcache-loads,l1-dcache-load-misses,l2_cache_accesses_from_dc_misses,l2_cache_misses_from_dc_misses ./cmake-build-release/6.MultipleSpherePhongReflections
```

### `perf record` & `perf report` (Deep CPU Profiling)
Records sample stacks to a file to show exactly where CPU execution spent its time.

```bash
# 1. Record call stacks (-g tells it to collect call graphs)
perf record -g ./cmake-build-release/6.MultipleSpherePhongReflections

# 2. Open interactive terminal user interface (TUI) report
perf report
```

#### TUI Keyboard Shortcuts in `perf report`:
* **Up / Down Arrow Keys:** Navigate the function list.
* **Enter:** Expand or collapse a call branch.
* **`a`:** Open assembly annotator. This shows you exactly which line of C++ code compiles into which CPU instructions (and highlights the hottest assembly lines in red!).
* **`q`:** Quit the report screen.

### Managing and Deleting `perf` Records
By default, `perf record` saves samples to a file named `perf.data` in the folder where you run the command.

* **Overwriting:** You do **not** need to delete old logs. Running `perf record` automatically renames the old file to `perf.data.old` and overwrites `perf.data`.
* **Checking File Size:** `ls -lh perf.data` (these files can grow to several gigabytes for long runs).
* **Deleting Records:** Run `rm perf.data*` to free up disk space when you are finished profiling.

---

## 3. Explaining `perf stat` Metrics

When you run `perf stat`, it prints a table containing hardware/software event counters. Below is an explanation of what each line means:

### General Performance Counters
| Counter Name | Typical Value | What it represents |
| :--- | :--- | :--- |
| **`task-clock`** | `2.724,50 msec` | The total CPU time allocated to run your program (across all threads/cores). A high value indicates high CPU usage. |
| **`CPUs utilized`** | `13.4 CPUs` | Tells you how many cores your parallel threads fully occupied. If you run a 16-threaded OpenMP loop, this number should ideally approach `16.0`. |
| **`context-switches`** | `310` | Times the CPU suspended your program to run another task. Lower is better, as context switches cause latency. |
| **`cpu-migrations`** | `56` | Times the OS shifted a running thread from one physical CPU core to another. Keep low to avoid cache coldness. |
| **`page-faults`** | `3.392` | Times the OS had to assign a physical RAM page to your program (lazy allocation). Normal on first writes to large buffers. |
| **`cycles`** | `10.370.5f35.468` | Raw CPU clock cycles executed. Useful for raw processor workload measurement. |
| **`instructions`** | `13.161.633.277` | Total machine instructions executed. |
| **`insn per cycle` (IPC)** | `1.3` | **Instructions per Cycle.** Tells you how many instructions the CPU is completing on average per clock tick. Higher is better (ideal is 1.5–2.0+). Low IPC (<0.5f) means CPU is stalling. |
| **`branch-misses`** | `6.633.258` | Times the CPU branch predictor guessed wrong on a branch (`if`, loops). |
| **`branch_miss_rate`** | `0.2f%` | **Branch Miss Rate.** The percentage of wrong guesses. **Lower is better.** Below $1\%$ is excellent. High branch miss rates cause CPU pipeline stalls. |
| **`frontend_cycles_idle`**| `7%` | Percentage of clock cycles where the CPU instruction decoder was idling (waiting for memory or instructions). Lower is better. |

### Cache Performance Counters
| Counter Name | What it represents | How to Interpret It |
| :--- | :--- | :--- |
| **`L1-dcache-loads`** | Total attempts to read data from the Level 1 Data Cache. | Higher is normal. Tells you the absolute volume of data requests. |
| **`L1-dcache-load-misses`** | Total failed L1 reads (data had to be retrieved from L2). | **Lower is better.** Divide this by loads to get your L1 miss rate (below $1\%$ is excellent!). |
| **`cache-references`** | Memory reads that missed the L1 cache and had to query L2/L3. | Represents the pressure being put on your secondary cache levels. |
| **`cache-misses`** | Memory reads that missed L1, L2, and L3, forcing a read from main RAM. | **Lower is better.** Represents the worst-case scenario where the CPU had to stall to fetch from DRAM. |
| **`l2_cache_accesses_from_dc_misses`** | (AMD Zen specific) Total read requests reaching the L2 cache. | Corresponds to L1 data cache misses. |
| **`l2_cache_misses_from_dc_misses`** | (AMD Zen specific) Reads that missed L2 cache and went to L3/DRAM. | **Lower is better.** Divide this by L2 accesses to get your L2 miss rate. |

---

## 4. Didactic Guide: CPU Cache Hierarchy (L1, L2, L3)

To understand why our cache miss rate is so important, think of CPU memory like a workspace hierarchy:

1. **Registers (The Desktop Surface):** Holds the data currently being operated on (takes **0 clock cycles** to access).
2. **L1 Cache (The Top Desk Drawer):** Extremely fast, dedicated to each CPU core (takes **1–4 clock cycles**). This is where our local loop variables, active ray structures, and current pixel colors live.
3. **L2 Cache (The Filing Cabinet in your cubicle):** Slightly larger, slightly slower (takes **10–20 clock cycles**).
4. **L3 Cache / LLC (The Shared Supply Room in the hallway):** Shared across all CPU cores (takes **40–60 clock cycles**).
5. **System RAM / DRAM (Driving to the store):** Massive storage, but incredibly slow compared to CPU speed (takes **100–300+ clock cycles**).

When the CPU needs a piece of data (like a Sphere's material or transformation matrix), it looks in order: **L1 ➡️ L2 ➡️ L3 ➡️ RAM**.
* **Cache Hit:** The CPU finds the data in L1 or L2 and continues executing at full speed.
* **Cache Miss (RAM Fetch):** The CPU must pause execution and stall for hundreds of clock cycles while the data is fetched from RAM.

#### Why our Ray Tracer has a 99.77% Cache Hit Rate:
* **Contiguous Vectors (Data-Oriented Design):** We replaced array-of-pointers structures (which scatter data all over RAM) with contiguous arrays (`std::vector<Sphere>`). When the CPU loads one Sphere, it automatically pre-fetches the adjacent Spheres into L1 cache.
* **Allocation-Free Loop:** We eliminated heap allocations (`new` or `std::vector::push_back` that calls `malloc`) inside the rendering loop. Heap allocations require OS system calls and scatter memory locations, which triggers cache misses.
* **Inlined Math Functions:** We forced standard math functions (`std::sqrt`) to compile inline. This avoids the overhead of function calls jumping to distant instruction addresses.

---

## 5. Didactic Guide: Valgrind (Memcheck & Cachegrind)

Valgrind is an instrumentation framework. Instead of running your compiled machine code directly on the CPU, Valgrind runs your program inside a virtual machine that intercepts every memory access.

### Tool 1: Memcheck (Memory Debugger)
Used to find memory leaks, float frees, and invalid reads/writes.
* **Command:** `valgrind --tool=memcheck ./your_program`
* **CLion Integration:** Configured in Settings. Once the Valgrind executable is set, select **Run with Valgrind Memcheck** from the top Run menu.
* **OpenMP Note:** In multi-threaded programs using OpenMP or `libpthread`, you might see warnings like `Leak_PossiblyLost` pointing to thread management libraries. These are normal background structures created by the compiler's OpenMP library and are not bugs in your code.

### Tool 2: Cachegrind (Cache Simulator)
Simulates a detailed L1/L2/L3 cache system to show exactly which line of code is causing cache misses.
* **Command:** `valgrind --tool=cachegrind ./your_program`
* **Analysing Results:** Run `cg_annotate cachegrind.out.<pid>` in the terminal to view a line-by-line breakdown of cache hits/misses for each source file.
* **Why is Valgrind so slow?** Because Valgrind compiles and translates every machine instruction at runtime, it introduces a **50x to 100x slowdown**. A ray-tracer run that takes 190ms normally will take 10 to 15 seconds under Valgrind. This is completely normal!

---

## 6. Key Takeaways for Ray Tracers

1. **Compilation Mode:** Always profile in **`Release`** mode. Profiling `Debug` mode will show you std::vector boundary checks, lack of inlining, and call stack overhead, hiding the actual mathematical bottlenecks.
2. **The C++ Module Math Gotcha:** If you see high self-time inside the standard library but low CPU performance, standard math functions (`std::sqrt`) imported through `import std;` might not be getting inlined as builtins. Restoring `#include <cmath>` in the global module fragment fixes this.
3. **Data-Oriented Design (DOD):** High L3 cache misses (`LLC-load-misses` or `l2_cache_misses_from_dc_misses`) indicate pointer chasing (following pointers like `unique_ptr<Shape>`). Flattening shapes into contiguous vectors (`std::vector<Sphere>`) eliminates this.

