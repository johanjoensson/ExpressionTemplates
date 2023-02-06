#ifndef MDARRAY_H
#define MDARRAY_H

#include <expr_template.h>
#include <extents_utils.h>
#include <vector>

#include <experimental/mdspan>
namespace stdex = std::experimental;

using expr::BaseExpr;
using expr::expression;

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

#endif // MDARRAY_H
