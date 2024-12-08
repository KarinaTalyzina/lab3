#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <memory> // ��� std::unique_ptr
#include <stdexcept> // ��� std::runtime_error

// ������� ����������� ����� �������
template <typename T>
class MatrixBase {
public:
    virtual ~MatrixBase() = default;

    virtual size_t GetRowsNum() const = 0; // �������� ���������� �����
    virtual size_t GetColsNum() const = 0; // �������� ���������� ��������
    virtual T GetValue(size_t row, size_t col) const = 0; // �������� �������� ��������
    virtual void SetValue(size_t row, size_t col, T value) = 0; // ���������� �������� ��������
    virtual void Print() const = 0; // ������ ������� �� �����
    virtual void SaveToFile(const std::string &filename) const = 0; // ���������� ������� � ����
    virtual std::unique_ptr<MatrixBase<T>> operator+(const MatrixBase<T>& other) const = 0; // �������� ������
    virtual std::unique_ptr<MatrixBase<T>> operator-(const MatrixBase<T>& other) const = 0; // ��������� ������
    virtual std::unique_ptr<MatrixBase<T>> operator*(const MatrixBase<T>& other) const = 0; // ��������� ������
    virtual std::unique_ptr<MatrixBase<T>> ElementWiseMultiply(const MatrixBase<T>& other) const = 0; // ���������� ��������� ������
};

// ������� (Dense) �������
template <typename T>
class MatrixDense : public MatrixBase<T> {
private:
    std::vector<std::vector<T>> data; // ��������� ������ ��� �������� ������ �������
    size_t rows, cols; // ���������� ����� � ��������

public:
    // �����������, ���������������� ������� ��������� ������� ������
    MatrixDense(size_t r, size_t c) : rows(r), cols(c), data(r, std::vector<T>(c, 0)) {}

    size_t GetRowsNum() const override { return rows; }
    size_t GetColsNum() const override { return cols; }
    
    T GetValue(size_t row, size_t col) const override { 
        return data[row][col]; 
    }
    
    void SetValue(size_t row, size_t col, T value) override { 
        data[row][col] = value; 
    }

    // �������� �������� ������
    std::unique_ptr<MatrixBase<T>> operator+(const MatrixBase<T>& other) const override {
        auto result = std::make_unique<MatrixDense>(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result->SetValue(i, j, GetValue(i, j) + other.GetValue(i, j));
            }
        }
        return result;
    }

    // �������� ��������� ������
    std::unique_ptr<MatrixBase<T>> operator-(const MatrixBase<T>& other) const override {
        auto result = std::make_unique<MatrixDense>(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result->SetValue(i, j, GetValue(i, j) - other.GetValue(i, j));
            }
        }
        return result;
    }

    // �������� ��������� ������
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

    // ���������� ��������� ������
    std::unique_ptr<MatrixBase<T>> ElementWiseMultiply(const MatrixBase<T>& other) const override {
        auto result = std::make_unique<MatrixDense>(rows, cols);
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                result->SetValue(i, j, GetValue(i, j) * other.GetValue(i, j));
            }
        }
        return result;
    }

    // ������ ������� �� �����
        void Print() const override {
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                std::cout << std::setw(4) << GetValue(i, j) << " ";
            }
            std::cout << std::endl;
        }
    }

    // ���������� ������� � ����
    void SaveToFile(const std::string &filename) const override {
        std::ofstream outFile(filename);
        if (!outFile.is_open()) {
            throw std::runtime_error("�� ������� ������� ���� ��� ������.");
        }
        outFile << rows << " " << cols << "\n";
        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                outFile << GetValue(i, j) << " ";
            }
            outFile << "\n";
        }
    }

    // ����� �������� ������� �� �����
    void LoadFromFile(const std::string &filename) {
        std::ifstream inFile(filename);
        if (!inFile.is_open()) {
            throw std::runtime_error("�� ������� ������� ���� ��� ������.");
        }

        inFile >> rows >> cols; // ������ ���������� ����� � �������� �� �����
        data.resize(rows, std::vector<T>(cols)); // �������� ������ ������� ��� �������� ������

        for (size_t i = 0; i < rows; ++i) {
            for (size_t j = 0; j < cols; ++j) {
                inFile >> data[i][j]; // ������ �������� ��������� �� �����
            }
        }
    }
};
// ������ ������������� �������
int main() {
    try {
        MatrixDense<int> mat1(2, 3); // ������� ������� �������� 2x3
        mat1.SetValue(0, 0, 1);
        mat1.SetValue(0, 1, 2);
        mat1.SetValue(0, 2, 3);
        mat1.SetValue(1, 0, 4);
        mat1.SetValue(1, 1, 5);
        mat1.SetValue(1, 2, 6);

        std::cout << "������� 1:\n";
        mat1.Print();

        MatrixDense<int> mat2(2, 3); // ������� ������ ������� �������� 2x3
        mat2.SetValue(0, 0, 7);
        mat2.SetValue(0, 1, 8);
        mat2.SetValue(0, 2, 9);
        mat2.SetValue(1, 0, 10);
        mat2.SetValue(1, 1, 11);
        mat2.SetValue(1, 2, 12);

        std::cout << "\n������� 2:\n";
        mat2.Print();

        auto resultAdd = mat1 + mat2; // �������� ������
        std::cout << "\n��������� ��������:\n";
        resultAdd->Print(); // �������������� ��������� ��� ������ ������ Print

    } catch (const std::exception &e) {
        std::cerr << "������: " << e.what() << std::endl;
    }

    return 0;
}
