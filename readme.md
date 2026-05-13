# High Performance FIR Filter Pipeline (Linux)

## 📌 Overview

This project implements a **high-performance streaming FIR filter pipeline** in C++ using:

* AVX2 SIMD intrinsics
* Cache-friendly circular buffering
* Integer-based convolution
* Nanosecond-level benchmarking

The application supports:

* CSV input mode
* Random data generation mode
* FIR filtering + thresholding
* Performance measurement in nanoseconds

The implementation is optimized for **very low latency processing** and demonstrates CPU-level optimization techniques such as SIMD vectorization, loop unrolling, and cache-aware memory access.

---

# ⚙️ Features

* 🚀 AVX2 SIMD accelerated FIR filtering
* 📂 CSV input support
* 🎲 Random input generation
* 📊 Nanosecond timing benchmark
* 🧠 Cache-friendly memory layout
* ⚡ Optimized integer arithmetic
* 🔬 Real-time style streaming pipeline

---

# 📁 Project Structure

```text id="pwv3i0"
project/
│
├── main.cpp
├── input.csv
└── README.md
```

---

# 🧾 FIR Filter Details

The system performs a **9-point FIR convolution** using the following coefficients:

```text id="28h5b8"
25
866
7802
24130
34375
24130
7802
866
12
```

Each incoming value is filtered using the previous and future neighboring samples.

---

# 💻 Requirements

## Linux Requirements

* Ubuntu / Debian / Arch / Fedora
* g++ compiler with AVX2 support
* CPU supporting AVX2 instructions

---

# 🔍 Check AVX2 Support

Run:

```bash id="1kixb0"
lscpu | grep AVX
```

If output contains:

```text id="16gdr9"
avx avx2
```

then AVX2 is supported.

---

# 🛠️ Build Instructions

Open terminal in the project folder.

Compile using:

```bash id="krorjj"
g++ main.cpp -O3 -march=native -mavx2 -flto -funroll-loops -o app
```

---

# 🧠 Compiler Flags Explained

| Flag             | Description                   |
| ---------------- | ----------------------------- |
| `-O3`            | Maximum optimization          |
| `-march=native`  | Use CPU-specific instructions |
| `-mavx2`         | Enable AVX2 SIMD              |
| `-flto`          | Link-time optimization        |
| `-funroll-loops` | Unroll loops for speed        |
| `-o app`         | Output executable name        |

---

# ▶️ Running the Program

Run:

```bash id="n6ql0z"
./app
```

---

# 📂 CSV Input Mode

The program reads data from:

```text id="ygjlwm"
input.csv
```

Example format:

```text id="6k9yfx"
12,45,67,23,89
34,56,78,90,12
11,22,33,44,55
```

---

# 🎲 Random Input Mode

Inside `main.cpp`:

```cpp id="xvty08"
bool useCSV = false;
```

This enables random number generation instead of CSV input.

---

# 📊 Example Output

```text id="ob3i0n"
Reading CSV...

===== PERFORMANCE =====
Samples: 999991
Total Time: 64000000 ns
Avg: 64 ns
Sink: 12345
```

---

# ⚡ Performance Notes

The implementation uses:

* SIMD vectorization (`_mm256_mullo_epi32`)
* Cache-local circular buffer
* Integer arithmetic instead of floating point
* Manual reduction using AVX2 intrinsics

Typical performance:

| Mode         | Avg Time  |
| ------------ | --------- |
| Random Input | ~40–70 ns |
| CSV Input    | ~60–90 ns |

Performance depends on:

* CPU architecture
* Cache size
* Compiler version
* Background system load

---

# 🧠 Optimization Techniques Used

## SIMD (AVX2)

Processes 8 integers simultaneously:

```cpp id="8imkbi"
_mm256_mullo_epi32()
```

---

## Cache Optimization

Uses mirrored circular buffer:

```cpp id="v79lm0"
window[idx] = v;
window[idx + 16] = v;
```

Avoids expensive modulo operations.

---

## Integer Arithmetic

Avoids floating-point overhead for lower latency.

---

## Loop Optimization

Compiler optimizations:

* loop unrolling
* instruction scheduling
* vectorization

---

# ⚠️ Troubleshooting

---

## ❌ `immintrin.h` not found

Install build tools:

### Ubuntu/Debian

```bash id="nsvvfk"
sudo apt update
sudo apt install build-essential
```

---

## ❌ Illegal instruction

CPU may not support AVX2.

Compile without AVX2:

```bash id="tyfpzt"
g++ main.cpp -O3 -o app
```

---

## ❌ CSV file not found

Ensure:

```text id="jlwmv1"
input.csv
```

exists in the same folder as executable.

---

# 📈 Benchmarking Tips

For stable measurements:

Run on a fixed CPU core:

```bash id="3vphcu"
taskset -c 0 ./app
```

Close unnecessary applications for cleaner benchmarks.

---

# 🧠 Technical Highlights

* High-throughput FIR processing
* SIMD-accelerated convolution
* Nanosecond-level benchmarking
* Low-latency streaming architecture
* Cache-aware implementation

---
