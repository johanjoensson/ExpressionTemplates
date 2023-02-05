#ifndef MDARRAY_H
#define MDARRAY_H

#include <expr_template.h>
#include <vector>

#include <experimental/mdspan>
namespace stdex = std::experimental;

using expr::BaseExpr;
using expr::expression;
#include <iostream>

template<class IndexType, std::size_t ... Extents, typename Func>
constexpr void forEachIndex(stdex::extents<IndexType, Extents...>, Func&&) noexcept;

template<typename IndexType, size_t... Extents, std::size_t Exti, std::size_t... Exts>
constexpr size_t ext_size(const stdex::extents<IndexType, Extents...>& exts, std::index_sequence<Exti, Exts...>) noexcept
{
    if constexpr(sizeof...(Exts) > 0){
        return exts.extent(Exti)*ext_size(exts, std::index_sequence<Exts...>{});
    }else{
        return exts.extent(Exti);
    }
}

template<typename IndexType, size_t... Extents>
constexpr size_t ext_size(const stdex::extents<IndexType, Extents...>& exts) noexcept
{
    return ext_size(exts, std::make_index_sequence<sizeof...(Extents)>{});
}


template<typename T, typename IndexType, size_t... Extents>
class ExprSpan: public BaseExpr<ExprSpan<T, IndexType, Extents...>>{
    public:
        using value_type = T;
        constexpr explicit ExprSpan(stdex::mdspan<T, stdex::extents<IndexType, Extents...>>&& mds) noexcept 
            : m_mdspan(std::move(mds))
        {}
        constexpr explicit ExprSpan(const stdex::mdspan<T, stdex::extents<IndexType, Extents...>>& mds) noexcept 
            : m_mdspan(mds)
        {}

        constexpr inline T& operator()(auto&&... indices) noexcept {return m_mdspan(indices...);}
        constexpr inline const T& operator()(auto&&... indices) const noexcept {return m_mdspan(indices...);}
        constexpr inline T& operator[](auto&&... indices) noexcept {return m_mdspan[indices...];}
        constexpr inline const T& operator[](auto&&... indices) const noexcept {return m_mdspan[indices...];}

        constexpr inline auto extents() const noexcept {return m_mdspan.extents();}
        constexpr inline auto extent(size_t i) const noexcept {return m_mdspan.extent(i);}
        constexpr operator stdex::mdspan<T, stdex::extents<IndexType, Extents...>>() noexcept {return m_mdspan;}

        template<expression Expr>
        constexpr ExprSpan& operator=(Expr&& expr) noexcept
        {
            auto assigner =  [&]<typename ...Indices>(Indices&&... indices) noexcept{
#ifdef CLANGBUG
                (*this)(indices...) = std::forward<Expr>(expr)(indices...);
#else
                (*this)[indices...] = std::forward<Expr>(expr)[indices...];
#endif
            };
            forEachIndex(this->extents(), assigner);
            return *this;
        }

        inline explicit ExprSpan() noexcept = default;
        inline explicit ExprSpan(const ExprSpan&) noexcept = default;
        inline explicit ExprSpan(ExprSpan&&) noexcept = default;
        inline ~ExprSpan() noexcept = default;

        inline ExprSpan& operator=(const ExprSpan&) noexcept = default;
        inline ExprSpan& operator=(ExprSpan&&) noexcept = default;
    private:
        stdex::mdspan<T, stdex::mdspan<IndexType, Extents>> m_mdspan;
};

template<typename IndexType, std::size_t ... Extents, typename Operator,
         std::size_t Exti, std::size_t... Exts, typename... Indices>
constexpr inline void for_each_index(const stdex::extents<IndexType, Extents...>& ext, Operator&& op,
                  std::index_sequence<Exti, Exts...>, Indices... indices) noexcept
{
    for(IndexType i = 0; i < ext.extent(Exti); i++)
        if constexpr(sizeof...(Exts) > 0)
            for_each_index(ext, std::forward<Operator>(op), std::index_sequence<Exts...>{}, indices..., i);
        else
            std::forward<Operator>(op)(indices..., i);
}

template<class IndexType, std::size_t ... Extents, typename Operator>
constexpr inline void for_each_index(stdex::extents<IndexType, Extents...> ext, Operator&& op) noexcept
{
    // for column-major, you could reverse the index_sequence
    for_each_index(ext, std::forward<Operator>(op), std::make_index_sequence<sizeof...(Extents)>{});
}

#endif // MDARRAY_H
