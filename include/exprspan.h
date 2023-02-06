#ifndef MDARRAY_H
#define MDARRAY_H

#include <expr_template.h>
#include <extents_utils.h>
#include <vector>

#include <experimental/mdspan>
namespace stdex = std::experimental;

using expr::BaseExpr;
using expr::expression;

template<typename T, typename IndexType, size_t... Extents>
class MDExprSpan: public BaseExpr<ExprSpan<T, IndexType, Extents...>>{
    public:
        using value_type = T;
        constexpr explicit MDExprSpan(stdex::mdspan<T, stdex::extents<IndexType, Extents...>>&& mds) noexcept 
            : m_mdspan(std::move(mds))
        {}
        constexpr explicit MDExprSpan(const stdex::mdspan<T, stdex::extents<IndexType, Extents...>>& mds) noexcept 
            : m_mdspan(mds)
        {}

#ifdef CLANGBUG
        constexpr T& operator()(auto&&... indices) noexcept {return m_mdspan(indices...);}
        constexpr const T& operator()(auto&&... indices) const noexcept {return m_mdspan(indices...);}
#endif
        constexpr T& operator[](auto&&... indices) noexcept {return m_mdspan[indices...];}
        constexpr const T& operator[](auto&&... indices) const noexcept {return m_mdspan[indices...];}

        constexpr auto extents() const noexcept {return m_mdspan.extents();}
        constexpr auto extent(size_t i) const noexcept {return m_mdspan.extent(i);}
        constexpr operator stdex::mdspan<T, stdex::extents<IndexType, Extents...>>() noexcept {return m_mdspan;}

        template<expression Expr>
        constexpr MDExprSpan& operator=(Expr&& expr) noexcept
        {
            auto assigner =  [&]<typename ...Indices>(Indices&&... indices) noexcept{
#ifdef CLANGBUG
                (*this)(indices...) = std::forward<Expr>(expr)(indices...);
#else
                (*this)[indices...] = std::forward<Expr>(expr)[indices...];
#endif
            };
            for_each_index(this->extents(), assigner);
            return *this;
        }

        explicit MDExprSpan() noexcept = default;
        explicit MDExprSpan(const MDExprSpan&) noexcept = default;
        explicit MDExprSpan(MDExprSpan&&) noexcept = default;
        ~MDExprSpan() noexcept = default;

        MDExprSpan& operator=(const MDExprSpan&) noexcept = default;
        <MDExprSpan& operator=(MDExprSpan&&) noexcept = default;
    private:
        stdex::mdspan<T, stdex::mdspan<IndexType, MDExtents>> m_mdspan;
};

#endif // MDARRAY_H
