#ifndef EXPR_TEMPLATE_MATRIX_MULTIPLICATION_OPERATOR_H
#define EXPR_TEMPLATE_MATRIX_MULTIPLICATION_OPERATOR_H

#include<base_expression.h>
#include <bits/utility.h>
#include <functional>
#include<iostream>
#include<exception>

namespace expr
{

template<typename IndexType, size_t... LHS_EXTENTS, size_t... RHS_EXTENTS, 
         size_t LHS_EXT_I, size_t LHS_EXT_J, size_t... LHS_EXTS, size_t RHS_EXT_I, size_t RHS_EXT_J, size_t... RHS_EXTS,
         size_t EXT_I, size_t EXT_J, size_t...EXTS, size_t... EXTENTS, typename... DYNAMIC_EXTENTS>
constexpr auto build_matmul_extents(stdex::extents<IndexType,LHS_EXTENTS...> lhs_exts, 
                                    stdex::extents<IndexType,RHS_EXTENTS...> rhs_exts, 
                                    std::integer_sequence<size_t, LHS_EXT_I, LHS_EXT_J, LHS_EXTS...>, 
                                    std::integer_sequence<size_t, RHS_EXT_I, RHS_EXT_J, RHS_EXTS...>, 
                                    std::index_sequence<EXT_I, EXT_J, EXTS...>, 
                                    stdex::extents<IndexType, EXTENTS...>, 
                                    DYNAMIC_EXTENTS... dynamic_extents)
{       
        if constexpr(sizeof...(EXTS) == 0){
                if constexpr(LHS_EXT_I != std::dynamic_extent && RHS_EXT_J != std::dynamic_extent){
                        return stdex::extents<IndexType, EXTENTS..., LHS_EXT_I, RHS_EXT_J>{ dynamic_extents...};
                }else if constexpr(LHS_EXT_I == std::dynamic_extent && RHS_EXT_J != std::dynamic_extent){
                        return stdex::extents<IndexType, EXTENTS..., std::dynamic_extent, RHS_EXT_J>{ dynamic_extents..., lhs_exts.extent(EXT_I) };
                }else if constexpr(LHS_EXT_I != std::dynamic_extent && RHS_EXT_J == std::dynamic_extent){
                        return stdex::extents<IndexType, EXTENTS..., LHS_EXT_I, std::dynamic_extent>{ dynamic_extents..., rhs_exts.extent(EXT_J) };
                }else{
                        return stdex::extents<IndexType, EXTENTS..., std::dynamic_extent, std::dynamic_extent>{ dynamic_extents..., lhs_exts.extent(EXT_I), rhs_exts.extent(EXT_J) };
                }
        }else{
                if constexpr(LHS_EXT_I != std::dynamic_extent){
                        return build_matmul_extents(lhs_exts, rhs_exts, std::integer_sequence<size_t, LHS_EXT_J, LHS_EXTS...>{}, std::integer_sequence<size_t, RHS_EXT_J, RHS_EXTS...>{}, std::index_sequence<EXT_J, EXTS...>{}, stdex::extents<IndexType, EXTENTS..., LHS_EXT_I>{dynamic_extents...}, dynamic_extents...);
                }else if constexpr(RHS_EXT_I != std::dynamic_extent){
                        return build_matmul_extents(lhs_exts, rhs_exts, std::integer_sequence<size_t, LHS_EXT_J, LHS_EXTS...>{}, std::integer_sequence<size_t, RHS_EXT_J, RHS_EXTS...>{}, std::index_sequence<EXT_J, EXTS...>{}, stdex::extents<IndexType, EXTENTS..., RHS_EXT_I>{dynamic_extents...}, dynamic_extents...);
                }else{
                        return build_matmul_extents(lhs_exts, rhs_exts, std::integer_sequence<size_t, LHS_EXT_J, LHS_EXTS...>{}, std::integer_sequence<size_t, RHS_EXT_J, RHS_EXTS...>{}, std::index_sequence<EXT_J, EXTS...>{}, stdex::extents<IndexType, EXTENTS..., std::dynamic_extent> { dynamic_extents..., lhs_exts.extent(EXT_I)}, dynamic_extents..., lhs_exts.extent(EXT_I));
                }
        }
}
template<typename IndexType, size_t... LHS_EXTENTS, size_t... RHS_EXTENTS>
constexpr auto build_matmul_extents(stdex::extents<IndexType,LHS_EXTENTS...> lhs_exts, stdex::extents<IndexType,RHS_EXTENTS...> rhs_exts)
{
        return build_matmul_extents(lhs_exts, rhs_exts, std::integer_sequence<size_t, LHS_EXTENTS...>{}, std::integer_sequence<size_t, RHS_EXTENTS...>{}, std::make_index_sequence<sizeof...(RHS_EXTENTS)>{}, stdex::extents<IndexType>{});
}


template<expression LHS, expression RHS>
using element_type = decltype(std::declval<typename std::remove_reference_t<LHS>::value_type>() * std::declval<typename std::remove_reference_t<RHS>::value_type>());

/***************************************************************************//**
* ElementwiseBinaryOp represents expressions where a function is
* applied to mathcing elements in two expressions. The evaluation of the
* function is not performed until the specific element of this expression is
* required (via the subscript operator).
 ******************************************************************************/
template<expression LHS, expression RHS, typename EXT>
class MatrixMultiplicationOp: public BaseExpr<MatrixMultiplicationOp<LHS, RHS, EXT>>{
    public:
        using Base = BaseExpr<MatrixMultiplicationOp<LHS, RHS, EXT>>;
        using RHS_noref = std::remove_reference_t<RHS>;
        using LHS_noref = std::remove_reference_t<LHS>;
        using value_type = element_type<LHS, RHS>;

