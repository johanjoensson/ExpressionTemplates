#ifndef MDARRAY_H
#define MDARRAY_H

#include <expr_template.h>
#include <vector>

#include <experimental/mdspan>
namespace stdex = std::experimental;

using expr::BaseExpr;
using expr::expression;
#include <iostream>

template<class IndexType, std::size_t ... Extents, typename Operator>
constexpr void for_each_index(stdex::extents<IndexType, Extents...>, Operator&&) noexcept;

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

template<typename T, typename Extents>
class MDArray;

template<typename T, typename IndexType, size_t... Extents>
class MDArray<T, stdex::extents<IndexType, Extents...>>: public BaseExpr<MDArray<T, stdex::extents<IndexType, Extents...>>>{
    public:
        using value_type = T;
        constexpr explicit MDArray(const stdex::extents<IndexType, Extents...>& exts) noexcept 
            : m_data(ext_size(exts)), m_mdspan(m_data.data(), exts)
        {}

        constexpr explicit MDArray(const stdex::extents<IndexType, Extents...>& exts, T val) noexcept 
            : m_data(ext_size(exts), val), m_mdspan(m_data.data(), exts)
        {}

#ifdef CLANGBUG
        constexpr inline T& operator()(auto&&... indices) {return m_mdspan(indices...);}
        constexpr inline const T& operator()(auto&&... indices) const {return m_mdspan(indices...);}
#endif
        constexpr inline T& operator[](auto&&... indices) {return m_mdspan[indices...];}
        constexpr inline const T& operator[](auto&&... indices) const {return m_mdspan[indices...];}

        constexpr inline auto extents() const noexcept {return m_mdspan.extents();}
        constexpr inline auto extent(size_t i) const noexcept {return m_mdspan.extent(i);}
        constexpr operator stdex::mdspan<T, stdex::extents<IndexType, Extents...>>() noexcept {return m_mdspan;}

        template<expression Expr>
        constexpr MDArray(Expr&& expr) noexcept
         : MDArray(expr.extents())
        {
            auto assigner =  [&](auto&&... indices) noexcept{
#ifdef CLANGBUG
                (*this)(indices...) = expr(indices...);
#else
                (*this)[indices...] = expr[indices...];
#endif
            };
            for_each_index(this->extents(), assigner);
        }

        inline explicit MDArray() noexcept = default;
        inline explicit MDArray(const MDArray&) noexcept = default;
        inline explicit MDArray(MDArray&&) noexcept = default;
        inline ~MDArray() noexcept = default;

        inline MDArray& operator=(const MDArray&) noexcept = default;
        inline MDArray& operator=(MDArray&&) noexcept = default;
    private:
        std::vector<T> m_data;
        stdex::mdspan<T, stdex::extents<IndexType, Extents...>> m_mdspan;
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
    for_each_index(ext, std::forward<Operator>(op), std::make_index_sequence<sizeof...(Extents)>{});
}

#endif // MDARRAY_H
