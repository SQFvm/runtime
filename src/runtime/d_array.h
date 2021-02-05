#pragma once
#include "vec.h"
#include "data.h"
#include "type.h"
#include "value.h"
#include "d_scalar.h"

#include <string>
#include <memory>
#include <vector>
#include <algorithm>
#include <sstream>
#include <array>
#include <cmath>

namespace sqf
{
    namespace runtime
    {
        struct t_array : public type::extend<t_array> { t_array() : extend() {} static const std::string name() { return "ARRAY"; } };
    }
    namespace types
    {
        class d_array : public sqf::runtime::data
        {
        public:
            using data_type = sqf::runtime::t_array;
            using iterator = std::vector<sqf::runtime::value>::iterator;
        private:
            std::vector<sqf::runtime::value> m_value;
            bool recursion_test_(std::vector<std::shared_ptr<d_array>>& visited)
            {
                for (auto& it : m_value)
                {
                    if (it.type() == data_type())
                    {
                        // Get child
                        auto arr = it.data<sqf::types::d_array>();

                        // Check if child was visited already
                        if (std::find(visited.begin(), visited.end(), arr) != visited.end())
                        {
                            // Child already was visited, recursion test failed.
                            return false;
                        }

                        // Add child to visited list
                        visited.push_back(arr);

                        // Check child recursion
                        if (!arr->recursion_test_(visited))
                        {
                            return false;
                        }

                        // Remove child from visited list
                        visited.pop_back();
                    }
                }
                return true;
            }
        protected:
            bool do_equals(std::shared_ptr<data> other, bool invariant) const override
            {
                auto other_array = std::static_pointer_cast<d_array>(other);
                if (other_array->size() != m_value.size()) { return false; }
                for (size_t i = 0; i < m_value.size(); i++)
                {
                    // check if both empty
                    if (m_value[i].empty() && other_array->m_value[i].empty())
                    {
                        // Arma returns false on nil isEqualTo nil
                        return false;
                    }
                    // check if one is empty
                    else if (m_value[i].empty() || other_array->m_value[i].empty())
                    {
                        return false;
                    }
                    // Do actual comparison
                    else if (!m_value[i].data()->equals(other_array->m_value[i].data(), invariant))
                    {
                        return false;
                    }
                }
                return true;
            }
        public:
            d_array() = default;
            d_array(size_t size) : m_value(std::vector<sqf::runtime::value>(size)) {}
            d_array(std::vector<sqf::runtime::value> value) : m_value(std::move(value)) {}
            template<typename TIterator>
            d_array(TIterator begin, TIterator end) : m_value(begin, end) {}

            std::shared_ptr<d_array> copy_deep() const
            {
                std::vector<sqf::runtime::value> copy;
                for (auto& val : m_value)
                {
                    if (val.is<sqf::runtime::t_array>())
                    {
                        copy.emplace_back(val.data<d_array>()->copy_deep());
                    }
                    else
                    {
                        copy.emplace_back(val.data());
                    }
                }
                return std::make_shared<d_array>(copy);
            }

            std::string to_string_sqf() const override
            {
                std::stringstream sstream;
                sstream << "[";
                if (m_value.size() > 0)
                {
                    for (auto it : m_value)
                    {
                        sstream << it.to_string_sqf() << ",";
                    }
                    sstream.seekp(-1, std::ios_base::end);
                }
                sstream << "]";
                return sstream.str();
            }
            std::string to_string() const override
            {
                std::stringstream sstream;
                sstream << "[";
                if (m_value.size() > 0)
                {
                    for (auto it : m_value)
                    {
                        sstream << it.to_string() << ",";
                    }
                    sstream.seekp(-1, std::ios_base::end);
                }
                sstream << "]";
                return sstream.str();
            }

            // Returns true, if no recursion is present.
            // Returns false, if current array state contains a recursion.
            bool recursion_test() { std::vector<std::shared_ptr<d_array>> vec; return recursion_test_(vec); }



