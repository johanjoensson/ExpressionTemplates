#ifndef EXPR_TEMPLATE_BASE_EXPRESSION_H
#define EXPR_TEMPLATE_BASE_EXPRESSION_H

#include <type_traits>
#include <concepts>

namespace expr
{
// Forward declaration of BaseExpr
template<typename Expr>
class BaseExpr;

// All Expressions must derive from BaseExpr
template<typename Expr>
concept derived_from_baseexpr = 
std::derived_from<std::remove_reference_t<Expr>,
                  BaseExpr<std::remove_reference_t<Expr>>
                 >;

// All Expressions must implement a member function extents()
// that returns the extents of the underlying data
template<typename Expr>
concept has_extents = requires(Expr e)
{
        e.extents();
};

// All expressions must implement the subscript operator 
// for one or more arguments
template<typename Expr, typename... Indices>
concept has_subscript_operator = true; /*requires(Expr e, Indices... indices)
{e[indices...]};
*/

// Combine all requirements on Expressions into one concept, pedagogically named expression
template<typename Expr>
concept expression = derived_from_baseexpr<Expr> && 
                     has_extents<Expr> && 
                     (has_subscript_operator<Expr, std::size_t> ||
                      has_subscript_operator<Expr, std::size_t, std::size_t>
                     );


template<typename Expr>
class BaseExpr
{
    public:

        const Expr& self() const {return static_cast<const Expr&>(*this);}
        Expr& self() {return static_cast<Expr&>(*this);}

        constexpr inline auto extents() const {return self().extents();}
        constexpr inline auto extent(std::size_t i) const {return self().extent(i);}

        constexpr inline auto operator[](auto&&... indices) const {return self()[indices...];}
    protected:
        constexpr explicit BaseExpr() noexcept = default;
        constexpr explicit BaseExpr(const BaseExpr&) noexcept = default;
        constexpr explicit BaseExpr(BaseExpr&&) noexcept = default;
        virtual ~BaseExpr() noexcept = default;

        constexpr BaseExpr& operator=(const BaseExpr&) noexcept = default;
        constexpr BaseExpr& operator=(BaseExpr&&) noexcept = default;
};

}; // ::expr
#endif // EXPR_TEMPLATE_BASE_EXPRESSION_H
