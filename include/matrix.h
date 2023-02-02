#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <concepts>

#include <experimental/mdspan>
#include <expr_template.h>

namespace stdex = std::experimental;
using expr::BaseExpr;

template<typename T>
concept numeric = std::integral<T> || std::floating_point<T>;

template<numeric T, std::size_t ROWS, std::size_t COLS>
class Matrix;

template<numeric T, std::size_t ROWS, std::size_t COLS>
class Matrix : public BaseExpr<Matrix<T, ROWS, COLS>>{
    public:
        using value_type = T;

        Matrix() : m_values(ROWS*COLS) {}
        Matrix(const T val) : m_values(ROWS*COLS, val) {}

        template<expr::expression Expr>
        Matrix(const Expr& expr)
         : Matrix()
        {
            auto extents = expr.extents();
            for(std::size_t i = 0; i < extents.extent(0); i++){
                for(std::size_t j = 0; j < extents.extent(1); j++){
                    (*this)[i, j] = expr[i, j];
                }
            }
        }

        static constexpr std::size_t rows = ROWS;
        static constexpr std::size_t cols =  COLS;

        auto extents() const {return stdex::extents<size_t, 2, 2>();}

        T& operator[](std::size_t i, std::size_t j){return m_values[i*COLS + j];}
        T operator[](std::size_t i, std::size_t j) const {return m_values[i*COLS + j];}

    private:
        std::vector<T> m_values;
};

#endif // MATRIX_H
