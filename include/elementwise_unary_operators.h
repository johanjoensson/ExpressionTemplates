#ifndef EXPR_TEMPLATE_ELEMENTWISE_UNARY_OPERATOR_H
#define EXPR_TEMPLATE_ELEMENTWISE_UNARY_OPERATOR_H

#include <base_expression.h>
#include <functional>

namespace expr{

template<expression RHS, typename UNARY_OP>
using unary_return_type = decltype(std::declval<UNARY_OP>()(std::declval<typename std::remove_reference_t<RHS>::value_type>()));

/***************************************************************************//**
* ElementWiseUnaryOp represents expressions where a unary function is
* applied to each element in another expression. The evaluation of the
* function is not performed until the specific element of this expression is
* required (via the subscript operator).
 ******************************************************************************/
template<expression RHS, typename UNARY_OP>
class ElementwiseUnaryOp: public BaseExpr<ElementwiseUnaryOp<RHS, UNARY_OP>>{
    public:
        using Base = BaseExpr<ElementwiseUnaryOp<RHS, UNARY_OP>>;
        using RHS_noref = std::remove_reference_t<RHS>;
        using value_type = unary_return_type<RHS, UNARY_OP>;

        constexpr explicit ElementwiseUnaryOp(RHS&& rhs, UNARY_OP&& op = UNARY_OP()) noexcept
            : Base(), m_rhs(std::forward<RHS>(rhs)), m_op(std::forward<UNARY_OP>(op))
        {}
        ~ElementwiseUnaryOp() noexcept = default;

        constexpr auto extents() const noexcept {return m_rhs.extents();};
        constexpr auto extent(std::size_t i) const noexcept {return m_rhs.extent(i);};

#ifdef CLANGBUG
        constexpr auto operator()(auto&& ... indices) const {return m_op(m_rhs(indices...));}
#endif
        constexpr auto operator[](auto&& ... indices) const {return m_op(m_rhs[indices...]);}

        constexpr explicit ElementwiseUnaryOp(const ElementwiseUnaryOp&) noexcept = default;
        constexpr explicit ElementwiseUnaryOp(ElementwiseUnaryOp&&) noexcept = default;

        constexpr ElementwiseUnaryOp& operator=(const ElementwiseUnaryOp&) noexcept = default;
        constexpr ElementwiseUnaryOp& operator=(ElementwiseUnaryOp&&) noexcept = default;
    private:
        std::remove_cv_t<RHS> m_rhs;
        UNARY_OP m_op;

        constexpr explicit ElementwiseUnaryOp() noexcept = default;
};

/***************************************************************************//**
* The map function is the fundamental operation for an ElementwiseUnaryOp
* expression. It takes an expression and an operator and returns an
* ElementwiseUnaryOp representing the result of applying the operator to
* each element of the expression.
 ******************************************************************************/
template<expression Expr, typename UnaryOp>
constexpr inline auto map(Expr&& expr, UnaryOp&& op) noexcept
{
    return ElementwiseUnaryOp<Expr, UnaryOp>(std::forward<Expr>(expr), std::forward<UnaryOp>(op));
}

/***************************************************************************//**
* Multiplying a scalar by an expression results in an 
* ElementwiseUnaryOp representing the multiplication of every element in the
* expression by the scalar.
 ******************************************************************************/
template<expression RHS>
constexpr inline auto operator*(const typename std::remove_reference_t<RHS>::value_type scalar, RHS&& rhs) noexcept
{
    auto multiplier = [scalar](auto elem){return elem*scalar;};
    return map(std::forward<RHS>(rhs), std::move(multiplier));
}

/***************************************************************************//**
* Multiplying an expression by a scalar results in an 
* ElementwiseUnaryOp representing the multiplication of every element in the
* expression by the scalar.
 ******************************************************************************/
template<expression LHS>
constexpr inline auto operator*(LHS&& lhs, const typename std::remove_reference_t<LHS>::value_type scalar) noexcept
{
    auto multiplier = [scalar](auto elem){return elem*scalar;};
    return map(std::forward<LHS>(lhs), std::move(multiplier));
}

/***************************************************************************//**
* Dividing a scalar with an expression results in an ElementwiseUnaryOp
* representing the division of the scalar by each element in the expression.
 ******************************************************************************/
template<expression RHS>
constexpr inline auto operator/(const typename std::remove_reference_t<RHS>::value_type scalar, RHS&& rhs) noexcept
{
    auto div = [scalar](auto elem){return scalar/elem;};
    return map(std::forward<RHS>(rhs), std::move(div));
}

/***************************************************************************//**
* Dividing an expression with a scalar results in an ElementwiseUnaryOp
* representing the division of each element in the expression by the scalar.
 ******************************************************************************/
template<expression LHS>
constexpr inline auto operator/(LHS&& lhs, const typename std::remove_reference_t<LHS>::value_type& scalar) noexcept
{
    auto div = [scalar](auto elem){return elem/scalar;};
    return map(std::forward<LHS>(lhs), std::move(div));
}

/***************************************************************************//**
* Negatign an expression results in an ElementwiseUnaryOp representing
* the negation of each element in the expression.
 ******************************************************************************/
template<expression RHS>
constexpr inline auto operator-(RHS&& rhs) noexcept
{
    return map(std::forward<RHS>(rhs), std::negate<>());
}

}; // namespace expr
#endif // EXPR_TEMPLATE_ELEMENTWISE_UNARY_OPERATOR_H
