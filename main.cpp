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

int* Random_Array(int size) {
    int* array = new int[size];
    for (int i = 0; i < size; i++) {
        array[i] = rand() % 10;
    }
    return array;
}

void CountingSort(int* &array, int size) {
    int* temp_cont = new int[10];
    int* sorted = new int[size];

    for (int i = 0; i < 10; i++) {
        temp_cont[i] = 0;
        for (int j = 0; j < size; j++) {
            if (i == array[j]) {
                temp_cont[i]++;
            }
        }
    }

    for (int i = 1; i < 10; i++) {
        temp_cont[i] += temp_cont[i-1];
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < 10; j++) {
            if (j == array[i]) {
                sorted[temp_cont[j]-1] = array[i];
                temp_cont[j]--;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        array[i] = sorted[i];
    }

    delete[] temp_cont;
    delete[] sorted;
}

int main() {
    srand(time(NULL));

    std::vector<int> n_values = {10, 100, 1000, 10000, 100000, 1000000, 10000000};
    std::vector<double> time_radix;
    std::vector<double> time_counting;

    for (int n : n_values) {
        std::vector<int> arr(n);
        int* array = Random_Array(n);
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % (n * 10);
        }

        auto start_radix = std::chrono::high_resolution_clock::now();
        radixSort(arr);
        auto end_radix = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_radix = end_radix - start_radix;
        time_radix.push_back(elapsed_radix.count());

        auto start_counting = std::chrono::high_resolution_clock::now();
        CountingSort(array, n);
        auto end_counting = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_counting = end_counting - start_counting;
        time_counting.push_back(elapsed_counting.count());

        delete[] array;
    }

    std::ofstream dataFile("data.txt");
    for (size_t i = 0; i < n_values.size(); i++) {
        dataFile << n_values[i] << " " << time_radix[i] << " " << time_counting[i] << std::endl;
    }
    dataFile.close();

    FILE* pipe = popen("gnuplot -persist", "w");
    if (pipe) {
        fprintf(pipe, "set xlabel 'Number of Random Integers'\n");
        fprintf(pipe, "set xlabel 'Number of Random Integers'\n");
        fprintf(pipe, "set ylabel 'Tiempo (s)'\n");
        fprintf(pipe, "set title 'Comparación de tiempos: Radix Sort vs Counting Sort'\n");
        fprintf(pipe, "plot 'data.txt' using 1:2 with lines title 'Radix Sort', 'data.txt' using 1:3 with lines title 'Counting Sort'\n");
        fflush(pipe);
        pclose(pipe);
    }

    return 0;
}
