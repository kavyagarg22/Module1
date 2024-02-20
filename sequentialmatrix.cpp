#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <random>

using namespace std;

// Function to print a matrix
void printMatrix(const vector<vector<double>>& matrix) {
    for (const auto& row : matrix) {
        for (double element : row) {
            cout << element << " ";
        }
        cout << endl;
    }
}

// Function to multiply two matrices
void multiplyMatrices(const vector<vector<double>>& A, const vector<vector<double>>& B, vector<vector<double>>& C) {
    int size = A.size();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            double sum = 0;
            for (int k = 0; k < size; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

int main() {
    int size = 1000;

    // Initialize matrices with random values using vectors
    vector<vector<double>> A(size, vector<double>(size));
    vector<vector<double>> B(size, vector<double>(size));
    vector<vector<double>> C(size, vector<double>(size));

    // Initialize random number generator
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(-10, 10);

    // Populate matrices with random values
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            A[i][j] = dis(gen);
            B[i][j] = dis(gen);
        }
    }

    // Multiply matrices and measure execution time
    auto start = chrono::high_resolution_clock::now();
    multiplyMatrices(A, B, C);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Execution time: " << duration.count() << " milliseconds" << endl;

    // Print matrices or write output to file if needed
    // printMatrix(C);

    return 0;
}
