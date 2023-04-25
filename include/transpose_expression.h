#ifndef EXPR_TEMPLATE_TRANSPOSE_EXPRESSION_H
#define EXPR_TEMPLATE_TRANSPOSE_EXPRESSION_H

#include<base_expression.h>
#include <bits/utility.h>
#include <functional>
#include<iostream>
#include<exception>

namespace expr
{

/***************************************************************************//**
* ElementwiseBinaryOp represents expressions where a function is
* applied to mathcing elements in two expressions. The evaluation of the
* function is not performed until the specific element of this expression is
* required (via the subscript operator).
 ******************************************************************************/
template<expression LHS, typename EXT, size_t... Order>
class TransposeExpressionOp: public BaseExpr<TransposeExpressionOp<LHS, EXT, Order...>>{
    public:
        using Base = BaseExpr<TransposeExpressionOp<LHS, EXT, Order...>>;
        using LHS_noref = std::remove_reference_t<LHS>;
        using value_type = std::remove_reference_t<LHS>::value_type;

        constexpr explicit TransposeExpressionOp(LHS&& lhs, EXT&& exts ) noexcept
         : Base(), m_expr(std::forward<LHS>(lhs)),
                m_ext(std::forward<EXT>(exts))
        {}

        ~TransposeExpressionOp() noexcept = default;

        constexpr auto extents() const noexcept {return m_ext.extents();};
        constexpr auto extent(std::size_t i) const noexcept {return m_ext.extent(i);};

        constexpr auto operator[](auto&&... indices) const
        {
                return get_value(indices...);
        }

        constexpr explicit TransposeExpressionOp(const TransposeExpressionOp&) noexcept = default;
        constexpr explicit TransposeExpressionOp(TransposeExpressionOp&&) noexcept = default;

        constexpr TransposeExpressionOp& operator=(const TransposeExpressionOp&) noexcept = default;
        constexpr TransposeExpressionOp& operator=(TransposeExpressionOp&&) noexcept = default;
    private:
        std::remove_cv_t<LHS> m_expr;
        EXT m_ext;


        auto get_value(auto&&... indices) const
        {
                return m_expr[indices...];
        }

}; // TransposeExpressionOp

template<size_t T, size_t... Ts, size_t... Reversed>
constexpr auto reverse_sequence(std::index_sequence<T, Ts...>, std::index_sequence<Reversed...>)
{
        if constexpr(sizeof...(Ts) == 0){
                return std::index_sequence<T, Reversed...>{};
        }else{
                return reverse_sequence(std::index_sequence<Ts...>{}, std::index_sequence<T, Reversed...>{});
        }
}

template<size_t N, typename Type, Type Target, Type T, Type... Ts>
constexpr auto find_in_sequence(std::integer_sequence<Type, T, Ts...>)
{
        if constexpr(Target == T){
                return N;
        }else if constexpr(sizeof...(Ts) > 0){
                return find_in_sequence<N + 1, Type, Target, Ts... >(std::integer_sequence<Type, Ts...>{});
        }else{
                return N + 1; 
        }
}


template<typename IndexType, size_t... Extents, typename... Order, size_t Idx>
constexpr auto reorder_extents(stdex::extents<IndexType, Extents...> exts, std::index_sequence<Idx>, std::tuple<Order...> order, auto... reordered_extents)
{
        return stdex::extents<IndexType, reordered_extents..., exts.extent(std::get<Idx>(order))>{};
}

template<size_t N, typename Type, Type T, Type... Ts>
constexpr auto get_from_sequence(std::index_sequence<T, Ts...>)
{
        if constexpr(N == 0){
                return T;
        }else{
                return get_from_sequence<N-1>(std::index_sequence<Ts...>{});
        }
}
template<typename IndexType, size_t... Extents, size_t Idx, size_t... ReorderedExtents, typename... DynamicExtents>
constexpr auto reorder_extents(stdex::extents<IndexType, Extents...> extents, std::index_sequence<Idx>, stdex::extents<IndexType, ReorderedExtents...>, DynamicExtents... dynamic_extents)
{
        constexpr auto ext = get_from_sequence<Idx>(std::index_sequence<Extents...>{});
        if constexpr( ext == std::dynamic_extent){
                return stdex::extents<IndexType, ReorderedExtents..., ext>{ dynamic_extents..., extents.extent(Idx)};
        }else{
                return stdex::extents<IndexType, ReorderedExtents..., ext>{ dynamic_extents...};
        }
}

template<typename IndexType, size_t... Extents, size_t Idx, size_t... Idxs, size_t... ReorderedExtents, typename... DynamicExtents>
constexpr auto reorder_extents(stdex::extents<IndexType, Extents...> extents, std::index_sequence<Idx, Idxs...>, stdex::extents<IndexType, ReorderedExtents...>, DynamicExtents... dynamic_extents)
{
        constexpr auto ext = get_from_sequence<Idx>(std::index_sequence<Extents...>{});
        if constexpr( ext == std::dynamic_extent){
                return reorder_extents(extents, std::index_sequence<Idxs...>{}, stdex::extents<IndexType, ReorderedExtents..., ext>{dynamic_extents..., extents.extent(Idx)}, dynamic_extents..., extents.extent(Idx));
        }else{
                return reorder_extents(extents, std::index_sequence<Idxs...>{}, stdex::extents<IndexType, ReorderedExtents..., ext>{dynamic_extents...}, dynamic_extents...);
        }
}

template<typename IndexType, size_t... Extents, size_t... Order>
constexpr auto reorder_extents(stdex::extents<IndexType, Extents...> exts, std::index_sequence<Order...>)
{
        return reorder_extents(exts, std::index_sequence<Order...>{}, stdex::extents<IndexType>{});
}

template<typename IndexType, size_t... Extents>
constexpr auto reverse_extents(stdex::extents<IndexType, Extents...>)
{
        return reverse_sequence(std::index_sequence<sizeof...(Extents)>{});
}

template<expression LHS, size_t... Order>
constexpr inline auto transpose(LHS&& lhs, std::index_sequence<Order...>)
{
        if (lhs.extents().rank() != sizeof...(Order)){
                throw std::runtime_error("Rank of expression and dimensions of new ordering do not match!\n" + std::to_string(lhs.extents().rank()) + " != " + std::to_string(sizeof...(Order)));
        }
        auto reordered_exts = reorder_extents(lhs.extents(), std::make_index_sequence<sizeof...(Order)>{});
        return TransposeExpressionOp<LHS, decltype(reordered_exts), Order...>{std::forward<LHS>(lhs), std::move(reordered_exts)};
}

template<expression LHS>
constexpr inline auto transpose(LHS&& lhs)
{
        return transpose(std::forward<LHS>(lhs), reverse_extents(lhs.extents()));
}


}; //expr

#endif // EXPR_TEMPLATE_TRANSPOSE_EXPRESSION_H
