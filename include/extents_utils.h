#ifndef EXPRESSION_TEMPLATE_EXTENTS_UTILS_H
#define EXPRESSION_TEMPLATE_EXTENTS_UTILS_H

#include <experimental/mdspan>
namespace stdex = std::experimental;

template<typename IndexType, size_t... Extents, std::size_t Exti, std::size_t... Exts>
constexpr inline size_t ext_size(const stdex::extents<IndexType, Extents...>& exts, std::index_sequence<Exti, Exts...>) noexcept
{
    if constexpr(sizeof...(Exts) > 0){
        return exts.extent(Exti)*ext_size(exts, std::index_sequence<Exts...>{});
    }else{
        return exts.extent(Exti);
    }
}

template<typename IndexType, size_t... Extents>
constexpr inline size_t ext_size(const stdex::extents<IndexType, Extents...>& exts) noexcept
{
    return ext_size(exts, std::make_index_sequence<sizeof...(Extents)>{});
}

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

#endif // EXPRESSION_TEMPLATE_EXTENTS_UTILS_H
