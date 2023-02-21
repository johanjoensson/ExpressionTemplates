#ifndef NAIVE_H
#define NAIVE_H

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
        operator stdex::mdspan<T, Extents>(){return m_mdspan;}

    private:
        std::vector<T> m_data;
        stdex::mdspan<T, Extents> m_mdspan;
    };

    template<typename TL, typename TR, typename Extents, typename BinaryOp>
    auto zip(const MDArray<TL, Extents>& lhs, const MDArray<TR, Extents>& rhs, BinaryOp&& op)
    {
        using value_type = decltype(std::declval<BinaryOp>()(std::declval<typename MDArray<TL, Extents>::value_type>(), std::declval<typename MDArray<TR, Extents>::value_type>()));
        MDArray<value_type, Extents> res(lhs.extents());
        exts::transform_each_index(lhs, rhs, res, rhs.extents(), std::forward<BinaryOp>(op));
        return res;
    }

    template<typename T, typename Extents, typename UnaryOp>
    auto map(const MDArray<T, Extents>& rhs, UnaryOp&& op)
    {
        using value_type = decltype(std::declval<UnaryOp>()(std::declval<typename MDArray<T, Extents>::value_type>()));
        MDArray<value_type, Extents> res(rhs.extents());
        exts::transform_each_index(rhs, res, rhs.extents(), std::forward<UnaryOp>(op));
        return res;
    }

    template<typename T, typename Extents, typename ReduceOp>
    auto reduce(const MDArray<T, Extents>& rhs, ReduceOp&& op, decltype(std::declval<ReduceOp>()(std::declval<typename MDArray<T, Extents>::value_type>())) acc = decltype(std::declval<ReduceOp>()(std::declval<typename MDArray<T, Extents>::value_type>()))())
    {
        return exts::reduce_each_index(rhs, acc, rhs.extents(), std::forward<ReduceOp>(op));
    }

    template<typename T, typename Extents>
    MDArray<T, Extents> operator+(const MDArray<T, Extents>& lhs, const MDArray<T, Extents>& rhs)
    {
        return zip(lhs, rhs, std::plus<>());
    }

    template<typename T, typename Extents>
    MDArray<T, Extents> operator-(const MDArray<T, Extents>& lhs, const MDArray<T, Extents>& rhs)
    {
        return zip(lhs, rhs, std::minus<>());
    }

    template<typename T, typename Extents>
    MDArray<T, Extents> operator*(const MDArray<T, Extents>& lhs, const MDArray<T, Extents>& rhs)
    {
        return zip(lhs, rhs, std::multiplies<>());
    }

    template<typename T, typename Extents>
    MDArray<T, Extents> operator/(const MDArray<T, Extents>& lhs, const MDArray<T, Extents>& rhs)
    {
        return zip(lhs, rhs, std::divides<>());
    }

}; // namespace naive
#endif // NAIVE_H
