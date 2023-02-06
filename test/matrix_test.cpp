#include <matrix.h>
// #include <expr_template.h>
#include <gtest/gtest.h>
// #include <type_traits>

template<typename T>
T v1(const size_t i, const size_t j)
{
    T i_t = static_cast<T>(i);
    T j_t = static_cast<T>(j);
    return 2*i_t + j_t + 1;
}

template<typename T>
T v2(const size_t i, const size_t j)
{
    T i_t = static_cast<T>(i);
    T j_t = static_cast<T>(j);
    return 2*(1-i_t) + (2 - j_t);
}

TEST(Matrix, TestAccess)
{
    Matrix<int, 2, 2> m;
    m[0, 0] = 1;
    m[0, 1] = 2;
    m[1, 0] = 3;
    m[1, 1] = 4;

    int val = m[0, 0];
    ASSERT_EQ(val, 1);
    val = m[0, 1];
    ASSERT_EQ(val, 2);
    val = m[1, 0];
    ASSERT_EQ(val, 3);
    val = m[1, 1];
    ASSERT_EQ(val, 4);
}

TEST(Matrix, TestNegate)
{
    Matrix<float, 2, 2> m;
    m[0, 0] = 1;
    m[0, 1] = 2;
    m[1, 0] = 3;
    m[1, 1] = 4;

    auto n = -m;
    // m1[i, j] = 2*i + j + 1
    // m1[0, 0] = 1, m[0, 1] = 2
    // m1[1, 0] = 3, m1[1, 1] = 4
    for(std::size_t i = 0; i < m.rows; i++){
        for(std::size_t j = 0; j < m.cols; j++){
            double val = n[i, j];
            ASSERT_DOUBLE_EQ(val, -v1<int>(i, j));
        }
    }
}

TEST(Matrix, TestScale)
{
    Matrix<int, 2, 2> m;
    m[0, 0] = 1;
    m[0, 1] = 2;
    m[1, 0] = 3;
    m[1, 1] = 4;

    auto n = 2*m;
    // m1[i, j] = 2*i + j + 1
    // m1[0, 0] = 1, m[0, 1] = 2
    // m1[1, 0] = 3, m1[1, 1] = 4
    for(size_t i = 0; i < m.rows; i++){
        for(size_t j = 0; j < m.cols; j++){
            int val = n[i, j];
            EXPECT_EQ(val, 2*v1<int>(i, j));
        }
    }
}

TEST(Matrix, TestScale2)
{
    Matrix<int, 2, 2> m;
    m[0, 0] = 1;
    m[0, 1] = 2;
    m[1, 0] = 3;
    m[1, 1] = 4;

    auto n = m*2;
    // m1[i, j] = 2*i + j + 1
    // m1[0, 0] = 1, m[0, 1] = 2
    // m1[1, 0] = 3, m1[1, 1] = 4
    for(size_t i = 0; i < m.rows; i++){
        for(size_t j = 0; j < m.cols; j++){
            int val = n[i, j];
            ASSERT_EQ(val, 2*v1<int>(i, j));
        }
    }
}

TEST(Matrix, TestScale3)
{
    Matrix<int, 2, 2> m;
    m[0, 0] = 1;
    m[0, 1] = 2;
    m[1, 0] = 3;
    m[1, 1] = 4;


    auto n = m/2;
    // m[i, j] = 2*i + j + 1
    // m[0, 0] = 1, m[0, 1] = 2
    // m[1, 0] = 3, m[1, 1] = 4
    for(size_t i = 0; i < m.rows; i++){
        for(size_t j = 0; j < m.cols; j++){
            int val = n[i, j];
            EXPECT_EQ(val, v1<int>(i, j)/2);
        }
    }
}

TEST(Matrix, TestScaleRecipr)
{
    Matrix<double, 2, 2> m;
    m[0, 0] = 1.;
    m[0, 1] = 2.;
    m[1, 0] = 3.;
    m[1, 1] = 4.;

    auto n = 2./m;
    // m1[i, j] = 2*i + j + 1
    // m1[0, 0] = 1, m[0, 1] = 2
    // m1[1, 0] = 3, m1[1, 1] = 4
    for(size_t i = 0; i < m.rows; i++){
        for(size_t j = 0; j < m.cols; j++){
            double val = n[i, j];
            ASSERT_DOUBLE_EQ(val, 2./v1<double>(i, j));
        }
    }
}

TEST(Matrix, TestAdd)
{
    Matrix<int, 2, 2> m1, m2, m3;
    m1[0, 0] = 1;
    m1[0, 1] = 2;
    m1[1, 0] = 3;
    m1[1, 1] = 4;
    m2[0, 0] = 4;
    m2[0, 1] = 3;
    m2[1, 0] = 2;
    m2[1, 1] = 1;

    auto sum = m1 + m2;
    for(size_t i = 0; i < m1.rows; i++){
        for(size_t j = 0; j < m1.cols; j++){
            m3[i, j] = sum[i, j];
        }
    }
    for(size_t i = 0; i < m3.rows; i++){
        for(size_t j = 0; j < m3.cols; j++){
            int val = m3[i, j];
            ASSERT_EQ(val, 5);
        }
    }
}