            sqf::runtime::value& operator[](size_t index) { return at(index); }
            sqf::runtime::value operator[](size_t index) const { return at(index); }
            sqf::runtime::value& at(size_t index) { return m_value.at(index); }
            sqf::runtime::value at(size_t index) const { return m_value.at(index); }
            template <class T, typename TValue>
            TValue get(size_t index) const { return m_value.at(index).data<T, TValue>(); }
            template <class T, typename TValue>
            TValue get(size_t index, TValue def) const { return m_value.size() > index ? m_value.at(index).data_try<T, TValue>(def) : def; }
            size_t size() const { return m_value.size(); }
            bool empty() const { return m_value.empty(); }

            iterator begin() { return m_value.begin(); }
            iterator end() { return m_value.end(); }
            iterator erase(iterator it) { return m_value.erase(it); }
            iterator erase(iterator begin, iterator end) { return m_value.erase(begin, end); }
            template<typename TIterator>
            iterator insert(iterator start, TIterator begin, TIterator end) { return m_value.insert(start, begin, end); }

            //#TODO emplace back
            bool push_back(sqf::runtime::value val) { m_value.push_back(std::move(val)); if (!recursion_test()) { m_value.pop_back(); return false; } return true; }
            sqf::runtime::value pop_back() { auto back = m_value.back(); m_value.pop_back(); return back; }

            void reverse() { std::reverse(m_value.begin(), m_value.end()); }
            void resize(size_t newsize)
            {
                auto cursize = m_value.size();
                m_value.resize(newsize);
                if (newsize > cursize)
                {
                    for (; cursize < newsize; cursize++)
                    {
                        m_value[cursize] = sqf::runtime::value();
                    }
                }
            }
            sqf::runtime::type type() const override { return data_type(); }
            virtual std::size_t hash() const override
            {
                size_t hash = 0x9e3779b9;
                for (auto& it : m_value)
                {
                    hash ^= std::hash<sqf::runtime::value>()(it) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
                }
                return hash;
            }

            std::vector<sqf::runtime::value> value() const { return m_value; }
            std::vector<sqf::runtime::value>& value() { return m_value; }

            operator std::vector<sqf::runtime::value>()
            {
                std::vector<sqf::runtime::value> vec = m_value;
                return vec;
            }
            operator sqf::runtime::vec3()
            {
                sqf::runtime::vec3 v;
                if (size() > 0) { v.x = at(0).data_try<d_scalar, float>(0); }
                if (size() > 1) { v.y = at(1).data_try<d_scalar, float>(0); }
                if (size() > 2) { v.z = at(2).data_try<d_scalar, float>(0); }
                return v;
            }

        private:
            bool check_type(sqf::runtime::runtime&, const sqf::runtime::type*, size_t, size_t) const;
        public:
            bool check_type(sqf::runtime::runtime& runtime, sqf::runtime::type t, size_t len) const { return check_type(runtime, t, len, len); }
            bool check_type(sqf::runtime::runtime& runtime, sqf::runtime::type t, size_t min, size_t max) const;
            template<size_t size>
            bool check_type(sqf::runtime::runtime& runtime, const std::array<sqf::runtime::type, size>& arr) const { return check_type(runtime, arr.data(), size, size); }
            bool check_type(sqf::runtime::runtime& runtime, const std::vector<sqf::runtime::type>& vec) const { return check_type(runtime, vec.data(), vec.size(), vec.size()); }
            template<size_t size>
            bool check_type(sqf::runtime::runtime& runtime, const std::array<sqf::runtime::type, size>& arr, size_t optionalstart) const { return check_type(runtime, arr.data(), optionalstart, size); }
        };
        template<>
        inline std::shared_ptr<sqf::runtime::data> to_data<std::vector<sqf::runtime::value>>(std::vector<sqf::runtime::value> arr)
        {
            return std::make_shared<d_array>(arr);
        }

