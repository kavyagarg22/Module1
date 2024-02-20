#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>
#include <random>

using namespace std;
using namespace std::chrono;

const int N = 1000;
const int BLOCK_SIZE = 50;

int A[N][N], B[N][N], C[N][N];
vector<thread> thread_pool;

void init_matrix(int mat[][N]) {
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<int> dist(1, 10);
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      mat[i][j] = dist(gen);
    }
  }
}

void matrix_multiply(int block_start_row, int block_end_row) {
  for (int i = block_start_row; i < block_end_row; ++i) {
    for (int j = 0; j < N; ++j) {
      C[i][j] = 0;
      for (int k = 0; k < N; ++k) {
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

int main() {
  init_matrix(A);
  init_matrix(B);

  auto start_time = high_resolution_clock::now();

  const int max_threads = thread::hardware_concurrency();
  const int NUM_THREADS = min((N * N) / (BLOCK_SIZE * BLOCK_SIZE), max_threads);

  for (int i = 0; i < NUM_THREADS; ++i) {
    int block_start_row = (i / (N / BLOCK_SIZE)) * BLOCK_SIZE;
    int block_end_row = block_start_row + BLOCK_SIZE;
    thread_pool.emplace_back(matrix_multiply, block_start_row, block_end_row);
  }

  for (auto& thread : thread_pool) {
    thread.join();
  }

  auto end_time = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(end_time - start_time);

  ofstream fout("output.txt");
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      fout << C[i][j] << " ";
    }
    fout << "\n";
  }
  fout.close();

  cout << "Execution time: " << duration.count() << " microseconds" << endl;

  return 0;
}