        constexpr explicit MatrixMultiplicationOp(LHS&& lhs, RHS&& rhs, EXT&& ext) noexcept
         : Base(), m_lhs(std::forward<LHS>(lhs)), m_rhs(std::forward<RHS>(rhs)), m_ext(std::forward<EXT>(ext))
        {}

        ~MatrixMultiplicationOp() noexcept = default;

        constexpr auto extents() const noexcept {return m_ext.extents();};
        constexpr auto extent(std::size_t i) const noexcept {return m_ext.extent(i);};

#ifdef CLANGBUG
        constexpr auto operator()(auto&&  ... indices, auto&& i, auto&& j) const
        {
                value_type res = 0;
                for (auto k = 0; k < m_lhs.extent(sizeof...(indices) + 1); k++){
                        res += m_lhs(indices..., i, k) * m_rhs(indices..., k, j);
                }
                return res;
        }
#endif
        constexpr auto operator[](auto&&... indices) const
        {
                if constexpr(sizeof...(indices) == 2){
                        return get_value(indices...);
                }else{
                        return get_value(std::make_index_sequence<sizeof...(indices) - 2>{}, std::tuple{indices...});
                }
        }

        constexpr explicit MatrixMultiplicationOp(const MatrixMultiplicationOp&) noexcept = default;
        constexpr explicit MatrixMultiplicationOp(MatrixMultiplicationOp&&) noexcept = default;

        constexpr MatrixMultiplicationOp& operator=(const MatrixMultiplicationOp&) noexcept = default;
        constexpr MatrixMultiplicationOp& operator=(MatrixMultiplicationOp&&) noexcept = default;
    private:
        std::remove_cvref_t<LHS> m_lhs;
        std::remove_cvref_t<RHS> m_rhs;
        EXT m_ext;

        constexpr explicit MatrixMultiplicationOp() noexcept = default;

        template<typename... Indices, size_t... Idxs>
        constexpr auto get_value(std::index_sequence<Idxs...>&&, std::tuple<Indices...>&& indices) const
        {
                value_type res = 0;
                auto i = std::get<sizeof...(Indices) - 2>(indices);
                auto j = std::get<sizeof...(Indices) - 1>(indices);
                for (decltype(m_lhs.extent(0)) k = 0; k < m_lhs.extent(sizeof...(Indices) - 1); k++){
                        res+= m_lhs[std::get<Idxs>(indices)..., i, k] * m_rhs[std::get<Idxs>(indices)..., k, j];
                }
                return res;
        }
        constexpr auto get_value(auto&& i, auto&& j) const
        {
                value_type res = 0;
                for (decltype(m_lhs.extent(1)) k = 0; k < m_lhs.extent(1); k++){
                        res+= m_lhs[i, k] * m_rhs[k, j];
                }
                return res;
        }
}; // MatrixMultiplicationOP

template<expression LHS, expression RHS>
constexpr inline auto matmul(LHS&& lhs, RHS&& rhs)
{
        if (lhs.extents().rank() != rhs.extents().rank()){
                throw std::runtime_error("Rank of left hand side expression does not match rank of right hand side expression!\n" + std::to_string(lhs.extents().rank()) + " != " + std::to_string(rhs.extents().rank()));
        }
        for (size_t i = 0; i < lhs.extents().rank() - 2; i++){
                if (lhs.extent(i) != rhs.extent(i)){
                        throw std::runtime_error("Dimensions do not match!\nDimension " + std::to_string(i) + ": " + std::to_string(lhs.extent(i)) + " != " 
                                        + std::to_string(rhs.extent(i)));
                }
        }
        if (lhs.extent(lhs.extents().rank() - 1) != rhs.extent(lhs.extents().rank() - 2)){
                throw std::runtime_error("incompatible dimensions for matrix multiplication!\n" + std::to_string(lhs.extent(lhs.extents().rank() - 1)) + " != " + std::to_string(rhs.extent(rhs.extents().rank() - 2)));
        }
        auto matmul_exts = build_matmul_extents(lhs.extents(), rhs.extents());
        return MatrixMultiplicationOp<LHS, RHS, decltype(matmul_exts)>{std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::move(matmul_exts)};
}


}; //expr

#endif // EXPR_TEMPLATE_ELEMENTWISE_BINARY_OPERATOR_H
