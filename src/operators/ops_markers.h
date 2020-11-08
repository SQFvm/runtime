#pragma once
#include "../runtime/data.h"
#include "../runtime/type.h"
#include "../runtime/value.h"
#include "../runtime/runtime.h"
#include "../runtime/d_array.h"

#include <string>
#include <memory>
#include <unordered_map>

namespace sqf
{

    namespace operators
    {
        class markers_storage : public sqf::runtime::runtime::datastorage
        {
        public:
            class marker
            {
            public:
                enum class shape
                {
                    NA,
                    Icon,
                    Rectangle,
                    Ellipse,
                    Polyline
                };
            private:
                std::string mtext;
                std::string mtype;
                std::string mcolor;
                std::string mbrush;
                std::array<float, 2> msize;
                std::array<float, 3> mpos;
                float malpha;
                float mdirection;
                shape mshape;

            public:
                marker();

                std::string get_text() const { return mtext; };
                void set_text(std::string val) { mtext = val; };

                std::string get_type() const { return mtype; };
                void set_type(std::string val) { mtype = val; };

                std::string get_brush() const { return mbrush; };
                void set_brush(std::string val) { mbrush = val; };

                std::string get_color() const { return mcolor; };
                void set_color(std::string val) { mcolor = val; };

                std::array<float, 2> get_size() const { return msize; };
                void set_size(std::array<float, 2> val) { msize = val; };

                std::array<float, 3> get_pos() const { return mpos; };
                void set_pos(std::array<float, 3> val) { mpos = val; };

                float get_alpha() const { return malpha; };
                void set_alpha(float val) { malpha = val; };

                float get_direction() const { return mdirection; };
                void set_direction(float val) { mdirection = val; };

                shape get_shape() const { return mshape; };
                std::string get_shape_string() const
                {
                    switch (mshape)
                    {
                    case shape::Icon:
                        return "ICON";
                    case shape::Rectangle:
                        return "RECTANGLE";
                    case shape::Ellipse:
                        return "ELLIPSE";
                    case shape::Polyline:
                        return "POLYLINE";
                    default:
                        return "NA";
                    }
                };
                void set_shape(shape val) { mshape = val; };
            };
            using iterator = std::unordered_map<std::string, marker>::iterator;
        private:
            std::unordered_map<std::string, marker> m_inner;
        public:
            markers_storage() {}
            virtual ~markers_storage() override {}

            bool exists(std::string key) { return m_inner.find(key) != m_inner.end(); }
            marker& at(std::string key) { return m_inner.at(key); }
            void set(std::string key, marker obj) { m_inner[key] = obj; }
            void erase(std::string key) { m_inner.erase(key); }

            iterator begin() { return m_inner.begin(); }
            iterator end() { return m_inner.end(); }

        };
        void ops_markers(::sqf::runtime::runtime& runtime);
    }
}