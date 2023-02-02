#ifndef EXPR_TEMPLATE_H
#define EXPR_TEMPLATE_H

#include <functional>
#include <type_traits>
#include <concepts>

namespace expr
{

template<typename Expr>
class BaseExpr;

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

template<typename RHS, typename UNARY_OP>
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
        const UNARY_OP m_op;

        constexpr explicit ElementwiseUnaryOp() noexcept = default;
};

template<typename LHS, typename RHS, typename BINARY_OP>
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
        const BINARY_OP m_op;

        constexpr explicit ElementwiseBinaryOp() noexcept = default;
};

template<typename Expr, typename F>
constexpr inline auto map(Expr&& expr, F&& f) noexcept
{
    return ElementwiseUnaryOp<Expr, F>(std::forward<Expr>(expr), std::forward<F>(f));
}

template<typename RHS>
constexpr inline auto operator-(RHS&& rhs) noexcept
{
    return map<RHS, std::negate<>>(std::forward<RHS>(rhs), std::negate<>());
}

}; //expr
#endif // EXPR_TEMPLATE_H