        float distance3dsqr(const std::shared_ptr<sqf::types::d_array>& l, const std::shared_ptr<sqf::types::d_array>& r);
        float distance3dsqr(const d_array* l, const d_array* r);
        inline float distance3dsqr(const ::sqf::runtime::vec3 l, const ::sqf::runtime::vec3 r)
        { return (float)(std::pow(l.x - r.x, 2) + std::pow(l.y - r.y, 2) + std::pow(l.z - r.z, 2)); }
        inline float distance3dsqr(std::array<float, 3> l, std::array<float, 3> r)
        { return (float)(std::pow(l[0] - r[0], 2) + std::pow(l[1] - r[1], 2) + std::pow(l[2] - r[2], 2)); }

        float distance3d(const std::shared_ptr<d_array>& l, const std::shared_ptr<d_array>& r);
        float distance3d(const d_array* l, const d_array* r);
        inline float distance3d(const ::sqf::runtime::vec3 l, const ::sqf::runtime::vec3 r)
        { return std::sqrt(distance3dsqr(l, r)); }
        inline float distance3d(std::array<float, 3> l, std::array<float, 3> r)
        { return std::sqrt(distance3dsqr(l, r)); }

        float distance2dsqr(const std::shared_ptr<d_array>& l, const std::shared_ptr<d_array>& r);
        float distance2dsqr(const d_array* l, const d_array* r);
        inline float distance2dsqr(const ::sqf::runtime::vec3 l, const ::sqf::runtime::vec3 r) 
        { return (float)(std::pow(l.x - r.x, 2) + std::pow(l.y - r.y, 2)); }
        inline float distance2dsqr(std::array<float, 2> l, std::array<float, 2> r)
        { return (float)(std::pow(l[0] - r[0], 2) + std::pow(l[1] - r[1], 2)); }

        float distance2d(const std::shared_ptr<d_array>& l, const std::shared_ptr<d_array>& r);
        float distance2d(const d_array* l, const d_array* r);
        inline float distance2d(const ::sqf::runtime::vec3 l, const ::sqf::runtime::vec3 r)
        { return std::sqrt(distance2dsqr(l, r)); }
        inline float distance2d(std::array<float, 2> l, std::array<float, 2> r)
        { return std::sqrt(distance2dsqr(l, r)); }



#define SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER(SIZE)\
            template<>inline std::shared_ptr<sqf::runtime::data>to_data<std::array<sqf::runtime::value,SIZE>>(std::array<sqf::runtime::value,SIZE>arr)\
            {return std::make_shared<d_array>(arr.begin(),arr.end());}
#define SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_10(SIZEX10)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER((SIZEX10) + 0)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER((SIZEX10) + 1)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER((SIZEX10) + 2)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER((SIZEX10) + 3)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER((SIZEX10) + 4)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER((SIZEX10) + 5)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER((SIZEX10) + 6)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER((SIZEX10) + 7)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER((SIZEX10) + 8)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER((SIZEX10) + 9)
#define SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_100(SIZEX100)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_10((SIZEX100) + 00)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_10((SIZEX100) + 10)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_10((SIZEX100) + 20)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_10((SIZEX100) + 30)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_10((SIZEX100) + 40)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_10((SIZEX100) + 50)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_10((SIZEX100) + 60)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_10((SIZEX100) + 70)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_10((SIZEX100) + 80)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_10((SIZEX100) + 90)
#define SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_1000(SIZEX100)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_100((SIZEX100) + 000)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_100((SIZEX100) + 100)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_100((SIZEX100) + 200)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_100((SIZEX100) + 300)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_100((SIZEX100) + 400)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_100((SIZEX100) + 500)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_100((SIZEX100) + 600)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_100((SIZEX100) + 700)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_100((SIZEX100) + 800)\
            SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_100((SIZEX100) + 900)

        SQF_RUNTIME_D_ARRAY_EXPANDO_TO_DATA_HELPER_1000(0)
    }
}
