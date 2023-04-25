#include "elementwise_binary_operators.h"
#include <mdarray.h>
#include <naive.h>

#include <iostream>
#include <Eigen/Dense>
#include <boost/core/demangle.hpp>
#include <chrono>

using Eigen::ArrayXXd;
int main()
{
    const size_t n_loop = 200;
    const double v0 = 3;
    const size_t ROWS = 5000, COLS = 3000;
    std::cout << "Matrix dimensions (" << ROWS << ", " << COLS <<")\n";
    std::cout << "m1[2, 3] = 5, m2[2, 4] = 6, m3[2, 5] = 7, all other values are set to " << v0 <<"\n";
    std::cout << "calculating (m1 - m2*(m3 + m1))/m3, " << n_loop << " times\n";
    constexpr auto calc = [](const auto& m1, const auto& m2, const auto& m3){return (m1 - m2*(m3 + m1))/m3;};
    using Exts = stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent>;
    using Compile_Exts = stdex::extents<size_t, ROWS, COLS>;
    naive::MDArray<double, Exts> nm1{Exts(ROWS, COLS), v0}, nm2{Exts(ROWS, COLS), v0}, nm3{Exts(ROWS, COLS), v0};
#ifdef CLANG_COMPILER
    nm1(2, 3) = 5;
    nm2(2, 4) = 6;
    nm3(2, 5) = 7;
#else
    nm1[2, 3] = 5;
    nm2[2, 4] = 6;
    nm3[2, 5] = 7;
#endif

    // naive::matmul(nm1, nm2);

    MDArray<double, Exts> em1{Exts(ROWS, COLS), v0}, em2{Exts(ROWS, COLS), v0}, em3{Exts(ROWS, COLS), v0};
#ifdef CLANG_COMPILER
    em1(2, 3) = 5;
    em2(2, 4) = 6;
    em3(2, 5) = 7;
#else
    em1[2, 3] = 5;
    em2[2, 4] = 6;
    em3[2, 5] = 7;
#endif

    MDArray<double, Compile_Exts> cem1{Compile_Exts(), v0}, cem2{Compile_Exts(), v0}, cem3{Compile_Exts(), v0};
#ifdef CLANG_COMPILER
    cem1(2, 3) = 5;
    cem2(2, 4) = 6;
    cem3(2, 5) = 7;
#else
    cem1[2, 3] = 5;
    cem2[2, 4] = 6;
    cem3[2, 5] = 7;
#endif

    ArrayXXd eig1 = v0+ArrayXXd::Zero(ROWS, COLS), eig2 = v0+ArrayXXd::Zero(ROWS, COLS), eig3 = v0+ArrayXXd::Zero(ROWS, COLS);
    eig1(2, 3) = 5;
    eig2(2, 4) = 6;
    eig3(2, 5) = 7;


    long naive_t =
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    
    naive::MDArray<double, Exts> nsum{Exts(ROWS, COLS)};
    for(size_t i = 0; i < n_loop; i++){
        nsum = calc(nm1, nm2, nm3);
    }
    
    naive_t = 
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count() - naive_t;

    std::cout << "Dynamic size Expr\n";
    long expr_t =
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

    for(size_t i = 0; i < n_loop; i++){
        MDArray<double, Exts> esum = calc(em1, em2, em3);
    }

    expr_t = 
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count() - expr_t;

    std::cout << "Compile time fixed size Expr\n";
    long cexpr_t =
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

    for(size_t i = 0; i < n_loop; i++){
        MDArray<double, Compile_Exts> cesum = calc(cem1, cem2, cem3);
    }
    
    cexpr_t = 
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count() - cexpr_t;

    std::cout << "Eigen3\n";
    long eig_t =
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();

    for(size_t i = 0; i < n_loop; i++){
        ArrayXXd eigsum= calc(eig1, eig2, eig3);
    }
    
    eig_t = 
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count() - eig_t;

    auto eigsum_ = calc(eig1, eig2, eig3);
    auto esum_ = calc(em1, em2, em3);
    auto cesum_ = calc(cem1, cem2, cem3);

    std::cout << "Naive implementation took (an average of) " << static_cast<double>(naive_t)/n_loop << " milliseconds\n";
    std::cout << "Expression template implementation with dynamic size took (an average of) " << static_cast<double>(expr_t)/n_loop << " milliseconds\n";
    std::cout << "Expression template implementation with compile time fixed size took (an average of) " << static_cast<double>(cexpr_t)/n_loop << " milliseconds\n";
    std::cout << "Eigen implementation took (an average of) " << static_cast<double>(eig_t)/n_loop << " milliseconds\n";

    std::cout << "---->Dynamic size Expr speedup = t_naive/t_expr = " << static_cast<double>(naive_t) / 
        static_cast<double>(expr_t) << "\n";
    std::cout << "---->Compile time fixed size Expr speedup = t_naive/t_expr = " << static_cast<double>(naive_t) / 
        static_cast<double>(cexpr_t) << "\n";
    std::cout << "---->Eigen speedup = t_naive/t_eigen = " << static_cast<double>(naive_t) / 
        static_cast<double>(eig_t) << "\n";

    std::cout << "\nDynamic size Expr\'s result's type:\n";
    std::cout << boost::core::demangle( typeid(decltype(esum_)).name() ) << '\n';
    std::cout << "\nCompile time fixed size Expr\'s result's type:\n";
    std::cout << boost::core::demangle( typeid(decltype(cesum_)).name() ) << '\n';
    std::cout << "\nEigen\'s result's type:\n";
    std::cout << boost::core::demangle( typeid(decltype(eigsum_)).name() ) << '\n';


    return 0;
}

