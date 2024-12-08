#include <iostream>
#include <vector>
#include <map>
#include <iomanip>
#include <cstdlib> // ��� ������� std::abs

// ����� ��� �������� ������ �������
class MatrixDiagonal {
private:
    std::vector<std::vector<int>> data; // ��������� ������ ��� �������� ��������� �������
    size_t rows, cols;                   // ���������� ����� � ��������

public:
    // �����������
    MatrixDiagonal(size_t r, size_t c) : rows(r), cols(c), data(r, std::vector<int>(c, 0)) {}

    // ��������� �������� ��������
    void SetValue(size_t row, size_t col, int value) {
        if (row < rows && col < cols) {
            data[row][col] = value;
        }
    }

    // ��������� �������� ��������
    int GetValue(size_t row, size_t col) const {
        return (row < rows && col < cols) ? data[row][col] : 0;
    }

    // ����� ������� �� �����
    void Print() const {
        for (const auto& row : data) {
            for (int value : row) {
                std::cout << std::setw(4) << value << " ";
            }
            std::cout << std::endl;
        }
    }

    // ��������� ���������� ����� � ��������
    size_t GetRowsNum() const { return rows; }
    size_t GetColsNum() const { return cols; }
};

// ����� ��� �������� ���������� ������������� ������� (������ ������������ ��������)
class MatrixFunc {
private:
    size_t rows, cols;                             // ������� �������
    std::map<int, std::vector<int>> diagonals;     // ��������� ��� ��������� ����������

public:
    // �����������
    MatrixFunc(size_t r, size_t c) : rows(r), cols(c) {}

    // ��������� �������� �������� (������ ��� ����������)
    void SetDiagonalValue(int diagonal, size_t index, int value) {
        if (diagonal >= -static_cast<int>(rows) + 1 && diagonal <= static_cast<int>(cols) - 1) {
            diagonals[diagonal].resize(std::max(diagonals[diagonal].size(), index + 1), 0);
            diagonals[diagonal][index] = value;
        }
    }

    // ��������� �������� ��������
    int GetValue(size_t row, size_t col) const {
        int diagonal = static_cast<int>(col) - static_cast<int>(row);
        if (diagonals.find(diagonal) != diagonals.end()) {
            size_t index = (diagonal >= 0) ? row : col; // ���������� ������ � ������� ���������
            return diagonals.at(diagonal)[index];
        }
        return 0;
    }

    // ����� ������� �� �����
    void Print() const {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << std::setw(4) << GetValue(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    // �������������� ������ ������� � ��������� ������
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

// ������� ��� ������������ ������ �������
int main() {
    MatrixDiagonal fullMatrix(5, 5);
    
    // ������������� ������ �������
    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 5; ++j) {
            fullMatrix.SetValue(i, j, (i == j) ? i + 1 : 0); // ��������� ������������ ��������
        }
    }

    std::cout << "������ �������:" << std::endl;
    fullMatrix.Print();

    MatrixFunc sparseMatrix(5, 5);
    sparseMatrix.FromMatrix(fullMatrix);

    std::cout << "\n��������� ������������� �������:" << std::endl;
    sparseMatrix.Print();

    return 0;
}