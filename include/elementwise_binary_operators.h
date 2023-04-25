#ifndef EXPR_TEMPLATE_ELEMENTWISE_BINARY_OPERATOR_H
#define EXPR_TEMPLATE_ELEMENTWISE_BINARY_OPERATOR_H

#include<base_expression.h>
#include <functional>

namespace expr
{

template<expression LHS, expression RHS, typename BINARY_OP>
using binary_return_type = decltype(std::declval<BINARY_OP>()(std::declval<typename std::remove_reference_t<LHS>::value_type>(), std::declval<typename std::remove_reference_t<RHS>::value_type>()));

/***************************************************************************//**
* ElementwiseBinaryOp represents expressions where a function is
* applied to mathcing elements in two expressions. The evaluation of the
* function is not performed until the specific element of this expression is
* required (via the subscript operator).
 ******************************************************************************/
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


/***************************************************************************//**
* The zip function is the fundamental operation for an 
* ElementwiseBinaryOp expression. It takes two expressions and an operator and
* returns an ElementwiseBinaryOp representing the result of applying the
* operator to each pair of matching elements element of the expressions.
 ******************************************************************************/
template<expression LHS, expression RHS, typename BinaryOp>
constexpr inline auto zip(LHS&& lhs, RHS&& rhs, BinaryOp&& op)
{
    if (lhs.extents().rank() != rhs.extents().rank()){
            throw std::runtime_error("Rank of left hand side expression does not match rank of right hand side expression!\n" + std::to_string(lhs.extents().rank()) + " != " + std::to_string(rhs.extents().rank()));
    }
    for (size_t i = 0; i < lhs.extents().rank(); i++){
            if (lhs.extent(i) != rhs.extent(i)){
                    throw std::runtime_error("Dimensions do not match!\nDimension " + std::to_string(i) + ": " + std::to_string(lhs.extent(i)) + " != " + std::to_string(rhs.extent(i)));
            }
    }

    if(lhs.extent(lhs.extents().rank() - 1) != rhs.extent(lhs.extents().rank() - 2)){
            throw std::runtime_error("incompatible dimensions for matrix multiplication!\n" + std::to_string(lhs.extent(lhs.extents().rank() - 1)) + " != " + std::to_string(rhs.extent(rhs.extents().rank() - 2)));
    }
    return ElementwiseBinaryOp<LHS, RHS, BinaryOp>(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::forward<BinaryOp>(op));
}

/***************************************************************************//**
* Elementwise addition of two expressions results in an ElementwiseBinaryOp
* representing the addition of matching elements in two expressions.
 ******************************************************************************/
template<expression LHS, expression RHS>
constexpr inline auto operator+(LHS&& lhs, RHS&& rhs)
{
     return zip(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::plus<>());
}

/***************************************************************************//**
* Elementwise subtraction of two expressions results in an ElementwiseBinaryOp
* representing the subtraction of matching elements in two expressions.
 ******************************************************************************/
template<expression LHS, expression RHS>
constexpr inline auto operator-(LHS&& lhs, RHS&& rhs)
{
    return zip(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::minus<>());
}

/***************************************************************************//**
* Elementwise multiplication of two expressions results in an
* ElementwiseBinaryOp representing the multiplication of matching elements in
* two expressions.
 ******************************************************************************/
template<expression LHS, expression RHS>
constexpr inline auto operator*(LHS&& lhs, RHS&& rhs)
{
    return zip(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::multiplies<>());
}

/***************************************************************************//**
* Elementwise division of two expressions results in an
* ElementwiseBinaryOp representing the division of matching elements in two
* expressions.
 ******************************************************************************/
template<expression LHS, expression RHS>
constexpr inline auto operator/(LHS&& lhs, RHS&& rhs)
{
    return zip(std::forward<LHS>(lhs), std::forward<RHS>(rhs), std::divides<>());
}
}; //expr

#endif // EXPR_TEMPLATE_ELEMENTWISE_BINARY_OPERATOR_H
