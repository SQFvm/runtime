#pragma once

#include <string>
#include <array>
#include <memory>

namespace sqf
{
	class value;
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

		std::shared_ptr<sqf::value> get_text_sqf() const;
		inline std::string get_text() const { return mtext; };
		inline void set_text(std::string val) { mtext = val; };

		std::shared_ptr<sqf::value> get_type_sqf() const;
		inline std::string get_type() const { return mtype; };
		inline void set_type(std::string val) { mtype = val; };

		std::shared_ptr<sqf::value> get_brush_sqf() const;
		inline std::string get_brush() const { return mbrush; };
		inline void set_brush(std::string val) { mbrush = val; };

		std::shared_ptr<sqf::value> get_color_sqf() const;
		inline std::string get_color() const { return mcolor; };
		inline void set_color(std::string val) { mcolor = val; };

		std::shared_ptr<sqf::value> get_size_sqf() const;
		inline std::array<float, 2> get_size() const { return msize; };
		inline void set_size(std::array<float, 2> val) { msize = val; };

		std::shared_ptr<sqf::value> get_pos_sqf() const;
		inline std::array<float, 3> get_pos() const { return mpos; };
		inline void set_pos(std::array<float, 3> val) { mpos = val; };

		std::shared_ptr<sqf::value> get_alpha_sqf() const;
		inline float get_alpha() const { return malpha; };
		inline void set_alpha(float val) { malpha = val; };

		std::shared_ptr<sqf::value> get_direction_sqf() const;
		inline float get_direction() const { return mdirection; };
		inline void set_direction(float val) { mdirection = val; };

		std::shared_ptr<sqf::value> get_shape_sqf() const;
		inline shape get_shape() const { return mshape; };
		inline std::string get_shape_string() const
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
		inline void set_shape(shape val) { mshape = val; };
	};
}