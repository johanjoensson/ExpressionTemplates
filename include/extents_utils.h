#ifndef EXPRESSION_TEMPLATE_EXTENTS_UTILS_H
#define EXPRESSION_TEMPLATE_EXTENTS_UTILS_H

#include <experimental/mdspan>
namespace stdex = std::experimental;

namespace exts{
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
    for(IndexType i = 0; i < ext.extent(Exti); i++){
        if constexpr(sizeof...(Exts) > 0){
            for_each_index(ext, std::forward<Operator>(op), std::index_sequence<Exts...>{}, indices..., i);
        }else{
            std::forward<Operator>(op)(indices..., i);
        }
    }
}

template<class IndexType, std::size_t ... Extents, typename Operator>
constexpr inline void for_each_index(stdex::extents<IndexType, Extents...> ext, Operator&& op) noexcept
{
    for_each_index(ext, std::forward<Operator>(op), std::make_index_sequence<sizeof...(Extents)>{});
}

template<typename Source1, typename Source2, typename Destination, typename IndexType, typename Operator, std::size_t ... Extents>
constexpr inline void transform_each_index(Source1&& source1, Source2&& source2, Destination& destination, const stdex::extents<IndexType, Extents...>& ext, Operator&& op) noexcept
{
    auto transform = [&](auto... indices)
        {
#ifdef CLANGBUG
                destination(indices...) = std::forward<Operator>(op)(std::forward<Source1>(source1)(indices...), std::forward<Source2>(source2)(indices...));
#else
                destination[indices...] = std::forward<Operator>(op)(std::forward<Source1>(source1)[indices...], std::forward<Source2>(source2)[indices...]);
#endif
        };
    for_each_index(ext, std::move(transform));
}

template<typename Source1, typename Source2, typename Destination, typename Operator>
constexpr inline void transform_each_index(Source1&& source1, Source2&& source2, Destination& destination, Operator&& op) noexcept
{
        return transform_each_index(std::forward<Source1>(source1), std::forward<Source2>(source2), destination, source1.extents(), std::forward<Operator>(op));
}

template<typename Source, typename Destination, typename IndexType, typename Operator, std::size_t ... Extents>
constexpr inline void transform_each_index(Source&& source, Destination& destination, const stdex::extents<IndexType, Extents...>& ext, Operator&& op) noexcept
{
    auto transform = [&](auto... indices)
        {
#ifdef CLANGBUG
                destination(indices...) = std::forward<Operator>(op)(std::forward<Source>(source)(indices...));
#else
                destination[indices...] = std::forward<Operator>(op)(std::forward<Source>(source)[indices...]);
#endif
        };
    for_each_index(ext, std::move(transform));
}

template<typename Source, typename Destination, typename Operator>
constexpr inline void transform_each_index(Source&& source, Destination& destination, Operator&& op) noexcept
{
        return transform_each_index(std::forward<Source>(source), destination, source.extents(), std::forward<Operator>(op));
}

template<typename Source, typename Destination, typename IndexType, std::size_t ... Extents>
constexpr inline void assign_each_index(Source&& source, Destination& destination, const stdex::extents<IndexType, Extents...>& ext) noexcept
{
    auto assign = [&](auto... indices)
        {
#ifdef CLANGBUG
                destination(indices...) = std::forward<Source>(source)(indices...);
#else
                destination[indices...] = std::forward<Source>(source)[indices...];
#endif
        };
    for_each_index(ext, std::move(assign));
}

template<typename Source, typename Destination>
constexpr inline void assign_each_index(Source&& source, Destination& destination) noexcept
{
        assign_each_index(std::forward<Source>(source), destination, source.extents());
}

template<typename Source, typename Accumulator, typename IndexType, std::size_t ... Extents, typename Operator>
constexpr inline Accumulator reduce_each_index(Source&& source, Accumulator acc, const stdex::extents<IndexType, Extents...>& ext, Operator&& op) noexcept
{
    auto reduce = [&](auto... indices)
        {
#ifdef CLANGBUG
                acc = std::forward<Operator>(op)(acc, std::forward<Source>(source)(indices...));
#else
                acc = std::forward<Operator>(op)(acc, std::forward<Source>(source)[indices...]);
#endif
        };
    for_each_index(ext, std::move(reduce));
    return acc;
}

template<typename Source, typename Accumulator, typename Operator>
constexpr inline Accumulator reduce_each_index(Source&& source, Accumulator acc, Operator&& op) noexcept
{
    return reduce_each_index(std::forward<Source>(source), acc, source.extents(), std::forward<Operator>(op));
}
};
#endif // EXPRESSION_TEMPLATE_EXTENTS_UTILS_H
