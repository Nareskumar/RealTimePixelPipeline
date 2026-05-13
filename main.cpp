#include <immintrin.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>

using namespace std;
using namespace std::chrono;

// ================= CONFIG =================
constexpr int N_DEFAULT = 1'000'000;

// FIR coefficients
alignas(32) const int coeffs[8] = {
    25, 866, 7802, 24130,
    34375, 24130, 7802, 866
};

constexpr int f8 = 12;
constexpr int THRESHOLD = 100 * 100000;

// prevent optimization removal
volatile int sink = 0;

// ================= CSV READER =================
vector<int> readCSV(const string& filename) {
    vector<int> data;
    ifstream file(filename);

    if (!file.is_open()) {
        cout << "ERROR: Cannot open CSV file!\n";
        return data;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string val;

        while (getline(ss, val, ',')) {
            if (!val.empty())
                data.push_back(stoi(val));
        }
    }
    return data;
}

// ================= RANDOM GENERATOR =================
vector<int> generateRandom(int N) {
    vector<int> data(N);
    for (int i = 0; i < N; i++)
        data[i] = rand() & 255;   // fast bounded random
    return data;
}

// ================= FIR CORE =================
void runFIR(const vector<int>& data) {

    int N = data.size();

    alignas(32) int window[32] = { 0 };

    int idx = 0;
    int count = 0;
    long long total = 0;

    // warm-up
    for (int i = 0; i < min(10000, N); i++)
        sink += data[i];

    auto start = high_resolution_clock::now();

    for (int i = 0; i < N; i++) {
        int v = data[i];

        // mirror buffer
        window[idx] = v;
        window[idx + 16] = v;

        idx++;
        if (idx == 16) idx = 0;

        if (count < 9) {
            count++;
            continue;
        }

        int* p = &window[idx];

        __m256i d = _mm256_load_si256((__m256i*)p);
        __m256i c = _mm256_load_si256((__m256i*)coeffs);

        __m256i mul = _mm256_mullo_epi32(d, c);

        __m128i low = _mm256_castsi256_si128(mul);
        __m128i high = _mm256_extracti128_si256(mul, 1);

        __m128i sum128 = _mm_add_epi32(low, high);
        sum128 = _mm_hadd_epi32(sum128, sum128);
        sum128 = _mm_hadd_epi32(sum128, sum128);

        int sum = _mm_cvtsi128_si32(sum128);

        sum += p[8] * f8;

        if (sum >= THRESHOLD)
            sink++;

        total++;
    }

    auto end = high_resolution_clock::now();

    long long time_ns =
        duration_cast<nanoseconds>(end - start).count();

    double avg = (double)time_ns / total;

    cout << "\n===== PERFORMANCE =====\n";
    cout << "Samples: " << total << endl;
    cout << "Total Time: " << time_ns << " ns\n";
    cout << "Avg: " << avg << " ns\n";
    cout << "Sink: " << sink << endl;
}

// ================= MAIN =================
int main() {

    bool useCSV = true;   // 🔁 CHANGE THIS

    vector<int> data;

    if (useCSV) {
        cout << "Reading CSV...\n";
        data = readCSV("input.csv");
    }
    else {
        cout << "Generating random data...\n";
        data = generateRandom(N_DEFAULT);
    }

    if (data.empty()) {
        cout << "No data available!\n";
        return 0;
    }

    runFIR(data);

    return 0;
}