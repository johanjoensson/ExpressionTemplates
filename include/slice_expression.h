#ifndef EXPR_TEMPLATE_SLICE_EXPRESSION_H
#define EXPR_TEMPLATE_SLICE_EXPRESSION_H

#include <base_expression.h>
#include <bits/utility.h>
#include <tuple>
namespace expr{


namespace detail{
template <typename T> 
int sgn(T val) {
        return (T(0) < val) - (val < T(0));
}
}; // detail
template<typename IndexType>
struct SliceRange
{
        using StepType = typename std::make_signed<IndexType>::type;
        SliceRange(IndexType start, IndexType stop, IndexType step)
                : m_start(start), m_stop(stop), m_step(step)
        {}
        SliceRange(IndexType start, IndexType stop)
                : m_start(start), m_stop(stop), m_step(detail::sgn(static_cast<StepType>(stop - start)))
        {}
        IndexType m_start;
        IndexType m_stop;
        StepType m_step;
};
struct all
{};
/*******************************************************************************
 * m ~ (1, 2, 3, 4, 5)
 * s = slice(m, 0, 0, all, all, {1, 5, 2}) ~ (3, 4, 2)
 * s[2, 3, 1] = m[0, 0, 2, 3, 1 + 1*2] = m[0, 0, 2, 3, 3]
 *
 * store offset, (o1, o2, o3, o4, o5) and strides(s1, s2, s3, s4, s5)
 ******************************************************************************/
template<expression Expr, typename OriginalExtents, typename SliceExtents, typename Offsets, typename Steps>
class Slice;

template<expression Expr, typename IndexType, size_t... OriginalExtents, size_t... SliceExtents, typename... Offsets, typename... Steps>
#ifndef DEDUCE_THIS
class Slice<Expr,  stdex::extents<IndexType, OriginalExtents...>, stdex::extents<IndexType, SliceExtents...>, Pack<Offsets...>, Pack<Steps...>> : public BaseExpr<Slice<Expr, stdex::extents<IndexType, OriginalExtents...>, stdex::extents<IndexType, SliceExtents...>, Pack<Offsets...>, Pack<Steps...>>>
#else
class Slice : public BaseExpr
#endif
{
        public:

                constexpr explicit Slice(Expr&& expr, stdex::extents<IndexType, SliceExtents...>&& slice_extents, std::tuple<Offsets...>&& offsets, std::tuple<Steps...>&& steps)
                        : Base(), m_offsets(std::forward<std::tuple<Offsets...>>(offsets)), m_steps(std::forward<std::tuple<Steps...>>(steps)), m_expr(std::forward<Expr>(expr)),  m_extents(std::forward<stdex::extents<IndexType, SliceExtents...>>(slice_extents))
                {}
                constexpr explicit Slice(const Slice&) noexcept = default;
                constexpr explicit Slice(Slice&&) noexcept = default;
                ~Slice() noexcept = default;

                constexpr Slice& operator=(const Slice&) noexcept = default;
                constexpr Slice& operator=(Slice&&) noexcept = default;

                using Base = BaseExpr<Slice<Expr, stdex::extents<IndexType, OriginalExtents...>, stdex::extents<IndexType, SliceExtents...>, Pack<Offsets...>, Pack<Steps...>>>;
                constexpr auto extents() const
                {
                        return m_extents();
                }
                constexpr auto extent(const size_t i) const
                {
                        return m_extents.extent(i);
                }

#ifdef CLANGBUG
                constexpr auto operator()(auto&& ... indices) const
#else
                constexpr auto operator[](auto&& ... indices) const
#endif
                {
                        return get_expanded_indices(
                                        indices...,
                                        std::make_index_sequence<sizeof...(OriginalExtents)>{}
                                        );
                }
        private:
                std::tuple<Offsets...> m_offsets;
                std::tuple<Steps...> m_steps;
                std::remove_cv_t<Expr> m_expr;
                stdex::extents<IndexType, SliceExtents...> m_extents;
                template<typename SliceIndex, typename... SliceIndices, size_t I, size_t... Is, typename... ExpandedIndices>
                constexpr auto get_expanded_indices(
                        SliceIndex index,
                        SliceIndices&&... indices,
                        std::index_sequence<I, Is...>,
                        ExpandedIndices&& ... expanded_indices) const
                {
                        if constexpr(sizeof...(Is) > 0){
                                if constexpr(std::get<I>(m_steps) > 0){
                                        return get_expanded_indices(
                                                        indices...,
                                                        std::index_sequence<Is...>{},
                                                        expanded_indices...,
                                                        std::get<I>(m_offsets) + index*std::get<I>(m_steps)
                                                        );
                                }else{
                                        return get_expanded_indices(
                                                        index,
                                                        indices...,
                                                        std::index_sequence<Is...>{},
                                                        expanded_indices...,
                                                        std::get<I>(m_offsets)
                                                        );
                                }
                        }else{
#ifdef CLANGBUG
                                return m_expr(expanded_indices...);
#else
                                return m_expr[expanded_indices...];
#endif
                        }
                }
                constexpr explicit Slice() noexcept = default;
};

};
#endif // EXPR_TEMPLATE_SLICE_EXPRESSION_H
