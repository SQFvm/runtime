#include "ops_math.h"
#include "../runtime/value.h"
#include "../runtime/logging.h"
#include "../runtime/runtime.h"
#include "../runtime/sqfop.h"
#include "../runtime/util.h"

#include "../runtime/d_string.h"
#include "../runtime/d_scalar.h"
#include "../runtime/d_boolean.h"
#include "../runtime/d_array.h"

#include <cmath>
#include <random>
#include <array>
#include <iomanip>


namespace err = logmessage::runtime;
using namespace sqf::runtime;
using namespace sqf::types;

namespace
{
	value plus_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		return (left.data<d_scalar, float>()) + (right.data<d_scalar, float>());
	}
	value minus_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		return (left.data<d_scalar, float>()) - (right.data<d_scalar, float>());
	}
	value multiply_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		return (left.data<d_scalar, float>()) * (right.data<d_scalar, float>());
	}
	value divide_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto r = (right.data<d_scalar, float>());
		if (r == 0)
		{
			runtime.__logmsg(err::ZeroDivisor((*runtime.active_context().current_frame().current())->diag_info()));
			return 0;
		}
		return (left.data<d_scalar, float>()) / r;
	}
	value abs_scalar(runtime& runtime, value::cref right)
	{
		auto r = right.data<d_scalar, float>();
		return std::fabs(r);
	}
	value atan2_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		return std::atan2(left.data<d_scalar, float>(), right.data<d_scalar, float>());
	}
	value deg_scalar(runtime& runtime, value::cref right)
	{
		return right.data<d_scalar, float>() * (180 / util::pi());
	}
    value log_scalar(runtime& runtime, value::cref right)
	{
		return std::log10(right.data<d_scalar, float>());
	}
	value pi_(runtime& runtime)
	{
		return util::pi();
	}
	value sin_scalar(runtime& runtime, value::cref right)
	{
		return std::sin(right.data<d_scalar, float>());
	}
	value acos_scalar(runtime& runtime, value::cref right)
	{
		return std::acos(right.data<d_scalar, float>());
	}
	value exp_scalar(runtime& runtime, value::cref right)
	{
		return std::exp(right.data<d_scalar, float>());
	}
    value rad_scalar(runtime& runtime, value::cref right)
	{
		return right.data<d_scalar, float>() * (util::pi() / 180);
	}
	value sqrt_scalar(runtime& runtime, value::cref right)
	{
		return std::sqrt(right.data<d_scalar, float>());
	}
	value tan_scalar(runtime& runtime, value::cref right)
	{
		return std::tan(right.data<d_scalar, float>());
	}
    value random_scalar(runtime& runtime, value::cref right)
	{
		return (static_cast<float>(std::rand()) / RAND_MAX) * right.data<d_scalar, float>();
	}
	value min_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_scalar, float>();
		auto r = right.data<d_scalar, float>();
		return std::fmin(l, r);
	}
	value max_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_scalar, float>();
		auto r = right.data<d_scalar, float>();
		return std::fmax(l, r);
	}
	value floor_scalar(runtime& runtime, value::cref right)
	{
		auto r = right.data<d_scalar, float>();
		return std::floor(r);
	}
	value ceil_scalar(runtime& runtime, value::cref right)
	{
		auto r = right.data<d_scalar, float>();
		return std::ceil(r);
	}
	value asin_scalar(runtime& runtime, value::cref right)
	{
		auto r = right.data<d_scalar, float>();
		return std::asin(r);
	}
	value atan_scalar(runtime& runtime, value::cref right)
	{
		auto r = right.data<d_scalar, float>();
		return std::atan(r);
	}
	value cos_scalar(runtime& runtime, value::cref right)
	{
		auto r = right.data<d_scalar, float>();
		return std::cos(r);
	}
    value ln_scalar(runtime& runtime, value::cref right)
	{
		auto r = right.data<d_scalar, float>();
		return std::log(r);
	}
	value mod_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_scalar, float>();
		auto r = right.data<d_scalar, float>();
		return std::fmod(l, r);
	}
    value round_scalar(runtime& runtime, value::cref right)
	{
		auto r = right.data<d_scalar, float>();
		return std::round(r);
	}
	value minus_scalar(runtime& runtime, value::cref right)
	{
		auto r = right.data<d_scalar, float>();
		return -r;
	}
	value exclamationmark_bool(runtime& runtime, value::cref right)
	{
		auto f = right.data<d_boolean, bool>();
		return !f;
	}
    value raisetopower_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_scalar, float>();
		auto r = right.data<d_scalar, float>();
		return std::pow(l, r);
	}
	value plus_scalar(runtime& runtime, value::cref right)
	{
		return right;
	}
	float dotProduct(std::array<float, 3> left, std::array<float, 3> right)
	{
		return left[0] * right[0] + left[1] * right[1] + left[2] * right[2];
	}
	float vectorMagnitudeSqr(std::array<float, 3> arr)
	{
		return std::pow(arr[0], 2) + std::pow(arr[1], 2) + std::pow(arr[2], 2);
	}
	float vectorMagnitude(std::array<float, 3> arr)
	{
		return std::sqrt(vectorMagnitudeSqr(arr));
	}
	float vectorDistanceSqr(std::array<float, 3> left, std::array<float, 3> right)
	{
		return std::pow(left[0] - right[0], 2) + std::pow(left[1] - right[1], 2) + std::pow(left[2] - right[2], 2);
	}
	float vectorDistance(std::array<float, 3> left, std::array<float, 3> right)
	{
		return std::sqrt(vectorDistanceSqr(left, right));
	}
	value vectoradd_array_array(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_array>();
		auto r = right.data<d_array>();
		auto arr = std::make_shared<d_array>();
		if (!l->check_type(runtime, t_scalar(), 3) || !r->check_type(runtime, t_scalar(), 3))
		{
			return {};
		}
		arr->push_back(l->at(0).data<d_scalar, float>() + r->at(0).data<d_scalar, float>());
		arr->push_back(l->at(1).data<d_scalar, float>() + r->at(1).data<d_scalar, float>());
		arr->push_back(l->at(2).data<d_scalar, float>() + r->at(2).data<d_scalar, float>());
		return value(arr);
	}
	value vectorcos_array_array(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_array>();
		auto r = right.data<d_array>();
		auto arr = std::make_shared<d_array>();
		if (!l->check_type(runtime, t_scalar(), 3) || !r->check_type(runtime, t_scalar(), 3))
		{
			return {};
		}
		return dotProduct(*l, *r) / (vectorMagnitude(*l) * vectorMagnitude(*r));
	}
	value vectorcrossproduct_array_array(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_array>();
		auto r = right.data<d_array>();
		auto arr = std::make_shared<d_array>();
		if (!l->check_type(runtime, t_scalar(), 3) || !r->check_type(runtime, t_scalar(), 3))
		{
			return {};
		}
		arr->push_back(l->at(1).data<d_scalar, float>() * r->at(2).data<d_scalar, float>() - l->at(2).data<d_scalar, float>() * r->at(1).data<d_scalar, float>());
		arr->push_back(l->at(2).data<d_scalar, float>() * r->at(0).data<d_scalar, float>() - l->at(0).data<d_scalar, float>() * r->at(2).data<d_scalar, float>());
		arr->push_back(l->at(0).data<d_scalar, float>() * r->at(1).data<d_scalar, float>() - l->at(1).data<d_scalar, float>() * r->at(0).data<d_scalar, float>());
		return value(arr);
	}
	value vectordistance_array_array(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_array>();
		auto r = right.data<d_array>();
		auto arr = std::make_shared<d_array>();
		if (!l->check_type(runtime, t_scalar(), 3) || !r->check_type(runtime, t_scalar(), 3))
		{
			return {};
		}
		return vectorDistance(*l, *r);
	}
	value vectordistancesqr_array_array(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_array>();
		auto r = right.data<d_array>();
		auto arr = std::make_shared<d_array>();
		if (!l->check_type(runtime, t_scalar(), 3) || !r->check_type(runtime, t_scalar(), 3))
		{
			return {};
		}
		return vectorDistanceSqr(*l, *r);
	}
	value vectormultiply_array_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_array>();
		auto r = right.data<d_scalar, float>();
		auto arr = std::make_shared<d_array>();
		if (!l->check_type(runtime, t_scalar(), 3))
		{
			return {};
		}
		arr->push_back(l->at(0).data<d_scalar, float>() * r);
		arr->push_back(l->at(1).data<d_scalar, float>() * r);
		arr->push_back(l->at(2).data<d_scalar, float>() * r);
		return value(arr);
	}
	value vectordiff_array_array(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_array>();
		auto r = right.data<d_array>();
		auto arr = std::make_shared<d_array>();
		if (!l->check_type(runtime, t_scalar(), 3) || !r->check_type(runtime, t_scalar(), 3))
		{
			return {};
		}
		arr->push_back(l->at(0).data<d_scalar, float>() - r->at(0).data<d_scalar, float>());
		arr->push_back(l->at(1).data<d_scalar, float>() - r->at(1).data<d_scalar, float>());
		arr->push_back(l->at(2).data<d_scalar, float>() - r->at(2).data<d_scalar, float>());
		return value(arr);
	}
	value vectordotproduct_array_array(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_array>();
		auto r = right.data<d_array>();
		if (!l->check_type(runtime, t_scalar(), 3) || !r->check_type(runtime, t_scalar(), 3))
		{
			return {};
		}
		return dotProduct(*l, *r);
	}
	value vectormagnitude_array(runtime& runtime, value::cref left)
	{
		auto l = left.data<d_array>();
		if (!l->check_type(runtime, t_scalar(), 3))
		{
			return {};
		}
		return vectorMagnitude(*l);
	}
	value vectormagnitudesqr_array(runtime& runtime, value::cref left)
	{
		auto l = left.data<d_array>();
		if (!l->check_type(runtime, t_scalar(), 3))
		{
			return {};
		}
		return vectorMagnitudeSqr(*l);
	}
	value vectornormalized_array(runtime& runtime, value::cref left)
	{
		auto l = left.data<d_array>();
		auto arr = std::make_shared<d_array>();
		if (!l->check_type(runtime, t_scalar(), 3))
		{
			return {};
		}
		auto magnitude = vectorMagnitude(*l);
		if (magnitude == 0)
		{
			arr->push_back(0);
			arr->push_back(0);
			arr->push_back(0);
		}
		else
		{
			arr->push_back(l->at(1).data<d_scalar, float>() / magnitude);
			arr->push_back(l->at(2).data<d_scalar, float>() / magnitude);
			arr->push_back(l->at(0).data<d_scalar, float>() / magnitude);
		}
		return value(arr);
	}
	value tofixed_scalar(runtime& runtime, value::cref right)
	{
		auto i = right.data<d_scalar, int>();
		if (i > 20)
		{
			i = 20;
		}
		else if (i < 0)
		{
			i = -1;
		}
		sqf::types::d_scalar::set_decimals(i);
		return {};
	}
	value tofixed_scalar_scalar(runtime& runtime, value::cref left, value::cref right)
	{
		auto i = right.data<d_scalar, int>();
		if (i > 20)
		{
			i = 20;
		}
		else if (i <= 0)
		{
			i = 0;
		}
		std::stringstream sstream;
		sstream << std::fixed << std::setprecision(i) << left.data<d_scalar, float>();
        return sstream.str();
	}
}

