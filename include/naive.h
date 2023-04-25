#ifndef NAIVE_H
#define NAIVE_H

#include <algorithm>
#include <experimental/__p0009_bits/full_extent_t.hpp>
#include <vector>
#include <functional>
#include <experimental/mdspan>
#include <extents_utils.h>

namespace stdex = std::experimental;

namespace naive{

    template<typename T, typename Extents>
    class MDArray{
    public:
        using value_type = T;
        MDArray(const Extents& exts): m_data(exts::ext_size(exts)), m_mdspan(m_data.data(), exts) 
        {
            m_data.shrink_to_fit();
        }
        MDArray(const Extents& exts, const T val): m_data(exts::ext_size(exts), val), m_mdspan(m_data.data(), exts) 
        {
            m_data.shrink_to_fit();
        }

#ifdef CLANGBUG
        T& operator()(auto&&... indices){return m_mdspan(indices...);}
        const T& operator()(auto&&... indices) const {return m_mdspan(indices...);}
#endif
        T& operator[](auto&&... indices){return m_mdspan[indices...];}
        const T& operator[](auto&&... indices) const {return m_mdspan[indices...];}

        Extents extents() const {return m_mdspan.extents();}
        auto extent(size_t i) const {return m_mdspan.extent(i);}
        auto mdspan(){return m_mdspan;}
        auto mdspan() const {return m_mdspan;}

    private:
        std::vector<T> m_data;
        stdex::mdspan<T, Extents> m_mdspan;
    };

    template<typename TL, typename TR, typename Extents, typename BinaryOp, typename LayoutPolicy_L, typename LayoutPolicy_R, typename AccesorPolicy_L, typename AccesorPolicy_R>
    auto zip(const stdex::mdspan<TL, Extents, LayoutPolicy_L, AccesorPolicy_L>& lhs, const stdex::mdspan<TR, Extents, LayoutPolicy_R, AccesorPolicy_R>& rhs, BinaryOp&& op)
    // auto zip(const MDArray<TL, Extents>& lhs, const MDArray<TR, Extents>& rhs, BinaryOp&& op)
    {
        using value_type = decltype(std::declval<BinaryOp>()(TL{}, TR{}));
        MDArray<value_type, Extents> res(lhs.extents());
        exts::transform_each_index(lhs, rhs, res, rhs.extents(), std::forward<BinaryOp>(op));
        return res;
    }

    template<typename T, typename Extents, typename UnaryOp, typename LayoutPolicy, typename AccesorPolicy>
    auto map(const stdex::mdspan<T, Extents, LayoutPolicy, AccesorPolicy>& rhs, UnaryOp&& op)
    // auto map(const MDArray<T, Extents>& rhs, UnaryOp&& op)
    {
        using value_type = decltype(std::declval<UnaryOp>()(T{}));
        MDArray<value_type, Extents> res(rhs.extents());
        exts::transform_each_index(rhs, res, rhs.extents(), std::forward<UnaryOp>(op));
        return res;
    }

    template<typename T, typename Extents, typename ReduceOp, typename LayoutPolicy, typename AccesorPolicy>
    auto reduce(const stdex::mdspan<T, Extents, LayoutPolicy, AccesorPolicy>& rhs, ReduceOp&& op, decltype(std::declval<ReduceOp>()(T{}, T{})) acc = decltype(std::declval<ReduceOp>()(T{}, T{})){})
    // auto reduce(const MDArray<T, Extents>& rhs, ReduceOp&& op, decltype(std::declval<ReduceOp>()(std::declval<typename MDArray<T, Extents>::value_type>())) acc = decltype(std::declval<ReduceOp>()(std::declval<typename MDArray<T, Extents>::value_type>()))())
    {
        return exts::reduce_each_index(rhs, acc, rhs.extents(), std::forward<ReduceOp>(op));
    }

    template<typename T, typename Extents>
    MDArray<T, Extents> operator+(const MDArray<T, Extents>& lhs, const MDArray<T, Extents>& rhs)
    {
        return zip(lhs.mdspan(), rhs.mdspan(), std::plus<>());
    }

    template<typename T, typename Extents>
    MDArray<T, Extents> operator-(const MDArray<T, Extents>& lhs, const MDArray<T, Extents>& rhs)
    {
        return zip(lhs.mdspan(), rhs.mdspan(), std::minus<>());
    }

    template<typename T, typename Extents>
    MDArray<T, Extents> operator*(const MDArray<T, Extents>& lhs, const MDArray<T, Extents>& rhs)
    {
        return zip(lhs.mdspan(), rhs.mdspan(), std::multiplies<>());
    }

    template<typename T, typename Extents>
    MDArray<T, Extents> operator/(const MDArray<T, Extents>& lhs, const MDArray<T, Extents>& rhs)
    {
        return zip(lhs.mdspan(), rhs.mdspan(), std::divides<>());
    }

    template<typename T, typename Extents_L, typename Extents_R>
    MDArray<T, Extents_R> matmul(const MDArray<T, Extents_L>& lhs, const MDArray<T, Extents_R>& rhs)
    {
            using value_type = decltype(std::declval<std::multiplies<>>()(T{}, T{}));
            MDArray<value_type, Extents_R> res(lhs.extents());
            auto matmuler = [&](auto... index, auto i, auto j){
                    auto sub_l = stdex::submdspan(lhs.mdspan(), index..., i, stdex::full_extent);
                    auto sub_r = stdex::submdspan(rhs.mdspan(), index..., stdex::full_extent, j);
                    auto tmp = zip(sub_l, sub_r, std::multiplies<>());
                    res[index..., i, j] =  reduce(tmp.mdspan(), std::plus<>());
            };
            exts::for_each_index(res.extents(), std::move(matmuler));
            return res;
    }

}; // namespace naive
#endif // NAIVE_H
