#ifndef EXPR_TEMPLATE_ELEMENTWISE_BINARY_OPERATOR_H
#define EXPR_TEMPLATE_ELEMENTWISE_BINARY_OPERATOR_H

#include<base_expression.h>
#include <functional>

namespace expr
{

template<expression LHS, expression RHS, typename BINARY_OP>
using binary_return_type = decltype(std::declval<BINARY_OP>()(std::declval<typename std::remove_reference_t<LHS>::value_type>(), std::declval<typename std::remove_reference_t<RHS>::value_type>()));

template<expression LHS, expression RHS, typename BINARY_OP>
class ElementwiseBinaryOp: public BaseExpr<ElementwiseBinaryOp<LHS, RHS, BINARY_OP>>{
    public:
        using Base = BaseExpr<ElementwiseBinaryOp<LHS, RHS, BINARY_OP>>;
        using RHS_noref = std::remove_reference_t<RHS>;
        using LHS_noref = std::remove_reference_t<LHS>;
        using value_type = binary_return_type<LHS, RHS, BINARY_OP>;

        constexpr explicit ElementwiseBinaryOp(LHS&& lhs, RHS&& rhs, BINARY_OP&& op) noexcept
         : Base(), m_lhs(std::forward<LHS>(lhs)), m_rhs(std::forward<RHS>(rhs)), m_op(std::forward<BINARY_OP>(op))
        {}

        ~ElementwiseBinaryOp() noexcept = default;

        constexpr auto extents() const noexcept {return m_rhs.extents();};
        constexpr auto extent(std::size_t i) const noexcept {return m_rhs.extent(i);};

#ifdef CLANGBUG
        constexpr auto operator()(auto&&  ... indices) const {return m_op(m_lhs(indices...), m_rhs(indices...));}
#endif
        constexpr auto operator[](auto&&  ... indices) const {return m_op(m_lhs[indices...], m_rhs[indices...]);}

        constexpr explicit ElementwiseBinaryOp(const ElementwiseBinaryOp&) noexcept = default;
        constexpr explicit ElementwiseBinaryOp(ElementwiseBinaryOp&&) noexcept = default;

        constexpr ElementwiseBinaryOp& operator=(const ElementwiseBinaryOp&) noexcept = default;
        constexpr ElementwiseBinaryOp& operator=(ElementwiseBinaryOp&&) noexcept = default;
    private:
        std::remove_cv_t<LHS> m_lhs;
        std::remove_cv_t<RHS> m_rhs;
        BINARY_OP m_op;

        constexpr explicit ElementwiseBinaryOp() noexcept = default;
};


template<expression LHS, expression RHS, typename BinaryOp>
constexpr inline auto zip(LHS&& lhs, RHS&& rhs, BinaryOp&& op) noexcept
{
    return ElementwiseBinaryOp<LHS, RHS, BinaryOp>(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::forward<BinaryOp>(op));
}

template<expression LHS, expression RHS>
constexpr inline auto operator+(LHS&& lhs, RHS&& rhs) noexcept
{
     return zip(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::plus<>());
}

template<expression LHS, expression RHS>
constexpr inline auto operator-(LHS&& lhs, RHS&& rhs) noexcept
{
    return zip(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::minus<>());
}

template<expression LHS, expression RHS>
constexpr inline auto operator*(LHS&& lhs, RHS&& rhs) noexcept
{
    return zip(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::multiplies<>());
}

template<expression LHS, expression RHS>
constexpr inline auto operator/(LHS&& lhs, RHS&& rhs) noexcept
{
    return zip(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::divides<>());
}
}; //expr

#endif // EXPR_TEMPLATE_ELEMENTWISE_BINARY_OPERATOR_H