TEST(Matrix, TestAddConvert)
{

    Matrix<int, 2, 2> m1, m2;
    m1[0, 0] = 1;
    m1[0, 1] = 2;
    m1[1, 0] = 3;
    m1[1, 1] = 4;
    m2[0, 0] = 4;
    m2[0, 1] = 3;
    m2[1, 0] = 2;
    m2[1, 1] = 1;

    Matrix<int, 2, 2> sum = m1 + m2;
    for(size_t i = 0; i < sum.rows; i++){
        for(size_t j = 0; j < sum.cols; j++){
            int val = sum[i, j];
            ASSERT_EQ(val, 5);
        }
    }
}

TEST(Matrix, TestSubConvert)
{
    Matrix<int, 2, 2> m1, m2;
    m1[0, 0] = 1;
    m1[0, 1] = 2;
    m1[1, 0] = 3;
    m1[1, 1] = 4;
    m2[0, 0] = 4;
    m2[0, 1] = 3;
    m2[1, 0] = 2;
    m2[1, 1] = 1;

    // m1[i, j] = 2*i + j + 1
    // m1[0, 0] = 1, m[0, 1] = 2
    // m1[1, 0] = 3, m1[1, 1] = 4
    //
    // m2[i, j] = 2*(1-i) + (2 - j)
    // m2[0, 0] = 4, m2[0, 1] = 3
    // m2[1, 0] = 2, m2[1, 1] = 1
    //
    // diff[i, j] = 2*i + j + 1 - (2*(1-i) + (2 - j)) = 
    //            = 2*i + j + 1 - 2 + 2*i - 2 + j =
    //            = 4*i + 2*j - 3
    Matrix<int, 2, 2> diff = m1 - m2;
    for(size_t i = 0; i < diff.rows; i++){
        for(size_t j = 0; j < diff.cols; j++){
            int val = diff[i, j];
            ASSERT_EQ(val, v1<int>(i, j) - v2<int>(i, j));
        }
    }
}

TEST(Matrix, TestMulConvert)
{
    Matrix<int, 2, 2> m1, m2;
    m1[0, 0] = 1;
    m1[0, 1] = 2;
    m1[1, 0] = 3;
    m1[1, 1] = 4;
    m2[0, 0] = 4;
    m2[0, 1] = 3;
    m2[1, 0] = 2;
    m2[1, 1] = 1;

    // m1[i, j] = 2*i + j + 1
    // m1[0, 0] = 1, m[0, 1] = 2
    // m1[1, 0] = 3, m1[1, 1] = 4
    //
    // m2[i, j] = 2*(1-i) + (2 - j)
    // m2[0, 0] = 4, m2[0, 1] = 3
    // m2[1, 0] = 2, m2[1, 1] = 1
    Matrix<int, 2, 2> prod = m1 * m2;
    for(size_t i = 0; i < prod.rows; i++){
        for(size_t j = 0; j < prod.cols; j++){
            int val = prod[i, j];
            ASSERT_EQ(val, v1<int>(i, j) * v2<int>(i, j));
        }
    }
}

TEST(Matrix, TestDivConvert)
{

    Matrix<int, 2, 2> m1, m2;
    m1[0, 0] = 1;
    m1[0, 1] = 2;
    m1[1, 0] = 3;
    m1[1, 1] = 4;
    m2[0, 0] = 4;
    m2[0, 1] = 3;
    m2[1, 0] = 2;
    m2[1, 1] = 1;

    Matrix<int, 2, 2> frac = m1 / m2;
    for(size_t i = 0; i < frac.rows; i++){
        for(size_t j = 0; j < frac.cols; j++){
            int val = frac[i, j];
            ASSERT_EQ(val, v1<int>(i, j) / v2<int>(i, j));
        }
    }
}

TEST(Matrix, TestAddMul)
{

    Matrix<int, 2, 2> m1, m2, m3;
    m1[0, 0] = 1;
    m1[0, 1] = 2;
    m1[1, 0] = 3;
    m1[1, 1] = 4;
    
    m2[0, 0] = 4;
    m2[0, 1] = 3;
    m2[1, 0] = 2;
    m2[1, 1] = 1;

    m3[0, 0] = 1;
    m3[0, 1] = 2;
    m3[1, 0] = 3;
    m3[1, 1] = 4;

    Matrix<int, 2, 2> res = m1 + m2*m3;
    for(size_t i = 0; i < res.rows; i++){
        for(size_t j = 0; j < res.cols; j++){
            int val = res[i, j], exact = v1<int>(i, j) + v2<int>(i, j)*v1<int>(i, j);
            ASSERT_EQ(val, exact);
        }
    }
}

TEST(Matrix, Cast)
{
    auto caster = [](const int val){return static_cast<double>(val);};
    Matrix<int, 2, 2> m1;
    m1[0, 0] = 1;
    m1[0, 1] = 2;
    m1[1, 0] = 3;
    m1[1, 1] = 4;
    Matrix<double, 2, 2> m2 = expr::map(m1, caster);
    for(size_t i = 0; i < m2.rows; i++){
        for(size_t j = 0; j < m2.cols; j++){
            auto val1 = m1[i, j];
            auto val2 = m2[i, j];
            ASSERT_DOUBLE_EQ(static_cast<double>(val1), val2);
        }
    }
}
