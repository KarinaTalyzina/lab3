#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <cstdlib> // Для функции std::abs

// Класс для хранения полной матрицы
class MatrixDiagonal {
private:
    std::vector<std::vector<int>> data; // Двумерный массив для хранения элементов матрицы
    size_t rows, cols;                   // Количество строк и столбцов

public:
    // Конструктор
    MatrixDiagonal(size_t r, size_t c) : rows(r), cols(c), data(r, std::vector<int>(c, 0)) {}

    // Установка значения элемента
    void SetValue(size_t row, size_t col, int value) {
        if (row < rows && col < cols) {
            data[row][col] = value;
        }
    }

    // Получение значения элемента
    int GetValue(size_t row, size_t col) const {
        return (row < rows && col < cols) ? data[row][col] : 0;
    }

    // Вывод матрицы на экран
    void Print() const {
        for (const auto& row : data) {
            for (int value : row) {
                std::cout << std::setw(4) << value << " ";
            }
            std::cout << std::endl;
        }
    }

    // Получение количества строк и столбцов
    size_t GetRowsNum() const { return rows; }
    size_t GetColsNum() const { return cols; }
};

// Класс для хранения экономного представления матрицы (только диагональные элементы)
class MatrixFunc {
private:
    size_t rows, cols;                             // Размеры матрицы
    std::map<int, std::vector<int>> diagonals;     // Хранилище для ненулевых диагоналей

public:
    // Конструктор
    MatrixFunc(size_t r, size_t c) : rows(r), cols(c) {}

    // Установка значения элемента (только для диагоналей)
    void SetDiagonalValue(int diagonal, size_t index, int value) {
        if (diagonal >= -static_cast<int>(rows) + 1 && diagonal <= static_cast<int>(cols) - 1) {
            diagonals[diagonal].resize(std::max(diagonals[diagonal].size(), index + 1), 0);
            diagonals[diagonal][index] = value;
        }
    }

    // Получение значения элемента
    int GetValue(size_t row, size_t col) const {
        int diagonal = static_cast<int>(col) - static_cast<int>(row);
        if (diagonals.find(diagonal) != diagonals.end()) {
            size_t index = (diagonal >= 0) ? row : col; // Определяем индекс в массиве диагонали
            return diagonals.at(diagonal)[index];
        }
        return 0;
    }

    // Вывод матрицы на экран
    void Print() const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << std::setw(4) << GetValue(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    // Преобразование полной матрицы в экономный формат
    void FromMatrix(const MatrixDiagonal &matrix) {
        rows = matrix.GetRowsNum();
        cols = matrix.GetColsNum();
        diagonals.clear();
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                int value = matrix.GetValue(i, j);
                if (value != 0) {
                    int diagonal = static_cast<int>(j) - static_cast<int>(i);
                    SetDiagonalValue(diagonal, (diagonal >= 0) ? i : j, value);
                }
            }
        }
    }
};

// Функция для тестирования работы классов
int main() {
    MatrixDiagonal fullMatrix(5, 5);
    
    // Инициализация полной матрицы
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            fullMatrix.SetValue(i, j, (i == j) ? i + 1 : 0); // Заполняем диагональные элементы
        }
    }

    std::cout << "Полная матрица:" << std::endl;
    fullMatrix.Print();

    MatrixFunc sparseMatrix(5, 5);
    sparseMatrix.FromMatrix(fullMatrix);

    std::cout << "\nЭкономное представление матрицы:" << std::endl;
    sparseMatrix.Print();

    return 0;
}