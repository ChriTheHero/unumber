//
// Created by Kirill Zhukov on 29.03.2025.
//

#ifndef BIGMATRIX_H
#define BIGMATRIX_H

#include <vector>
#include <type_traits>
#include "unumber/integer/BigInteger.h"
#include "unumber/decimal/BigDecimal.h"
#include "unumber/rational/BigRational.h"
#include "unumber/float/BigFloat.h"

namespace usub::mp
{
    template <typename T>
    class BigMatrix
    {
        static_assert(std::is_same_v<T, BigInteger> || std::is_same_v<T, BigRational> || std::is_same_v<T, BigFloat> || std::is_same_v<T, BigDecimal>,
                      "BigMatrix only supports BigInteger, BigRational, and BigFloat");

    public:
        BigMatrix(size_t r, size_t c);

        BigMatrix(const std::vector<std::vector<T>>& values);

        BigMatrix operator+(const BigMatrix& other) const;
        BigMatrix operator-(const BigMatrix& other) const;
        BigMatrix operator*(const BigMatrix& other) const;
        BigMatrix transpose() const;

        std::vector<T>& operator[](size_t index);

        const std::vector<T>& operator[](size_t index) const;

        template <typename U>
        friend std::ostream& operator<<(std::ostream& os, const BigMatrix<U>& matrix);
        template <typename U>
        friend std::istream& operator>>(std::istream& is, BigMatrix<U>& matrix);

    private:
        std::vector<std::vector<T>> data;
        size_t rows;
        size_t cols;
    };

    template <typename T>
    BigMatrix<T>::BigMatrix(size_t r, size_t c) : rows(r), cols(c), data(r, std::vector<T>(c))
    {
    }


    template <typename T>
    BigMatrix<T>::BigMatrix(const std::vector<std::vector<T>>& values) : data(values), rows(values.size()),
                                                                         cols(values.empty() ? 0 : values[0].size())
    {
    }

    template <typename T>
    BigMatrix<T> BigMatrix<T>::operator+(const BigMatrix& other) const
    {
        if (rows != other.rows || cols != other.cols) throw std::invalid_argument("Matrix dimensions must match");
        BigMatrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                result.data[i][j] = data[i][j] + other.data[i][j];
        return result;
    }

    template <typename T>
    BigMatrix<T> BigMatrix<T>::operator-(const BigMatrix& other) const
    {
        if (rows != other.rows || cols != other.cols) throw std::invalid_argument("Matrix dimensions must match");
        BigMatrix result(rows, cols);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                result.data[i][j] = data[i][j] - other.data[i][j];
        return result;
    }

    template <typename T>
    BigMatrix<T> BigMatrix<T>::operator*(const BigMatrix& other) const
    {
        if (cols != other.rows) throw std::invalid_argument("Invalid matrix dimensions for multiplication");
        BigMatrix result(rows, other.cols);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < other.cols; ++j)
                for (size_t k = 0; k < cols; ++k)
                    result.data[i][j] += data[i][k] * other.data[k][j];
        return result;
    }

    template <typename T>
    BigMatrix<T> BigMatrix<T>::transpose() const
    {
        BigMatrix result(cols, rows);
        for (size_t i = 0; i < rows; ++i)
            for (size_t j = 0; j < cols; ++j)
                result.data[j][i] = data[i][j];
        return result;
    }

    template <typename T>
    std::vector<T>& BigMatrix<T>::operator[](size_t index) { return data[index]; }

    template <typename T>
    const std::vector<T>& BigMatrix<T>::operator[](size_t index) const { return data[index]; }

    template <typename T>
    std::ostream& operator<<(std::ostream& os, const BigMatrix<T>& matrix)
    {
        for (const auto& row : matrix.data)
        {
            for (const auto& elem : row) os << elem << " ";
            os << "\n";
        }
        return os;
    }

    template <typename T>
    std::istream& operator>>(std::istream& is, BigMatrix<T>& matrix)
    {
        for (auto& row : matrix.data)
            for (auto& elem : row)
                is >> elem;
        return is;
    }
} // mp
// usub

#endif //BIGMATRIX_H
