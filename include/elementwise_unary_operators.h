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
            : Base(), m_rhs(std::forward<RHS>(rhs)), m_op(std::move(op))
        {}
        ~ElementwiseUnaryOp() noexcept = default;

        constexpr inline auto extents() const noexcept {return m_rhs.extents();};
        constexpr inline auto extent(std::size_t i) const noexcept {return m_rhs.extent(i);};

        constexpr inline auto operator[](auto&& ... indices) const noexcept {return m_op(m_rhs[indices...]);}
        constexpr explicit ElementwiseUnaryOp(const ElementwiseUnaryOp&) noexcept = default;
        constexpr explicit ElementwiseUnaryOp(ElementwiseUnaryOp&&) noexcept = default;

        constexpr ElementwiseUnaryOp& operator=(const ElementwiseUnaryOp&) noexcept = default;
        constexpr ElementwiseUnaryOp& operator=(ElementwiseUnaryOp&&) noexcept = default;
    private:
        RHS m_rhs;
        UNARY_OP m_op;

        constexpr explicit ElementwiseUnaryOp() noexcept = default;
};

// template<expression Expr, typename F>
// constexpr inline auto map(const Expr& expr, F&& f) noexcept
// {
//     return ElementwiseUnaryOp<Expr, F>(expr, std::forward<F>(f));
// }
template<expression Expr, typename F>
constexpr inline auto map(Expr&& expr, F&& f) noexcept
{
    return ElementwiseUnaryOp<Expr, F>(std::forward<Expr>(expr), std::forward<F>(f));
}


template<typename T>
struct ElementScale
{
    constexpr ElementScale() = default;
    constexpr ElementScale(const T& val) : m_val(val) {}
    constexpr inline T operator()(const T& a) const noexcept {return m_val*a;}
    private:
    const T m_val;
};

// template<expression RHS>
// constexpr inline auto operator*(const typename RHS::value_type scalar, const RHS& rhs) noexcept
// {
//     return map<RHS, ElementScale<typename RHS::value_type>>(rhs, ElementScale<typename RHS::value_type>(scalar));
// }
template<expression RHS>
constexpr inline auto operator*(const typename std::remove_reference_t<RHS>::value_type scalar, RHS&& rhs) noexcept
{
    return map<RHS, ElementScale<typename std::remove_reference_t<RHS>::value_type>>(std::forward<RHS>(rhs), ElementScale<typename std::remove_reference_t<RHS>::value_type>(scalar));
}

// template<expression LHS>
// constexpr inline auto operator*(const LHS& lhs, const typename LHS::value_type scalar) noexcept
// {
//     return map<LHS, ElementScale<typename LHS::value_type>>(lhs, ElementScale<typename LHS::value_type>(scalar));
// }
template<expression LHS>
constexpr inline auto operator*(LHS&& lhs, const typename std::remove_reference_t<LHS>::value_type scalar) noexcept
{
    return map<LHS, ElementScale<typename std::remove_reference_t<LHS>::value_type>>(std::forward<LHS>(lhs), ElementScale(scalar));
}

template<typename T>
struct ElementScaleRecipr
{
    constexpr ElementScaleRecipr() = default;
    constexpr ElementScaleRecipr(const T val) : m_val(val) {}
    constexpr inline T operator()(const T a) const noexcept {return m_val/a;}
    private:
    const T m_val;
};

// template<expression RHS>
// constexpr inline auto operator/(const typename RHS::value_type& scalar, const RHS& rhs) noexcept
// {
//     return map<RHS, ElementScaleRecipr<typename RHS::value_type>>(rhs, ElementScaleRecipr<typename RHS::value_type>(scalar));
// }
template<expression RHS>
constexpr inline auto operator/(const typename std::remove_reference_t<RHS>::value_type scalar, RHS&& rhs) noexcept
{
    return map<RHS, ElementScaleRecipr<typename std::remove_reference_t<RHS>::value_type>>(std::forward<RHS>(rhs), ElementScaleRecipr(scalar));
}

template<typename T>
struct ElementScaleDiv
{
    constexpr ElementScaleDiv() = default;
    constexpr ElementScaleDiv(const T& val) : m_val(val) {}
    constexpr inline T operator()(const T& a) const noexcept {return a/m_val;}
    private:
    const T m_val;
};

// template<expression LHS>
// constexpr inline auto operator/(const LHS& lhs, const typename LHS::value_type& scalar) noexcept
// {
//     return map<LHS, ElementScaleDiv<typename LHS::value_type>>(lhs, ElementScaleDiv<typename LHS::value_type>(scalar));
// }
template<expression LHS>
constexpr inline auto operator/(LHS&& lhs, const typename std::remove_reference_t<LHS>::value_type& scalar) noexcept
{
    return map<LHS, ElementScaleDiv<typename std::remove_reference_t<LHS>::value_type>>(std::forward<LHS>(lhs), ElementScaleDiv<typename std::remove_reference_t<LHS>::value_type>(scalar));
}

// template<expression RHS>
// constexpr inline auto operator-(const RHS& rhs) noexcept
// {
//     return map<RHS, std::negate<>>(rhs, std::negate<>());
// }
template<expression RHS>
constexpr inline auto operator-(RHS&& rhs) noexcept
{
    return map<RHS, std::negate<>>(std::forward<RHS>(rhs), std::negate<>());
}

}; // namespace expr
#endif // EXPR_TEMPLATE_ELEMENTWISE_UNARY_OPERATOR_H
