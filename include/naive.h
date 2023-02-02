#ifndef NAIVE_H
#define NAIVE_H

#include <vector>
#include <experimental/mdspan>
#include <iostream>

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

        template<typename... Indices>
        T& operator[](Indices... indices){return m_mdspan[indices...];}
        template<typename... Indices>
        const T& operator[](Indices... indices) const {return m_mdspan[indices...];}

        Extents extents() const {return m_mdspan.extents();}
        auto extent(size_t i) const {return m_mdspan.extent(i);}
        operator stdex::mdspan<T, Extents>(){return m_mdspan;}

    private:
        std::vector<T> m_data;
        stdex::mdspan<T, Extents> m_mdspan;
    };

    template<typename T, typename Extents>
    MDArray<T, Extents> operator+(const MDArray<T, Extents>& lhs, const MDArray<T, Extents>& rhs)
    {
        MDArray<T, Extents> res(lhs.extents());
        auto add = [&]<typename... Indices>(Indices... indices)
        {
            res[indices...] = lhs[indices...] + rhs[indices...];
        };
        forEachIndex(res.extents(), add);
        return res;
    }

    template<typename T, typename Extents>
    MDArray<T, Extents> operator*(const MDArray<T, Extents>& lhs, const MDArray<T, Extents>& rhs)
    {
        MDArray<T, Extents> res(lhs.extents());
        auto mult = [&]<typename... Indices>(Indices... indices)
        {
            res[indices...] = lhs[indices...] * rhs[indices...];
        };
        forEachIndex(res.extents(), mult);
        return res;
    }

}; // namespace naive
#endif // NAIVE_H
