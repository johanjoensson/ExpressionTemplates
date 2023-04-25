#include <mdarray.h>
#include <gtest/gtest.h>
TEST(MDArray, TestAccess)
{
    using D2 = stdex::dextents<std::size_t, 2>;
     MDArray<int, D2> m(D2(2, 2));

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

TEST(MDArray, TestMDspanConvert)
{
    using D2 = stdex::dextents<std::size_t, 2>;
    MDArray<int, D2> m(D2(2, 3));

    m[0, 0] = 1;
    m[0, 1] = 2;
    m[0, 2] = 3;
    m[1, 0] = 4;
    m[1, 1] = 5;
    m[1, 2] = 6;

    stdex::mdspan<int, D2> ms(m);

    ASSERT_EQ(decltype(ms)::rank(), 2);
    ASSERT_EQ(ms.extent(0), 2);
    ASSERT_EQ(ms.extent(1), 3);
    ASSERT_EQ(ms.size(), 6);

}

TEST(MDArray, TestMDspanAdd)
{
    using D2 = stdex::dextents<std::size_t, 2>;
    MDArray<int, D2> m1(D2(2, 2)), m2(D2(2, 2));

    m1[0, 0] = 1;
    m1[0, 1] = 2;
    m1[1, 0] = 3;
    m1[1, 1] = 4;
    m2[0, 0] = 4;
    m2[0, 1] = 3;
    m2[1, 0] = 2;
    m2[1, 1] = 1;

    MDArray<int, D2> sum = m1 + m2;
    for(size_t i = 0; i < sum.extents().extent(0); i++){
        for(size_t j = 0; j < sum.extents().extent(1); j++){
            int val = sum[i, j];
            ASSERT_EQ(val, 5);
        }
    }

}

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

TEST(MDArray, TestAddMul)
{
    using D2 = stdex::dextents<std::size_t, 2>;
    MDArray<int, D2> m1(D2(2, 2)), m2(D2(2, 2)), m3(D2(2, 2));
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

    MDArray<int, D2> res = m1 + m2*m3;
    for(size_t i = 0; i < res.extents().extent(0); i++){
        for(size_t j = 0; j < res.extents().extent(1); j++){
            int val = res[i, j], exact = v1<int>(i, j) + v2<int>(i, j)*v1<int>(i, j);
            ASSERT_EQ(val, exact);
        }
    }
}
