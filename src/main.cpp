#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <cmath>

double calculate_integral_part(double start, double end, int steps) {
    double sum = 0.0;
    double step_size = (end - start) / steps;

    for (int i = 0; i < steps; ++i) {
        double x = start + i * step_size;
        sum += 4.0 / (1.0 + x * x);
    }

    return sum * step_size;
}

void parallel_integral(int num_threads, int num_steps) {
    std::vector<std::thread> threads;
    std::vector<double> results(num_threads);
    double step_size = 1.0 / num_steps;
    int steps_per_thread = num_steps / num_threads;

    auto calculate_part = [&](int thread_id) {
        double start = thread_id * steps_per_thread * step_size;
        double end = start + steps_per_thread * step_size;
        results[thread_id] = calculate_integral_part(start, end, steps_per_thread);
    };

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(calculate_part, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    double total_sum = 0.0;
    for (const auto& result : results) {
        total_sum += result;
    }

    std::cout << "Wynik: " << total_sum << std::endl;
}

int main() {
    int num_threads, num_steps;

    std::cout << "Podaj liczbe watkow: ";
    std::cin >> num_threads;

    std::cout << "Podaj liczbe krokow: ";
    std::cin >> num_steps;

    auto start_time = std::chrono::high_resolution_clock::now();
    parallel_integral(num_threads, num_steps);
    auto end_time = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end_time - start_time;
    std::cout << "Czas obliczen: " << duration.count() << " sekund" << std::endl;

    return 0;
}