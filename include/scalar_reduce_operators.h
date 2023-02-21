#ifndef EXPR_TEMPLATE_REDUCE_H
#define EXPR_TEMPLATE_REDUCE_H

#include <base_expression.h>
#include <extents_utils.h>
#include <functional>
#include <type_traits>
#include <limits>

namespace expr{

template<expression Expr, typename ReduceOp>
using reduce_return_type = decltype(std::declval<ReduceOp>()(std::declval<typename std::remove_reference_t<Expr>::value_type>(), std::declval<typename std::remove_reference_t<Expr>::value_type>()));

template<expression RHS, typename REDUCE_OP>
class ScalarReduceOp: public BaseExpr<ScalarReduceOp<RHS, REDUCE_OP>>{
    public:
        using Base = BaseExpr<ScalarReduceOp<RHS, REDUCE_OP>>;
        using RHS_noref = std::remove_reference_t<RHS>;
        using value_type = reduce_return_type<RHS, REDUCE_OP>;

        constexpr explicit ScalarReduceOp(RHS&& rhs, REDUCE_OP&& op, value_type&& acc) noexcept
          : m_rhs(std::forward<RHS>(rhs)), m_op(std::forward<REDUCE_OP>(op)), m_acc(std::forward<value_type>(acc))
        {}
        ~ScalarReduceOp() noexcept = default;

        constexpr auto extents() const noexcept = delete;
        constexpr auto extent(std::size_t i) const noexcept = delete;

        operator value_type() const noexcept
        {
                /*
                auto acc = m_acc;
                auto reduce_op = [&](auto&& ... indices ){acc = m_op(acc, m_rhs[indices...]);};
                for_each_index(m_rhs.extents(), reduce_op);
                return acc;
                */
                return exts::reduce_each_index(m_rhs, m_acc, m_rhs.extents(), m_op);
        }
        constexpr explicit ScalarReduceOp(const ScalarReduceOp&) noexcept = default;
        constexpr explicit ScalarReduceOp(ScalarReduceOp&&) noexcept = default;

        constexpr ScalarReduceOp& operator=(const ScalarReduceOp&) noexcept = default;
        constexpr ScalarReduceOp& operator=(ScalarReduceOp&&) noexcept = default;
    private:
        constexpr explicit ScalarReduceOp() noexcept = default;
        std::remove_cv_t<RHS_noref> m_rhs;
        std::remove_cv_t<REDUCE_OP> m_op;
        std::remove_cv_t<value_type> m_acc;
};

template<expression Expr, typename REDUCE_OP>
constexpr inline auto reduce(Expr&& expr, REDUCE_OP&& op, reduce_return_type<Expr, REDUCE_OP>&& acc_init = 0)
{
        using value_type = reduce_return_type<Expr, REDUCE_OP>;
        return ScalarReduceOp<Expr, REDUCE_OP>(std::forward<Expr>(expr), std::forward<REDUCE_OP>(op), std::forward<value_type>(acc_init));
}

template<expression Expr>
constexpr inline auto sum(Expr&& expr)
{
        auto f = [](auto acc, auto val){return acc + val;};
        return reduce(std::forward<Expr>(expr), std::move(f), 0);
}

template<expression Expr>
constexpr inline auto min(Expr&& expr)
{
        using value_type = typename std::remove_cv_t<std::remove_reference_t<Expr>>::value_type;
        auto f = [](auto acc, auto val){return std::min(acc, val);};
        return reduce(std::forward<Expr>(expr), std::move(f), std::numeric_limits<value_type>::max());
}

template<expression Expr>
constexpr inline auto max(Expr&& expr)
{
        using value_type = typename std::remove_cv_t<std::remove_reference_t<Expr>>::value_type;
        auto f = [](auto acc, auto val){return std::max(acc, val);};
        return reduce(std::forward<Expr>(expr), std::move(f), std::numeric_limits<value_type>::min());
}
}; // expr

#endif // EXPR_TEMPLATE_REDUCE_H
