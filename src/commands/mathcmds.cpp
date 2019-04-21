#include "../commandmap.h"
#include "../value.h"
#include "../cmd.h"
#include "../virtualmachine.h"
#include "../compiletime.h"
#include "../arraydata.h"
#include "../scalardata.h"
#include <cmath>
#include <random>
#include <array>
#include <iomanip>

using namespace sqf;
namespace
{
	value plus_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		return (left.as_double()) + (right.as_double());
	}
	value minus_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		return (left.as_double()) - (right.as_double());
	}
	value multiply_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		return (left.as_double()) * (right.as_double());
	}
	value divide_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto r = (right.as_double());
		if (r == 0)
		{
			vm->wrn() << "Zero Divisor" << std::endl;
			return 0;
		}
		return (left.as_double()) / r;
	}
	value abs_scalar(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_double();
		return std::fabs(r);
	}
	value atan2_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		return std::atan2(left.as_double(), right.as_double());
	}
	value deg_scalar(virtualmachine* vm, value::cref right)
	{
		return right.as_double() * (180 / compiletime::pi());
	}
    value log_scalar(virtualmachine* vm, value::cref right)
	{
		return std::log10(right.as_double());
	}
	value pi_(virtualmachine* vm)
	{
		return compiletime::pi();
	}
	value sin_scalar(virtualmachine* vm, value::cref right)
	{
		return std::sin(right.as_double());
	}
	value acos_scalar(virtualmachine* vm, value::cref right)
	{
		return std::acos(right.as_double());
	}
	value exp_scalar(virtualmachine* vm, value::cref right)
	{
		return std::exp(right.as_double());
	}
    value rad_scalar(virtualmachine* vm, value::cref right)
	{
		return right.as_double() * (compiletime::pi() / 180);
	}
	value sqrt_scalar(virtualmachine* vm, value::cref right)
	{
		return std::sqrt(right.as_double());
	}
	value tan_scalar(virtualmachine* vm, value::cref right)
	{
		return std::tan(right.as_double());
	}
    value random_scalar(virtualmachine* vm, value::cref right)
	{
		return compiletime::rand_lim(std::rand(), right.as_double());
	}
	value min_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_double();
		auto r = right.as_double();
		return std::fmin(l, r);
	}
	value max_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_double();
		auto r = right.as_double();
		return std::fmax(l, r);
	}
	value floor_scalar(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_double();
		return std::floor(r);
	}
	value ceil_scalar(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_double();
		return std::ceil(r);
	}
	value asin_scalar(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_double();
		return std::asin(r);
	}
	value atan_scalar(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_double();
		return std::atan(r);
	}
	value cos_scalar(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_double();
		return std::cos(r);
	}
    value ln_scalar(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_double();
		return std::log(r);
	}
	value mod_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_double();
		auto r = right.as_double();
		return std::fmod(l, r);
	}
    value round_scalar(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_double();
		return std::round(r);
	}
	value minus_scalar(virtualmachine* vm, value::cref right)
	{
		auto r = right.as_double();
		return -r;
	}
	value exclamationmark_bool(virtualmachine* vm, value::cref right)
	{
		auto f = right.as_bool();
		return !f;
	}
    value raisetopower_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.as_double();
		auto r = right.as_double();
		return std::pow(l, r);
	}
	value plus_scalar(virtualmachine* vm, value::cref right)
	{
		return right;
	}
	double dotProduct(std::array<double, 3> left, std::array<double, 3> right)
	{
		return left[0] * right[0] + left[1] * right[1] + left[2] * right[2];
	}
	double vectorMagnitudeSqr(std::array<double, 3> arr)
	{
		return std::pow(arr[0], 2) + std::pow(arr[1], 2) + std::pow(arr[2], 2);
	}
	double vectorMagnitude(std::array<double, 3> arr)
	{
		return std::sqrt(vectorMagnitudeSqr(arr));
	}
	double vectorDistanceSqr(std::array<double, 3> left, std::array<double, 3> right)
	{
		return std::pow(left[0] - right[0], 2) + std::pow(left[1] - right[1], 2) + std::pow(left[2] - right[2], 2);
	}
	double vectorDistance(std::array<double, 3> left, std::array<double, 3> right)
	{
		return std::sqrt(vectorDistanceSqr(left, right));
	}
	value vectoradd_array_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.data<arraydata>();
		auto arr = std::make_shared<arraydata>();
		if (!l->check_type(vm, SCALAR, 3) || !r->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		arr->push_back(l->at(0).as_double() + r->at(0).as_double());
		arr->push_back(l->at(1).as_double() + r->at(1).as_double());
		arr->push_back(l->at(2).as_double() + r->at(2).as_double());
		return value(arr);
	}
	value vectorcos_array_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.data<arraydata>();
		auto arr = std::make_shared<arraydata>();
		if (!l->check_type(vm, SCALAR, 3) || !r->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		return dotProduct(*l, *r) / (vectorMagnitude(*l) * vectorMagnitude(*r));
	}
	value vectorcrossproduct_array_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.data<arraydata>();
		auto arr = std::make_shared<arraydata>();
		if (!l->check_type(vm, SCALAR, 3) || !r->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		arr->push_back(l->at(1).as_double() * r->at(2).as_double() - l->at(2).as_double() * r->at(1).as_double());
		arr->push_back(l->at(2).as_double() * r->at(0).as_double() - l->at(0).as_double() * r->at(2).as_double());
		arr->push_back(l->at(0).as_double() * r->at(1).as_double() - l->at(1).as_double() * r->at(0).as_double());
		return value(arr);
	}
	value vectordistance_array_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.data<arraydata>();
		auto arr = std::make_shared<arraydata>();
		if (!l->check_type(vm, SCALAR, 3) || !r->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		return vectorDistance(*l, *r);
	}
	value vectordistancesqr_array_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.data<arraydata>();
		auto arr = std::make_shared<arraydata>();
		if (!l->check_type(vm, SCALAR, 3) || !r->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		return vectorDistanceSqr(*l, *r);
	}
	value vectormultiply_array_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.as_double();
		auto arr = std::make_shared<arraydata>();
		if (!l->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		arr->push_back(l->at(0).as_double() * r);
		arr->push_back(l->at(1).as_double() * r);
		arr->push_back(l->at(2).as_double() * r);
		return value(arr);
	}
	value vectordiff_array_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.data<arraydata>();
		auto arr = std::make_shared<arraydata>();
		if (!l->check_type(vm, SCALAR, 3) || !r->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		arr->push_back(l->at(0).as_double() - r->at(0).as_double());
		arr->push_back(l->at(1).as_double() - r->at(1).as_double());
		arr->push_back(l->at(2).as_double() - r->at(2).as_double());
		return value(arr);
	}
	value vectordotproduct_array_array(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto l = left.data<arraydata>();
		auto r = right.data<arraydata>();
		if (!l->check_type(vm, SCALAR, 3) || !r->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		return dotProduct(*l, *r);
	}
	value vectormagnitude_array(virtualmachine* vm, value::cref left)
	{
		auto l = left.data<arraydata>();
		if (!l->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		return vectorMagnitude(*l);
	}
	value vectormagnitudesqr_array(virtualmachine* vm, value::cref left)
	{
		auto l = left.data<arraydata>();
		if (!l->check_type(vm, SCALAR, 3))
		{
			return {};
		}
		return vectorMagnitudeSqr(*l);
	}
	value vectornormalized_array(virtualmachine* vm, value::cref left)
	{
		auto l = left.data<arraydata>();
		auto arr = std::make_shared<arraydata>();
		if (!l->check_type(vm, SCALAR, 3))
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
			arr->push_back(l->at(1).as_double() / magnitude);
			arr->push_back(l->at(2).as_double() / magnitude);
			arr->push_back(l->at(0).as_double() / magnitude);
		}
		return value(arr);
	}
	value tofixed_scalar(virtualmachine* vm, value::cref right)
	{
		auto i = right.as_int();
		if (i > 20)
		{
			i = 20;
		}
		else if (i < 0)
		{
			i = -1;
		}
		sqf::scalardata::setdecimals(i);
		return {};
	}
	value tofixed_scalar_scalar(virtualmachine* vm, value::cref left, value::cref right)
	{
		auto i = right.as_int();
		if (i > 20)
		{
			i = 20;
		}
		else if (i <= 0)
		{
			i = 0;
		}
		std::stringstream sstream;
		sstream << std::fixed << std::setprecision(i) << left.as_float();
        return sstream.str();
	}
}

