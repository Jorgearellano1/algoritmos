#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <chrono>
#include <random>

int getDigit(int num, int pos) {
    return (num / static_cast<int>(pow(10, pos))) % 10;
}

int getMax(const std::vector<int>& arr) {
    return *std::max_element(arr.begin(), arr.end());
}

void countingSortByDigit(std::vector<int>& arr, int pos) {
    int n = arr.size();
    std::vector<int> output(n);
    std::vector<int> count(10, 0);

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

template <typename T>
void counting_sort(T *arr, int n) {
    if (n == 0)
        return;

    T max_val = arr[0];
    for (int i = 1; i < n; ++i) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }

    int *count = new int[max_val + 1];
    for (int i = 0; i <= max_val; ++i) {
        count[i] = 0;
    }

    for (int i = 0; i < n; ++i) {
        ++count[arr[i]];
    }

    int index = 0;
    for (int i = 0; i <= max_val; ++i) {
        for (int j = 0; j < count[i]; ++j) {
            arr[index++] = i;
        }
    }

    delete[] count;
}


template <typename T>
void selection_sort(T* arr, int n) {
    for (int i = 0; i < n-1; i++) {
        int min_idx = i;
        for (int j = i+1; j < n; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            std::swap(arr[i], arr[min_idx]);
        }
    }
}
int main() {
    srand(time(NULL));

    std::vector<int> n_values = {10, 100, 1000, 10000, 100000};
    std::vector<double> time_radix;
    std::vector<double> time_counting;
    std::vector<double> time_selection;

    for (int n : n_values) {
        std::vector<int> arr(n);
        int* counting_array = new int[n];
        int* selection_array = new int[n];

        for (int i = 0; i < n; i++) {
            int val = rand() % (n * 10);
            arr[i] = val;
            counting_array[i] = val;
            selection_array[i] = val;
        }

        auto start_radix = std::chrono::high_resolution_clock::now();
        radixSort(arr);
        auto end_radix = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_radix = end_radix - start_radix;
        time_radix.push_back(elapsed_radix.count());

        auto start_counting = std::chrono::high_resolution_clock::now();
        counting_sort(counting_array, n);
        auto end_counting = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_counting = end_counting - start_counting;
        time_counting.push_back(elapsed_counting.count());

        auto start_selection = std::chrono::high_resolution_clock::now();
        selection_sort(selection_array, n);
        auto end_selection = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_selection = end_selection - start_selection;
        time_selection.push_back(elapsed_selection.count());

        delete[] counting_array;
        delete[] selection_array;
    }

    std::ofstream dataFile("data.txt");
    for (size_t i = 0; i < n_values.size(); i++) {
        dataFile << n_values[i] << " " << time_radix[i] << " " << time_counting[i] << " " << time_selection[i] << std::endl;
    }
    dataFile.close();

    FILE* pipe = popen("gnuplot -persist", "w");
    if (pipe) {
        fprintf(pipe, "set xlabel 'Number of Random Integers'\n");
        fprintf(pipe, "set ylabel 'Time (s)'\n");
        fprintf(pipe, "set logscale y\n");

        fprintf(pipe, "set title 'Comparison of Sorting Algorithms: Radix Sort vs Counting Sort vs Selection Sort'\n");
        fprintf(pipe, "plot 'data.txt' using 1:2 with lines title 'Radix Sort', 'data.txt' using 1:3 with lines title 'Counting Sort', 'data.txt' using 1:4 with lines title 'Selection Sort'\n");
        fflush(pipe);
        pclose(pipe);
    }

    return 0;
}