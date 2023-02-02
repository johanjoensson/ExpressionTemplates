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


template<typename T, typename Extents>
class MDArray: public BaseExpr<MDArray<T, Extents>>{
    public:
        using value_type = T;
        constexpr explicit MDArray(const Extents& exts) noexcept 
            : m_data(ext_size(exts)), m_mdspan(m_data.data(), exts)
        {
            m_data.shrink_to_fit();
        }
        constexpr explicit MDArray(const Extents& exts, const T val) noexcept 
            : m_data(ext_size(exts), val), m_mdspan(m_data.data(), exts)
        {
            m_data.shrink_to_fit();
        }

        template<typename... Indices>
        constexpr inline T& operator[](Indices... indices) noexcept {return m_mdspan[indices...];}
        template<typename... Indices>
        constexpr inline const T& operator[](Indices... indices) const noexcept {return m_mdspan[indices...];}

        constexpr inline Extents extents() const noexcept {return m_mdspan.extents();}
        constexpr inline auto extent(size_t i) const noexcept {return m_mdspan.extent(i);}
        constexpr operator stdex::mdspan<T, Extents>() noexcept {return m_mdspan;}

        template<expression Expr>
        constexpr MDArray(Expr&& expr) noexcept
         : MDArray(std::forward<Expr>(expr).extents())
        {
            auto assigner =  [&]<typename ...Indices>(Indices&&... indices) noexcept{
                (*this)[indices...] = std::forward<Expr>(expr)[indices...];
            };
            forEachIndex(this->extents(), assigner);
        }

        inline explicit MDArray() noexcept = default;
        inline explicit MDArray(const MDArray&) noexcept = default;
        inline explicit MDArray(MDArray&&) noexcept = default;
        inline ~MDArray() noexcept = default;

        inline MDArray& operator=(const MDArray&) noexcept = default;
        inline MDArray& operator=(MDArray&&) noexcept = default;
    private:
        std::vector<T> m_data;
        stdex::mdspan<T, Extents> m_mdspan;
};

template<typename IndexType, std::size_t ... Extents, typename F,
         std::size_t Exti, std::size_t... Exts, typename... Indices>
constexpr inline void forEachIndex(const stdex::extents<IndexType, Extents...>& ext, F&& func,
                  std::index_sequence<Exti, Exts...>, Indices... indices) noexcept
{
    for(IndexType i = 0; i < ext.extent(Exti); i++)
        if constexpr(sizeof...(Exts) > 0)
            forEachIndex(ext, std::forward<F>(func), std::index_sequence<Exts...>{}, indices..., i);
        else
            std::forward<F>(func)(indices..., i);
}

template<class IndexType, std::size_t ... Extents, typename F>
constexpr inline void forEachIndex(stdex::extents<IndexType, Extents...> ext, F&& func) noexcept
{
    // for column-major, you could reverse the index_sequence
    forEachIndex(ext, std::forward<F>(func), std::make_index_sequence<sizeof...(Extents)>{});
}

#endif // MDARRAY_H