void sqf::commandmap::initmathcmds()
{
	add(unary("abs", sqf::type::SCALAR, "Absolute value of a real number", abs_scalar));
	add(binary(7, "atan2", sqf::type::SCALAR, sqf::type::SCALAR, "ArcTangent of x/y. Used to determine the angle of a vector [x,y]. Result in Degrees between -180 and 180. Note that this command can handle y being 0, unlike when using atan, and will return 90", atan2_scalar_scalar));
	add(unary("deg", sqf::type::SCALAR, "Convert a number from Radians to Degrees.", deg_scalar));
	add(unary("log", sqf::type::SCALAR, "Base-10 logarithm of x.", log_scalar));
	add(nular("pi", "The value of PI.", pi_));
	add(unary("sin", sqf::type::SCALAR, "Sine of x, argument in Degrees.", sin_scalar));
	add(unary("acos", sqf::type::SCALAR, "ArcCosine of a number, result in Degrees.", acos_scalar));
	add(unary("exp", sqf::type::SCALAR, "Let x be a number, then exp (x) is equal to e to the power of x (or e^x).", exp_scalar));
	add(unary("rad", sqf::type::SCALAR, "Convert x from Degrees to Radians. 360 degrees is equal to 2 multiplied with pi.", rad_scalar));
	add(unary("sqrt", sqf::type::SCALAR, "Returns square root of x.", sqrt_scalar));
	add(unary("tan", sqf::type::SCALAR, "Tangent of x, argument in Degrees.", tan_scalar));
	add(unary("random", sqf::type::SCALAR, "Random real (floating point) value from 0 (inclusive) to x (not inclusive).", random_scalar));
	add(unary("-", sqf::type::SCALAR, "Zero minus a.", minus_scalar));
	add(unary("+", sqf::type::SCALAR, "Returns a copy of a.", plus_scalar));
	add(unary("+", sqf::type::NaN, "Returns a copy of a.", plus_scalar));
	add(unary("!", sqf::type::BOOL, "Returns a negation of Boolean expression. That means true becomes false and vice versa.", exclamationmark_bool));
	add(binary(6, "min", sqf::type::SCALAR, sqf::type::SCALAR, "The smaller of a, b", min_scalar_scalar));
	add(binary(6, "max", sqf::type::SCALAR, sqf::type::SCALAR, "The greater of a, b.", max_scalar_scalar));
	add(unary("floor", sqf::type::SCALAR, "Returns the next lowest integer in relation to x.", floor_scalar));
	add(unary("ceil", sqf::type::SCALAR, "The ceil value of x.", ceil_scalar));
	add(unary("asin", sqf::type::SCALAR, "Arcsine of a number, result in Degrees.", asin_scalar));
	add(unary("atan", sqf::type::SCALAR, "ArcTangent of a number, result in Degrees.", atan_scalar));
	add(unary("atg", sqf::type::SCALAR, "Equivalent to atan.", atan_scalar));
	add(unary("cos", sqf::type::SCALAR, "Cosine of a number, argument in degrees.", cos_scalar));
	add(unary("ln", sqf::type::SCALAR, "Natural logarithm of x.", ln_scalar));
	add(unary("round", sqf::type::SCALAR, "Rounds up or down to the closest integer.", round_scalar));
	add(binary(7, "mod", sqf::type::SCALAR, sqf::type::SCALAR, "Remainder of a divided by b.", mod_scalar_scalar));
	add(binary(7, "%", sqf::type::SCALAR, sqf::type::SCALAR, "Remainder of a divided by b.", mod_scalar_scalar));
	add(binary(9, "^", sqf::type::SCALAR, sqf::type::SCALAR, "a raised to the power of b.", raisetopower_scalar_scalar));
	add(binary(6, "+", sqf::type::SCALAR, sqf::type::SCALAR, "b added to a.", plus_scalar_scalar));
	add(binary(6, "-", sqf::type::SCALAR, sqf::type::SCALAR, "Subtracts b from a.", minus_scalar_scalar));
	add(binary(7, "*", sqf::type::SCALAR, sqf::type::SCALAR, "Returns the value of a multiplied by b.", multiply_scalar_scalar));
	add(binary(7, "/", sqf::type::SCALAR, sqf::type::SCALAR, "a divided by b. Division by 0 throws \"Division by zero\" error, however script doesn't stop and the result of such division is assumed to be 0.", divide_scalar_scalar));

	add(binary(4, "vectorAdd", type::ARRAY, type::ARRAY, "Adds two 3D vectors.", vectoradd_array_array));
	add(binary(4, "vectorCos", type::ARRAY, type::ARRAY, "Cosine of angle between two 3D vectors.", vectorcos_array_array));
	add(binary(4, "vectorCrossProduct", type::ARRAY, type::ARRAY, "Cross product of two 3D vectors.", vectorcrossproduct_array_array));
	add(binary(4, "vectorDiff", type::ARRAY, type::ARRAY, "Subtracts one 3D vector from another.", vectordiff_array_array));
	add(binary(4, "vectorDistance", type::ARRAY, type::ARRAY, "Distance between two 3D vectors.", vectordistance_array_array));
	add(binary(4, "vectorDistanceSqr", type::ARRAY, type::ARRAY, "Squared distance between two 3D vectors.", vectordistancesqr_array_array));
	add(binary(4, "vectorDotProduct", type::ARRAY, type::ARRAY, "Dot product of two 3D vectors.", vectordotproduct_array_array));
	add(unary("vectorMagnitude", type::ARRAY, "Magnitude of a 3D vector.", vectormagnitude_array));
	add(unary("vectorMagnitudeSqr", type::ARRAY, "Squared magnitude of a 3D vector.", vectormagnitudesqr_array));
	add(binary(4, "vectorMultiply", type::ARRAY, type::SCALAR, "Multiplies 3D vector by a scalar.", vectormultiply_array_scalar));
	add(unary("vectorNormalized", type::ARRAY, "Returns normalized vector (unit vector, vectorMagnitude = 1) of given vector. If given vector is 0 result is a 0 vector as well.", vectornormalized_array));
	add(binary(4, "toFixed", type::SCALAR, type::SCALAR, "Converts a number into a string, keeping the specified number of decimals. Range from 0 to 20.", tofixed_scalar_scalar));
	add(unary("toFixed", type::SCALAR, "Sets how many decimals should be displayed during to string conversion. -1 resets to default. Range from 0 to 20.", tofixed_scalar));


}
