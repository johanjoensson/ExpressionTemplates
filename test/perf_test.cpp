#include <mdarray.h>
#include <naive.h>

#include <Eigen/Dense>
#include <boost/core/demangle.hpp>
#include <chrono>

using Eigen::ArrayXXd;
int main()
{
    const size_t ROWS = 30000, COLS = 10000;
    using Exts = stdex::extents<size_t, std::dynamic_extent, std::dynamic_extent>;
    naive::MDArray<double, Exts> nm1{Exts(ROWS, COLS), 0}, nm2{Exts(ROWS, COLS), 0}, nm3{Exts(ROWS, COLS), 0};
    nm1[2, 3] = 5;
    nm2[2, 4] = 6;
    nm3[2, 5] = 7;

    MDArray<double, Exts> em1{Exts(ROWS, COLS), 0}, em2{Exts(ROWS, COLS), 0}, em3{Exts(ROWS, COLS), 0};
    em1[2, 3] = 5;
    em2[2, 4] = 6;
    em3[2, 5] = 7;

    ArrayXXd eig1 = ArrayXXd::Zero(ROWS, COLS), eig2 = ArrayXXd::Zero(ROWS, COLS), eig3 = ArrayXXd::Zero(ROWS, COLS);
    eig1(2, 3) = 5;
    eig2(2, 4) = 6;
    eig3(2, 5) = 7;


    const size_t n_loop = 100;
    long naive_t =
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    for(size_t i = 0; i < n_loop; i++){
        naive::MDArray<double, Exts> nsum = nm1 + nm2*(nm3 + nm1);
    }
    naive_t = 
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count() - naive_t;

    std::cout << "Expr\n";
    long expr_t =
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    for(size_t i = 0; i < n_loop; i++){
        MDArray<double, Exts> esum = em1 + em2*(em3 + em1);
    }
    expr_t = 
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count() - expr_t;

    std::cout << "Eigen3\n";
    long eig_t =
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count();
    for(size_t i = 0; i < n_loop; i++){
        ArrayXXd eigsum = eig1 + eig2*(eig3 + eig1);
    }
    eig_t = 
        std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now().time_since_epoch()).count() - eig_t;

    auto eigsum_ = eig1 + eig2*(eig3 + eig1);
    auto esum_ = em1 + em2*(em3 + em1);

    std::cout << "Naive implementation took (an average of) " << static_cast<double>(naive_t)/n_loop << " milliseconds\n";
    std::cout << "Expression template implementation took (an average of) " << static_cast<double>(expr_t)/n_loop << " milliseconds\n";
    std::cout << "Eigen implementation took (an average of) " << static_cast<double>(eig_t)/n_loop << " milliseconds\n";

    std::cout << "---->Expr speedup = t_naive/t_expr = " << static_cast<double>(naive_t) / 
        static_cast<double>(expr_t) << "\n";
    std::cout << "---->Eigen speedup = t_naive/t_eigen = " << static_cast<double>(naive_t) / 
        static_cast<double>(eig_t) << "\n";

    std::cout << "\nExpr\'s result's type:\n";
    std::cout << boost::core::demangle( typeid(decltype(esum_)).name() ) << '\n';
    std::cout << "\nEigen\'s result's type:\n";
    std::cout << boost::core::demangle( typeid(decltype(eigsum_)).name() ) << '\n';

    return 0;
}

