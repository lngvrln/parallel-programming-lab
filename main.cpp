#include <iostream>
#include <vector>
#include <atomic>
#include <thread>
#include <random>
#include <chrono>
#include <algorithm>
using namespace std;

#ifndef THREADS
#define THREADS 4
#endif

const size_t N = 10'000'000;
const size_t BLOCK = 100'000;
const int THRESHOLD = 999'990;

size_t find_serial(const vector<int>& a) {
    for (size_t i = 0; i < a.size(); i++)
        if (a[i] > THRESHOLD) return i;
    return a.size();
}

#ifdef PARALLEL
atomic<size_t> next_block{0};
atomic<size_t> first_idx;

void worker(const vector<int>& a) {
    while (true) {
        size_t start = next_block.fetch_add(BLOCK);
        if (start >= a.size() || start >= first_idx.load()) break;
        size_t end = min(start + BLOCK, a.size());
        for (size_t i = start; i < end; i++) {
            if (a[i] > THRESHOLD) {
                size_t cur = first_idx.load();
                while (i < cur && !first_idx.compare_exchange_weak(cur, i)) {}
                break;
            }
        }
    }
}

size_t find_parallel(const vector<int>& a) {
    next_block = 0;
    first_idx = a.size();
    vector<thread> threads;
    for (int t = 0; t < THREADS; t++)
        threads.emplace_back(worker, cref(a));
    for (auto& th : threads) th.join();
    return first_idx.load();
}
#endif

int main() {
    cout << "Номер: 7" << endl;
    cout << "Задача 4: первое число, превышающее " << THRESHOLD << endl;
    cout << "Параллелизация 2: atomic (общий счётчик), тип: int" << endl;

    mt19937 gen(42);
    uniform_int_distribution<int> dist(0, 1'000'000);
    vector<int> a(N);
    for (auto& x : a) x = dist(gen);

    auto t0 = chrono::steady_clock::now();
#ifdef PARALLEL
    cout << "Режим: parallel, потоков: " << THREADS << endl;
    size_t idx = find_parallel(a);
#else
    cout << "Режим: serial" << endl;
    size_t idx = find_serial(a);
#endif
    auto t1 = chrono::steady_clock::now();

    if (idx == a.size()) cout << "Не найдено" << endl;
    else cout << "Индекс: " << idx << ", значение: " << a[idx] << endl;
    cout << "Время, мс: " << chrono::duration<double, milli>(t1 - t0).count() << endl;
}
