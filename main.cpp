#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <chrono>



int getDigit(int num, int pos) {
    return (num / static_cast<int>(pow(10, pos))) % 10;
}

int getMax(const std::vector<int>& arr) {
    return *std::max_element(arr.begin(), arr.end());
}

void countingSortByDigit(std::vector<int>& arr, int pos) {
    int n = arr.size();
    std::vector<int> output(n);
    std::vector<int> count(10, 0); // 10 dígitos posibles (0-9)

    for (int i = 0; i < n; i++) {
        count[getDigit(arr[i], pos)]++;
    }

    for (int i = 1; i < 10; i++) {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; i--) {
        output[count[getDigit(arr[i], pos)] - 1] = arr[i];
        count[getDigit(arr[i], pos)]--;
    }

    for (int i = 0; i < n; i++) {
        arr[i] = output[i];
    }
}

void radixSort(std::vector<int>& arr) {
    int maxNum = getMax(arr);
    int digits = log10(maxNum) + 1;

    for (int pos = 0; pos < digits; pos++) {
        countingSortByDigit(arr, pos);
    }
}

int main() {
    std::vector<int> n_values = {10, 100, 1000, 10000, 100000, 1000000, 10000000};
    std::vector<double> time_complexity;

    for (int n : n_values) {
        std::vector<int> arr(n);
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % (n * 10);
        }

        auto start = std::chrono::high_resolution_clock::now();
        radixSort(arr);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end - start;

        time_complexity.push_back(elapsed.count());
    }

    std::ofstream dataFile("data.txt");
    for (size_t i = 0; i < n_values.size(); i++) {
        dataFile << n_values[i] << " " << time_complexity[i] << std::endl;
    }
    dataFile.close();


    FILE* pipe = popen("gnuplot -persist", "w");
    if (pipe) {
        fprintf(pipe, "set xlabel 'Number of Random Integers'\n");  // Cambiado el nombre del eje x
        fprintf(pipe, "set ylabel 'Tiempo (s)'\n");
        fprintf(pipe, "set title 'Complejidad en tiempo de Radix Sort'\n");
        fprintf(pipe, "plot 'data.txt' using 1:2 with lines title 'Tiempo'\n");
        fflush(pipe);
        pclose(pipe);
    }

    return 0;
}