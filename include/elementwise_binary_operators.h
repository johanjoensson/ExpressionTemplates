#ifndef EXPR_TEMPLATE_ELEMENTWISE_BINARY_OPERATOR_H
#define EXPR_TEMPLATE_ELEMENTWISE_BINARY_OPERATOR_H

#include<base_expression.h>
#include <functional>

namespace expr
{

template<expression LHS, expression RHS, typename BINARY_OP>
class ElementwiseBinaryOp: public BaseExpr<ElementwiseBinaryOp<LHS, RHS, BINARY_OP>>{
    public:
        using Base = BaseExpr<ElementwiseBinaryOp<LHS, RHS, BINARY_OP>>;
        using RHS_noref = std::remove_reference_t<RHS>;
        using LHS_noref = std::remove_reference_t<LHS>;
        using value_type = decltype(std::declval<BINARY_OP>()(std::declval<typename LHS_noref::value_type>(), std::declval<typename RHS_noref::value_type>()));

        constexpr explicit ElementwiseBinaryOp(LHS&& lhs, RHS&& rhs, BINARY_OP&& op) noexcept
            : Base(), m_lhs(std::forward<LHS>(lhs)), m_rhs(std::forward<RHS>(rhs)), m_op(std::move(op))
        {}

        ~ElementwiseBinaryOp() noexcept = default;

        constexpr auto extents() const noexcept {return m_rhs.extents();};
        constexpr auto extent(std::size_t i) const noexcept {return m_rhs.extent(i);};

        constexpr inline auto operator[](auto&&  ... indices) const noexcept {return m_op(m_lhs[indices...], m_rhs[indices...]);}

        constexpr explicit ElementwiseBinaryOp(const ElementwiseBinaryOp&) noexcept = default;
        constexpr explicit ElementwiseBinaryOp(ElementwiseBinaryOp&&) noexcept = default;

        constexpr ElementwiseBinaryOp& operator=(const ElementwiseBinaryOp&) noexcept = default;
        constexpr ElementwiseBinaryOp& operator=(ElementwiseBinaryOp&&) noexcept = default;
    private:
        LHS m_lhs;
        RHS m_rhs;
        BINARY_OP m_op;

        constexpr explicit ElementwiseBinaryOp() noexcept = default;
};


// template<expression LHS, expression RHS>
// constexpr inline auto operator+(const LHS& lhs, const RHS& rhs) noexcept
// {
//     return ElementwiseBinaryOp<LHS, RHS, std::plus<>>(lhs, rhs, std::plus<>());
// }
template<expression LHS, expression RHS>
constexpr inline auto operator+(LHS&& lhs, RHS&& rhs) noexcept
{
     return ElementwiseBinaryOp<LHS, RHS, std::plus<>>(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::plus<>());
}

// template<expression LHS, expression RHS>
// constexpr inline auto operator-(const LHS& lhs, const RHS& rhs) noexcept
// {
//     return ElementwiseBinaryOp<LHS, RHS, std::minus<>>(lhs, rhs, std::minus<>());
// }
template<expression LHS, expression RHS>
constexpr inline auto operator-(LHS&& lhs, RHS&& rhs) noexcept
{
    return ElementwiseBinaryOp<LHS, RHS, std::minus<>>(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::minus<>());
}

// template<expression LHS, expression RHS>
// constexpr inline auto operator*(const LHS& lhs, const RHS& rhs) noexcept
// {
//     return ElementwiseBinaryOp<LHS, RHS, std::multiplies<>>(lhs, rhs, std::multiplies<>());
// }
template<expression LHS, expression RHS>
constexpr inline auto operator*(LHS&& lhs, RHS&& rhs) noexcept
{
    return ElementwiseBinaryOp<LHS, RHS, std::multiplies<>>(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::multiplies<>());
}

// template<expression LHS, expression RHS>
// constexpr inline auto operator/(const LHS& lhs, const RHS& rhs) noexcept
// {
//     return ElementwiseBinaryOp<LHS, RHS, std::divides<>>(lhs, rhs, std::divides<>());
// }
template<expression LHS, expression RHS>
constexpr inline auto operator/(LHS&& lhs, RHS&& rhs) noexcept
{
    return ElementwiseBinaryOp<LHS, RHS, std::divides<>>(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::divides<>());
}
}; //expr

#endif // EXPR_TEMPLATE_ELEMENTWISE_BINARY_OPERATOR_H
