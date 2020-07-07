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

		sqf::value get_text_sqf() const;
		std::string get_text() const { return mtext; };
		void set_text(std::string val) { mtext = val; };

		sqf::value get_type_sqf() const;
		std::string get_type() const { return mtype; };
		void set_type(std::string val) { mtype = val; };

		sqf::value get_brush_sqf() const;
		std::string get_brush() const { return mbrush; };
		void set_brush(std::string val) { mbrush = val; };

		sqf::value get_color_sqf() const;
		std::string get_color() const { return mcolor; };
		void set_color(std::string val) { mcolor = val; };

		sqf::value get_size_sqf() const;
		std::array<float, 2> get_size() const { return msize; };
		void set_size(std::array<float, 2> val) { msize = val; };

		sqf::value get_pos_sqf() const;
		std::array<float, 3> get_pos() const { return mpos; };
		void set_pos(std::array<float, 3> val) { mpos = val; };

		sqf::value get_alpha_sqf() const;
		float get_alpha() const { return malpha; };
		void set_alpha(float val) { malpha = val; };

		sqf::value get_direction_sqf() const;
		float get_direction() const { return mdirection; };
		void set_direction(float val) { mdirection = val; };

		sqf::value get_shape_sqf() const;
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
}