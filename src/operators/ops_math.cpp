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
	inline float rad(float f) {
		return (float)(f * util::pi() / 180.0);
	}
	inline float deg(float f) {
		return (float)(f * 180.0 / util::pi());
	}
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
		auto r_is_rero = std::abs(r) <= std::numeric_limits<float>::epsilon();
		if (r_is_rero)
		{
			runtime.__logmsg(err::ZeroDivisor(runtime.context_active().current_frame().diag_info_from_position()));
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
		return deg(std::atan2(rad(left.data<d_scalar, float>()), rad(right.data<d_scalar, float>())));
	}
	value deg_scalar(runtime& runtime, value::cref right)
	{
		return deg(right.data<d_scalar, float>());
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
		return std::sin(rad(right.data<d_scalar, float>()));
	}
	value acos_scalar(runtime& runtime, value::cref right)
	{
		return deg(std::acos(right.data<d_scalar, float>()));
	}
	value exp_scalar(runtime& runtime, value::cref right)
	{
		return std::exp(right.data<d_scalar, float>());
	}
    value rad_scalar(runtime& runtime, value::cref right)
	{
		return rad(right.data<d_scalar, float>());
	}
	value sqrt_scalar(runtime& runtime, value::cref right)
	{
		return std::sqrt(right.data<d_scalar, float>());
	}
	value tan_scalar(runtime& runtime, value::cref right)
	{
		return std::tan(rad(right.data<d_scalar, float>()));
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
		return deg(std::asin(r));
	}
	value atan_scalar(runtime& runtime, value::cref right)
	{
		auto r = right.data<d_scalar, float>();
		return deg(std::atan(r));
	}
	value cos_scalar(runtime& runtime, value::cref right)
	{
		auto r = right.data<d_scalar, float>();
		return std::cos(rad(r));
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
	value matrixmultiply_array_array(runtime& runtime, value::cref left, value::cref right)
	{
		auto l = left.data<d_array>();
		auto r = right.data<d_array>();

		// Check that neither rows nor first col is empty
		if (l->size() == 0 || l->at(0).type() != t_array() || l->at(0).data<d_array>()->size() == 0) {
			return std::make_shared<d_array>();
		}
		if (r->size() == 0 || r->at(0).type() != t_array() || r->at(0).data<d_array>()->size() == 0) {
			return std::make_shared<d_array>();
		}

		size_t l_rows = l->size();
		size_t l_cols = l->at(0).data<d_array>()->size();
		size_t r_rows = r->size();
		size_t r_cols = r->at(0).data<d_array>()->size();

		// Check that array is n x k
		for (size_t i = 1; i < l_rows; i++) {
			if (l->at(i).data<d_array>()->size() != l_cols) {
				return std::make_shared<d_array>();
			}
		}

		// Check that array is k x m
		for (size_t i = 1; i < r_rows; i++) {
			if (r->at(i).data<d_array>()->size() != r_cols) {
				return std::make_shared<d_array>();
			}
		}

		// Ensure arrays are n x k and k x m
		if (l_cols != r_rows) {
			return std::make_shared<d_array>();
		}

		auto multiplied = std::make_shared<d_array>();

		for (size_t i = 0; i < l_rows; i++) {
			auto row = std::make_shared<d_array>();
			for (size_t j = 0; j < r_cols; j++) {
				float col = 0;
				for (size_t k = 0; k < r_rows; k++) {
					auto l_value = l->at(i).data<d_array>()->at(k).data<d_scalar, float>();
					auto r_value = r->at(k).data<d_array>()->at(j).data<d_scalar, float>();
					col = col + l_value * r_value;
				}
				row->push_back(col);
			}
			multiplied->push_back(row);
		}

		return multiplied;
	}
	value matrixtranspose_array(runtime& runtime, value::cref left)
	{
		auto l = left.data<d_array>();

		// Check that neither rows nor first col is empty
		if (l->size() == 0 || l->at(0).type() != t_array() || l->at(0).data<d_array>()->size() == 0) {
			return std::make_shared<d_array>();
		}

		auto row_size = l->size();
		auto col_size = l->at(0).data<d_array>()->size();

		// Check that array is n x k
		for (size_t i = 1; i < row_size; i++) {
			if (l->at(i).data<d_array>()->size() != col_size) {
				return std::make_shared<d_array>();
			}
		}

		auto transposed = std::make_shared<d_array>();

		for (size_t i = 0; i < col_size; i++) {
			auto row = std::make_shared<d_array>();
			for (size_t j = 0; j < row_size; j++) {
				row->push_back(l->at(j).data<d_array>()->at(i).data<d_scalar, float>());
			}
			transposed->push_back(row);
		}

		return transposed;
	}
	float dotProduct(vec3 left, vec3 right)
	{
		return left.x * right.x + left.y * right.y + left.z * right.z;
	}
	float vectorMagnitudeSqr(vec3 arr)
	{
		return (float)(std::pow(arr.x, 2) + std::pow(arr.y, 2) + std::pow(arr.z, 2));
	}
	float vectorMagnitude(vec3 arr)
	{
		return std::sqrt(vectorMagnitudeSqr(arr));
	}
	float vectorDistanceSqr(vec3 left, vec3 right)
	{
		return (float)(std::pow(left.x - right.x, 2) + std::pow(left.y - right.y, 2) + std::pow(left.z - right.z, 2));
	}
	float vectorDistance(vec3 left, vec3 right)
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
			arr->push_back(l->at(0).data<d_scalar, float>() / magnitude);
			arr->push_back(l->at(1).data<d_scalar, float>() / magnitude);
			arr->push_back(l->at(2).data<d_scalar, float>() / magnitude);
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
	runtime.register_sqfop(unary("not", t_boolean(), "Returns a negation of Boolean expression. That means true becomes false and vice versa.", exclamationmark_bool));
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

	runtime.register_sqfop(binary(4, "matrixMultiply", t_array(), t_array(), "", matrixmultiply_array_array));
	runtime.register_sqfop(unary("matrixTranspose", t_array(), "", matrixtranspose_array));
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
