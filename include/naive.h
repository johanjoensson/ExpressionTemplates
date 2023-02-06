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
        MDArray(const Extents& exts): m_data(ext_size(exts)), m_mdspan(m_data.data(), exts) 
        {
            m_data.shrink_to_fit();
        }
        MDArray(const Extents& exts, const T val): m_data(ext_size(exts), val), m_mdspan(m_data.data(), exts) 
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

    template<typename T, typename Extents, typename BinaryOp>
    MDArray<T, Extents> zip(const MDArray<T, Extents>& lhs, const MDArray<T, Extents>& rhs, BinaryOp&& op)
    {
        MDArray<T, Extents> res(lhs.extents());
        auto operate = [&]<typename... Indices>(Indices... indices)
        {
#ifdef CLANGBUG
            res(indices...) = op(lhs(indices...), rhs(indices...));
#else
            res[indices...] = op(lhs[indices...], rhs[indices...]);
#endif
        };
        for_each_index(res.extents(), operate);
        return res;
    }

    template<typename T, typename Extents, typename UnaryOp>
    MDArray<T, Extents> map(const MDArray<T, Extents>& rhs, UnaryOp&& op)
    {
        MDArray<T, Extents> res(rhs.extents());
        auto operate = [&]<typename... Indices>(Indices... indices)
        {
#ifdef CLANGBUG
            res(indices...) = op(rhs(indices...));
#else
            res[indices...] = op(rhs[indices...]);
#endif
        };
        for_each_index(res.extents(), operate);
        return res;
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