void sqf::operators::ops_math(sqf::runtime::runtime& runtime)
{
	using namespace sqf::runtime::sqfop;

	runtime.register_sqfop(unary("abs", t_scalar(), "Absolute value of a real number", abs_scalar));
	runtime.register_sqfop(binary(7, "atan2", t_scalar(), t_scalar(), "ArcTangent of x/y. Used to determine the angle of a vector [x,y]. Result in Degrees between -180 and 180. Note that this command can handle y being 0, unlike when using atan, and will return 90", atan2_scalar_scalar));
	runtime.register_sqfop(unary("deg", t_scalar(), "Convert a number from Radians to Degrees.", deg_scalar));
	runtime.register_sqfop(unary("log", t_scalar(), "Base-10 logarithm of x.", log_scalar));
	runtime.register_sqfop(nular("pi", "The value of PI.", pi_));
	runtime.register_sqfop(unary("sin", t_scalar(), "Sine of x, argument in Degrees.", sin_scalar));
	runtime.register_sqfop(unary("acos", t_scalar(), "ArcCosine of a number, result in Degrees.", acos_scalar));
	runtime.register_sqfop(unary("exp", t_scalar(), "Let x be a number, then exp (x) is equal to e to the power of x (or e^x).", exp_scalar));
	runtime.register_sqfop(unary("rad", t_scalar(), "Convert x from Degrees to Radians. 360 degrees is equal to 2 multiplied with pi.", rad_scalar));
	runtime.register_sqfop(unary("sqrt", t_scalar(), "Returns square root of x.", sqrt_scalar));
	runtime.register_sqfop(unary("tan", t_scalar(), "Tangent of x, argument in Degrees.", tan_scalar));
	runtime.register_sqfop(unary("random", t_scalar(), "Random real (floating point) value from 0 (inclusive) to x (not inclusive).", random_scalar));
	runtime.register_sqfop(unary("-", t_scalar(), "Zero minus a.", minus_scalar));
	runtime.register_sqfop(unary("+", t_scalar(), "Returns a copy of a.", plus_scalar));
	runtime.register_sqfop(unary("+", t_nan(), "Returns a copy of a.", plus_scalar));
	runtime.register_sqfop(unary("!", t_boolean(), "Returns a negation of Boolean expression. That means true becomes false and vice versa.", exclamationmark_bool));
	runtime.register_sqfop(binary(6, "min", t_scalar(), t_scalar(), "The smaller of a, b", min_scalar_scalar));
	runtime.register_sqfop(binary(6, "max", t_scalar(), t_scalar(), "The greater of a, b.", max_scalar_scalar));
	runtime.register_sqfop(unary("floor", t_scalar(), "Returns the next lowest integer in relation to x.", floor_scalar));
	runtime.register_sqfop(unary("ceil", t_scalar(), "The ceil value of x.", ceil_scalar));
	runtime.register_sqfop(unary("asin", t_scalar(), "Arcsine of a number, result in Degrees.", asin_scalar));
	runtime.register_sqfop(unary("atan", t_scalar(), "ArcTangent of a number, result in Degrees.", atan_scalar));
	runtime.register_sqfop(unary("atg", t_scalar(), "Equivalent to atan.", atan_scalar));
	runtime.register_sqfop(unary("cos", t_scalar(), "Cosine of a number, argument in degrees.", cos_scalar));
	runtime.register_sqfop(unary("ln", t_scalar(), "Natural logarithm of x.", ln_scalar));
	runtime.register_sqfop(unary("round", t_scalar(), "Rounds up or down to the closest integer.", round_scalar));
	runtime.register_sqfop(binary(7, "mod", t_scalar(), t_scalar(), "Remainder of a divided by b.", mod_scalar_scalar));
	runtime.register_sqfop(binary(7, "%", t_scalar(), t_scalar(), "Remainder of a divided by b.", mod_scalar_scalar));
	runtime.register_sqfop(binary(9, "^", t_scalar(), t_scalar(), "a raised to the power of b.", raisetopower_scalar_scalar));
	runtime.register_sqfop(binary(6, "+", t_scalar(), t_scalar(), "b added to a.", plus_scalar_scalar));
	runtime.register_sqfop(binary(6, "-", t_scalar(), t_scalar(), "Subtracts b from a.", minus_scalar_scalar));
	runtime.register_sqfop(binary(7, "*", t_scalar(), t_scalar(), "Returns the value of a multiplied by b.", multiply_scalar_scalar));
	runtime.register_sqfop(binary(7, "/", t_scalar(), t_scalar(), "a divided by b. Division by 0 throws \"Division by zero\" error, however script doesn't stop and the result of such division is assumed to be 0.", divide_scalar_scalar));

	runtime.register_sqfop(binary(4, "vectorAdd", t_array(), t_array(), "Adds two 3D vectors.", vectoradd_array_array));
	runtime.register_sqfop(binary(4, "vectorCos", t_array(), t_array(), "Cosine of angle between two 3D vectors.", vectorcos_array_array));
	runtime.register_sqfop(binary(4, "vectorCrossProduct", t_array(), t_array(), "Cross product of two 3D vectors.", vectorcrossproduct_array_array));
	runtime.register_sqfop(binary(4, "vectorDiff", t_array(), t_array(), "Subtracts one 3D vector from another.", vectordiff_array_array));
	runtime.register_sqfop(binary(4, "vectorDistance", t_array(), t_array(), "Distance between two 3D vectors.", vectordistance_array_array));
	runtime.register_sqfop(binary(4, "vectorDistanceSqr", t_array(), t_array(), "Squared distance between two 3D vectors.", vectordistancesqr_array_array));
	runtime.register_sqfop(binary(4, "vectorDotProduct", t_array(), t_array(), "Dot product of two 3D vectors.", vectordotproduct_array_array));
	runtime.register_sqfop(unary("vectorMagnitude", t_array(), "Magnitude of a 3D vector.", vectormagnitude_array));
	runtime.register_sqfop(unary("vectorMagnitudeSqr", t_array(), "Squared magnitude of a 3D vector.", vectormagnitudesqr_array));
	runtime.register_sqfop(binary(4, "vectorMultiply", t_array(), t_scalar(), "Multiplies 3D vector by a t_scalar().", vectormultiply_array_scalar));
	runtime.register_sqfop(unary("vectorNormalized", t_array(), "Returns normalized vector (unit vector, vectorMagnitude = 1) of given vector. If given vector is 0 result is a 0 vector as well.", vectornormalized_array));
	runtime.register_sqfop(binary(4, "toFixed", t_scalar(), t_scalar(), "Converts a number into a string, keeping the specified number of decimals. Range from 0 to 20.", tofixed_scalar_scalar));
	runtime.register_sqfop(unary("toFixed", t_scalar(), "Sets how many decimals should be displayed during to string conversion. -1 resets to default. Range from 0 to 20.", tofixed_scalar));
}
