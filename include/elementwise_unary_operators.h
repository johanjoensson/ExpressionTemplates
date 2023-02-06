#ifndef EXPR_TEMPLATE_ELEMENTWISE_UNARY_OPERATOR_H
#define EXPR_TEMPLATE_ELEMENTWISE_UNARY_OPERATOR_H

#include <base_expression.h>
#include <functional>

namespace expr{

template<expression RHS, typename UNARY_OP>
class ElementwiseUnaryOp: public BaseExpr<ElementwiseUnaryOp<RHS, UNARY_OP>>{
    public:
        using Base = BaseExpr<ElementwiseUnaryOp<RHS, UNARY_OP>>;
        using RHS_noref = std::remove_reference_t<RHS>;
        using value_type = decltype(std::declval<UNARY_OP>()(std::declval<typename RHS_noref::value_type>()));

        constexpr explicit ElementwiseUnaryOp(RHS&& rhs, UNARY_OP&& op = UNARY_OP()) noexcept
            : Base(), m_rhs(std::forward<RHS>(rhs)), m_op(std::forward<UNARY_OP>(op))
        {}
        ~ElementwiseUnaryOp() noexcept = default;

        constexpr inline auto extents() const noexcept {return m_rhs.extents();};
        constexpr inline auto extent(std::size_t i) const noexcept {return m_rhs.extent(i);};

#ifdef CLANGBUG
        constexpr inline auto operator()(auto&& ... indices) const {return m_op(m_rhs(indices...));}
#endif
        constexpr inline auto operator[](auto&& ... indices) const {return m_op(m_rhs[indices...]);}
        constexpr explicit ElementwiseUnaryOp(const ElementwiseUnaryOp&) noexcept = default;
        constexpr explicit ElementwiseUnaryOp(ElementwiseUnaryOp&&) noexcept = default;

        constexpr ElementwiseUnaryOp& operator=(const ElementwiseUnaryOp&) noexcept = default;
        constexpr ElementwiseUnaryOp& operator=(ElementwiseUnaryOp&&) noexcept = default;
    private:
        std::remove_cv_t<RHS> m_rhs;
        UNARY_OP m_op;

        constexpr explicit ElementwiseUnaryOp() noexcept = default;
};

template<expression Expr, typename UnaryOp>
constexpr inline auto map(Expr&& expr, UnaryOp&& op) noexcept
{
    return ElementwiseUnaryOp<Expr, UnaryOp>(std::forward<Expr>(expr), std::forward<UnaryOp>(op));
}

template<expression RHS>
constexpr inline auto operator*(const typename std::remove_reference_t<RHS>::value_type scalar, RHS&& rhs) noexcept
{
    auto multiplier = [scalar](auto elem){return elem*scalar;};
    return map(std::forward<RHS>(rhs), std::move(multiplier));
}

template<expression LHS>
constexpr inline auto operator*(LHS&& lhs, const typename std::remove_reference_t<LHS>::value_type scalar) noexcept
{
    auto multiplier = [scalar](auto elem){return elem*scalar;};
    return map(std::forward<LHS>(lhs), std::move(multiplier));
}

template<expression RHS>
constexpr inline auto operator/(const typename std::remove_reference_t<RHS>::value_type scalar, RHS&& rhs) noexcept
{
    auto div = [scalar](auto elem){return scalar/elem;};
    return map(std::forward<RHS>(rhs), std::move(div));
}

template<expression LHS>
constexpr inline auto operator/(LHS&& lhs, const typename std::remove_reference_t<LHS>::value_type& scalar) noexcept
{
    auto div = [scalar](auto elem){return scalar*(1/elem);};
    return map(std::forward<LHS>(lhs), std::move(div));
}

template<expression RHS>
constexpr inline auto operator-(RHS&& rhs) noexcept
{
    return map(std::forward<RHS>(rhs), std::negate<>());
}

}; // namespace expr
#endif // EXPR_TEMPLATE_ELEMENTWISE_UNARY_OPERATOR_H
