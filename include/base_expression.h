#ifndef EXPR_TEMPLATE_BASE_EXPRESSION_H
#define EXPR_TEMPLATE_BASE_EXPRESSION_H

#include <bits/utility.h>
#include <type_traits>
#include <concepts>
#include <extents_utils.h>

/***************************************************************************//**
 * \brief Namespace used for all expression templates, all expression template
 * classes and free standing functions.
 ******************************************************************************/
namespace expr
{

template<typename Expr>
class BaseExpr;

/***************************************************************************//**
* \brief Concept ensuring that expressions are derived from BaseExpr.
*
* Because expression templates are an example of the "curiously recurring
* template" (CRT) pattern, all expressions will derive from the BaseExpr class.
* BaseExpr is a templated class, with the derived class as its template
* parameter.
 ******************************************************************************/
template<typename Expr>
concept derived_from_baseexpr = 
std::derived_from<std::remove_cv_t<std::remove_reference_t<Expr>>,
                  BaseExpr<std::remove_cv_t<std::remove_reference_t<Expr>>>
                 >;

/***************************************************************************//**
* \brief Concept ensuring the existence of an extents() member function
*
* Concept for classes that have an extents() member function.
* Expressions do not store their sizes/dimensions/extents, instead they all
* must have an extents() member function. This function should return an extent
* as used by mdspan.
 ******************************************************************************/
template<typename Expr>
concept has_extents = requires(Expr e)
{
        e.extents();
        e.extent(0);
};

namespace detail
{
        template<typename Expr, std::size_t Ext_i, std::size_t... Exts, typename... Indices>
                constexpr inline typename Expr::value_type check_subscript(const Expr& e, std::index_sequence<Ext_i, Exts...>, Indices... indices)
                {
                        if constexpr(sizeof...(Exts) > 0){
                                return check_subscript(e, std::index_sequence<Exts...>{}, indices..., 0);
                        }else{
#ifdef CLANGBUG
                                return e(indices..., 0);
#else
                                return e[indices..., 0];
#endif
                        }
                }
        template<typename Expr, typename IndexType, std::size_t... Extents>
                constexpr inline typename Expr::value_type check_subscript(const Expr& e, const stdex::extents<IndexType, Extents...>& )
                {
                        return check_subscript(e, std::make_index_sequence<sizeof...(Extents)>{});
                }
}; // ::detail
/***************************************************************************//**
* \brief Concept ensuring the subscipt operator is present.
*
* This concepts ensures the presence of a  (possibly multi-)dimensional
* subscript operator. All expressions must be subscriptable, since this is how
* thay are evaluated.
 ******************************************************************************/
template<typename Expr>
concept has_subscript_operator = requires(Expr e)
{
        {detail::check_subscript(e, e.extents())};
};

/***************************************************************************//**
* \brief Concept describing a valid expression. 
*
* This gives the compiler the
* ability to, among other things, distinguish scalar values from expressions.
* Enabling such features as scalar multiplication, etc.
 ******************************************************************************/
template<typename Expr>
concept expression = derived_from_baseexpr<Expr> && 
                     has_extents<Expr> && 
                     has_subscript_operator<Expr>;

/***************************************************************************//**
* \brief Base class for all template expressions.
* 
* The BaseExpr template class defines a basic expression base class
* that all valid expressions must inherit from. BaseExpr is a template class,
* and takes the derived type as it's template parameter (this is the CRT).
 ******************************************************************************/
#ifndef DEDUCE_THIS
template<typename Expr>
#endif
class BaseExpr
{
    public:

#ifndef DEDUCE_THIS
        const Expr& self() const noexcept {return static_cast<const Expr&>(*this);}
        Expr& self() noexcept {return static_cast<Expr&>(*this);}
#endif

#ifndef DEDUCE_THIS
        constexpr auto extents() const noexcept {return self().extents();}
        constexpr auto extent(std::size_t i) const noexcept {return self().extent(i);}
#else
        constexpr auto extents(this auto& self) const noexcept {return self.extents();}
        constexpr auto extent(this auto& self, std::size_t i) const noexcept {return self.extent(i);}
#endif
#ifndef DEDUCE_THIS
#ifdef CLANGBUG
        constexpr auto operator()(auto&&... indices) const {return self()(indices...);}
#endif
        constexpr auto operator[](auto&&... indices) const {return self()[indices...];}
#else
#ifdef CLANGBUG
        constexpr auto operator()(this auto& self, auto&&... indices) const {return self(indices...);}
#endif
        constexpr auto operator[](this auto& self, auto&&... indices) const {return self[indices...];}
#endif
    protected:
        constexpr explicit BaseExpr() noexcept = default;
        constexpr explicit BaseExpr(const BaseExpr&) noexcept = default;
        constexpr explicit BaseExpr(BaseExpr&&) noexcept = default;
        ~BaseExpr() noexcept = default;

        constexpr BaseExpr& operator=(const BaseExpr&) noexcept = default;
        constexpr BaseExpr& operator=(BaseExpr&&) noexcept = default;
};

}; // ::expr
#endif // EXPR_TEMPLATE_BASE_EXPRESSION_H
