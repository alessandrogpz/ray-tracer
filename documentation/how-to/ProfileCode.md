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

| Counter Name | Typical Value | What it represents |
| :--- | :--- | :--- |
| **`task-clock`** | `2.724,50 msec` | The total CPU time allocated to run your program (across all threads/cores). A high value indicates high CPU usage. |
| **`CPUs utilized`** | `13.4 CPUs` | Tells you how many cores your parallel threads fully occupied. If you run a 16-threaded OpenMP loop, this number should ideally approach `16.0`. |
| **`context-switches`** | `310` | Times the CPU suspended your program to run another task. Lower is better, as context switches cause latency. |
| **`cpu-migrations`** | `56` | Times the OS shifted a running thread from one physical CPU core to another. Keep low to avoid cache coldness. |
| **`page-faults`** | `3.392` | Times the OS had to assign a physical RAM page to your program (lazy allocation). Normal on first writes to large buffers. |
| **`cycles`** | `10.370.535.468` | Raw CPU clock cycles executed. Useful for raw processor workload measurement. |
| **`instructions`** | `13.161.633.277` | Total machine instructions executed. |
| **`insn per cycle` (IPC)** | `1.3` | **Instructions per Cycle.** Tells you how many instructions the CPU is completing on average per clock tick. Higher is better (ideal is 1.5–2.0+). Low IPC (<0.5) means CPU is stalling. |
| **`branch-misses`** | `6.633.258` | Times the CPU branch predictor guessed wrong on a branch (`if`, loops). |
| **`branch_miss_rate`** | `0.2%` | **Branch Miss Rate.** The percentage of wrong guesses. **Lower is better.** Below $1\%$ is excellent. High branch miss rates cause CPU pipeline stalls. |
| **`frontend_cycles_idle`**| `7%` | Percentage of clock cycles where the CPU instruction decoder was idling (waiting for memory or instructions). Lower is better. |

---

## 4. Key Takeaways for Ray Tracers

1. **Compilation Mode:** Always profile in **`Release`** mode. Profiling `Debug` mode will show you std::vector boundary checks, lack of inlining, and call stack overhead, hiding the actual mathematical bottlenecks.
2. **The C++ Module Math Gotcha:** If you see high self-time inside the standard library but low CPU performance, standard math functions (`std::sqrt`) imported through `import std;` might not be getting inlined as builtins. Restoring `#include <cmath>` in the global module fragment fixes this.
3. **Data-Oriented Design (DOD):** High L3 cache misses (`LLC-load-misses`) indicate pointer chasing (following pointers like `unique_ptr<Shape>`). Flattening shapes into contiguous vectors (`std::vector<Sphere>`) eliminates this.
