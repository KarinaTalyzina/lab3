#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <memory> // Для std::unique_ptr
#include <stdexcept> // Для std::runtime_error

// Базовый абстрактный класс матрицы
template <typename T>
class MatrixBase {
public:
    virtual ~MatrixBase() = default;

    virtual size_t GetRowsNum() const = 0; // Получить количество строк
    virtual size_t GetColsNum() const = 0; // Получить количество столбцов
    virtual T GetValue(size_t row, size_t col) const = 0; // Получить значение элемента
    virtual void SetValue(size_t row, size_t col, T value) = 0; // Установить значение элемента
    virtual void Print() const = 0; // Печать матрицы на экран
    virtual void SaveToFile(const std::string &filename) const = 0; // Сохранение матрицы в файл
    virtual std::unique_ptr<MatrixBase<T>> operator+(const MatrixBase<T>& other) const = 0; // Сложение матриц
    virtual std::unique_ptr<MatrixBase<T>> operator-(const MatrixBase<T>& other) const = 0; // Вычитание матриц
    virtual std::unique_ptr<MatrixBase<T>> operator*(const MatrixBase<T>& other) const = 0; // Умножение матриц
    virtual std::unique_ptr<MatrixBase<T>> ElementWiseMultiply(const MatrixBase<T>& other) const = 0; // Элементное умножение матриц
};

// Плотная (Dense) матрица
template <typename T>
class MatrixDense : public MatrixBase<T> {
private:
    std::vector<std::vector<T>> data; // Двумерный вектор для хранения данных матрицы
    size_t rows, cols; // Количество строк и столбцов

public:
    // Конструктор, инициализирующий матрицу заданного размера нулями
    MatrixDense(size_t r, size_t c) : rows(r), cols(c), data(r, std::vector<T>(c, 0)) {}

    size_t GetRowsNum() const override { return rows; }
    size_t GetColsNum() const override { return cols; }
    
    T GetValue(size_t row, size_t col) const override { 
        return data[row][col]; 
    }
    
    void SetValue(size_t row, size_t col, T value) override { 
        data[row][col] = value; 
    }

    // Операция сложения матриц
    std::unique_ptr<MatrixBase<T>> operator+(const MatrixBase<T>& other) const override {
        auto result = std::make_unique<MatrixDense>(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result->SetValue(i, j, GetValue(i, j) + other.GetValue(i, j));
            }
        }
        return result;
    }

    // Операция вычитания матриц
    std::unique_ptr<MatrixBase<T>> operator-(const MatrixBase<T>& other) const override {
        auto result = std::make_unique<MatrixDense>(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result->SetValue(i, j, GetValue(i, j) - other.GetValue(i, j));
            }
        }
        return result;
    }

    // Операция умножения матриц
    std::unique_ptr<MatrixBase<T>> operator*(const MatrixBase<T>& other) const override {
        auto result = std::make_unique<MatrixDense>(rows, other.GetColsNum());
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < other.GetColsNum(); ++j) {
                T sum = 0;
                for (size_t k = 0; k < cols; ++k) {
                    sum += GetValue(i, k) * other.GetValue(k, j);
                }
                result->SetValue(i, j, sum);
            }
        }
        return result;
    }

    // Элементное умножение матриц
    std::unique_ptr<MatrixBase<T>> ElementWiseMultiply(const MatrixBase<T>& other) const override {
        auto result = std::make_unique<MatrixDense>(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result->SetValue(i, j, GetValue(i, j) * other.GetValue(i, j));
            }
        }
        return result;
    }

    // Печать матрицы на экран
        void Print() const override {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << std::setw(4) << GetValue(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    // Сохранение матрицы в файл
    void SaveToFile(const std::string &filename) const override {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            throw std::runtime_error("Не удалось открыть файл для записи.");
        }
        outFile << rows << " " << cols << "\n";
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                outFile << GetValue(i, j) << " ";
            }
            outFile << "\n";
        }
    }

    // Метод загрузки матрицы из файла
    void LoadFromFile(const std::string &filename) {
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            throw std::runtime_error("Не удалось открыть файл для чтения.");
        }

        inFile >> rows >> cols; // Читаем количество строк и столбцов из файла
        data.resize(rows, std::vector<T>(cols)); // Изменяем размер вектора для хранения данных

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                inFile >> data[i][j]; // Читаем значения элементов из файла
            }
        }
    }
};
// Пример использования классов
int main() {
    try {
        MatrixDense<int> mat1(2, 3); // Создаем матрицу размером 2x3
        mat1.SetValue(0, 0, 1);
        mat1.SetValue(0, 1, 2);
        mat1.SetValue(0, 2, 3);
        mat1.SetValue(1, 0, 4);
        mat1.SetValue(1, 1, 5);
        mat1.SetValue(1, 2, 6);

        std::cout << "Матрица 1:\n";
        mat1.Print();

        MatrixDense<int> mat2(2, 3); // Создаем вторую матрицу размером 2x3
        mat2.SetValue(0, 0, 7);
        mat2.SetValue(0, 1, 8);
        mat2.SetValue(0, 2, 9);
        mat2.SetValue(1, 0, 10);
        mat2.SetValue(1, 1, 11);
        mat2.SetValue(1, 2, 12);

        std::cout << "\nМатрица 2:\n";
        mat2.Print();

        auto resultAdd = mat1 + mat2; // Сложение матриц
        std::cout << "\nРезультат сложения:\n";
        resultAdd->Print(); // Разыменовываем указатель для вызова метода Print

    } catch (const std::exception &e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
    }

    return 0;
}
