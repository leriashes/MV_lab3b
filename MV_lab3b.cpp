#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
using namespace std;

extern "C" {
    void dgetrf_(int* m, int* n, double* a, int* lda, int* ipiv, int* info);
}

extern "C" {
    void dgetrs_(char* trans, int* n, int* nrhs, double* a, int* lda, int* ipiv, double* b, int* ldb, int* info);
}

//функция для чтения матрицы и вектора из файла
void read_file(int& n, vector<double>& matrix, vector<double>& vect) {
    ifstream f("input.txt");

    if (!f.is_open())
    {
        cerr << "Ошибка открытия файла" << endl;
        return;
    }

    f >> n;

    matrix.resize(n * n);
    vect.resize(n);

    for (int i = 0; i < n * n; ++i)
    {
        double num;
        f >> num;
        matrix[i] = double(num);
    }

    for (int i = 0; i < n; ++i)
    {
        double num;
        f >> num;
        vect[i] = double(num);
    }

    f.close();

    vector<double> transp(n * n);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            transp[j * n + i] = matrix[i * n + j];
        }
    }

    matrix = transp;
}


int main()
{
    setlocale(LC_ALL, "rus");

    int n, info;

    vector<double> A;      //Матрица
    vector<double> b;      //Вектор

    read_file(n, A, b);

    vector<double> A1 = A;
    vector<double> b1 = b;

    vector<int> ipiv(n);    //Массив индексов

    char trans = 'N';
    int nrhs = 1;


    auto start = chrono::high_resolution_clock::now();

    dgetrf_(&n, &n, A.data(), &n, ipiv.data(), &info);

    dgetrs_(&trans, &n, &nrhs, A.data(), &n, ipiv.data(), b.data(), &n, &info);

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end - start;


    cout << "Время выполнения: " << elapsed.count() << " секунд" << endl;

    cout << "Решение системы:" << endl;
    for (int i = 0; i < n; ++i) {
        cout << "x[" << i << "] = " << b[i] << endl;
    }

}
