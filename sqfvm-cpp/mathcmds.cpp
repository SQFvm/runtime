#include "commandmap.h"
#include "value.h"
#include "cmd.h"
#include "virtualmachine.h"
#include "compiletime.h"
#include <cmath>
#include <random>
using namespace sqf;
namespace
{
	std::shared_ptr<value> plus_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		return std::make_shared<value>((left->as_double()) + (right->as_double()));
	}
	std::shared_ptr<value> minus_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		return std::make_shared<value>((left->as_double()) - (right->as_double()));
	}
	std::shared_ptr<value> multiply_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		return std::make_shared<value>((left->as_double()) * (right->as_double()));
	}
	std::shared_ptr<value> divide_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto r = (right->as_double());
		if (r == 0)
			return std::make_shared<value>(0);
		return std::make_shared<value>((left->as_double()) / r);
	}
	std::shared_ptr<value> abs_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_double();
		return std::make_shared<value>(std::fabs(right->as_double()));
	}
	std::shared_ptr<value> atan2_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(std::atan2(left->as_double(), right->as_double()));
	}
	std::shared_ptr<value> deg_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(right->as_double() * (180 / compiletime::pi()));
	}
	std::shared_ptr<value> log_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(std::log10(right->as_double()));
	}
	std::shared_ptr<value> pi_(virtualmachine* vm)
	{
		return std::make_shared<value>(compiletime::pi());
	}
	std::shared_ptr<value> sin_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(std::sin(right->as_double()));
	}
	std::shared_ptr<value> acos_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(std::acos(right->as_double()));
	}
	std::shared_ptr<value> exp_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(std::exp(right->as_double()));
	}
	std::shared_ptr<value> rad_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(right->as_double() * (compiletime::pi() / 180));
	}
	std::shared_ptr<value> sqrt_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(std::sqrt(right->as_double()));
	}
	std::shared_ptr<value> tan_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(std::tan(right->as_double()));
	}
	std::shared_ptr<value> random_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return std::make_shared<value>(compiletime::rand_lim(std::rand(), right->as_double()));
	}
	std::shared_ptr<value> min_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_double();
		auto r = right->as_double();
		return std::make_shared<value>(std::fmin(l, r));
	}
	std::shared_ptr<value> max_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_double();
		auto r = right->as_double();
		return std::make_shared<value>(std::fmax(l, r));
	}
	std::shared_ptr<value> floor_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_long();
		return std::make_shared<value>(r);
	}
	std::shared_ptr<value> ceil_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_long();
		return std::make_shared<value>(std::ceil(r));
	}
	std::shared_ptr<value> asin_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_double();
		return std::make_shared<value>(std::asin(r));
	}
	std::shared_ptr<value> atan_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_double();
		return std::make_shared<value>(std::atan(r));
	}
	std::shared_ptr<value> cos_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_double();
		return std::make_shared<value>(std::cos(r));
	}
	std::shared_ptr<value> ln_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_double();
		return std::make_shared<value>(std::log(r));
	}
	std::shared_ptr<value> mod_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_double();
		auto r = right->as_double();
		return std::make_shared<value>(std::fmod(l, r));
	}
	std::shared_ptr<value> round_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_double();
		return std::make_shared<value>(std::round(r));
	}
	std::shared_ptr<value> minus_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto r = right->as_double();
		return std::make_shared<value>(-r);
	}
	std::shared_ptr<value> exclamationmark_bool(virtualmachine* vm, std::shared_ptr<value> right)
	{
		auto f = right->as_bool();
		return std::make_shared<value>(!f);
	}
	std::shared_ptr<value> raisetopower_scalar_scalar(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_double();
		auto r = right->as_double();
		return std::make_shared<value>(std::pow(l, r));
	}
	std::shared_ptr<value> and_bool_bool(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_bool();
		auto r = right->as_bool();
		return std::make_shared<value>(l && r);
	}
	std::shared_ptr<value> or_bool_bool(virtualmachine* vm, std::shared_ptr<value> left, std::shared_ptr<value> right)
	{
		auto l = left->as_bool();
		auto r = right->as_bool();
		return std::make_shared<value>(l || r);
	}
	std::shared_ptr<value> plus_scalar(virtualmachine* vm, std::shared_ptr<value> right)
	{
		return right;
	}
}

void sqf::commandmap::initmathcmds(void)
{
	add(unary(L"abs", sqf::type::SCALAR, L"Absolute value of a real number", abs_scalar));
	add(binary(7, L"atan2", sqf::type::SCALAR, sqf::type::SCALAR, L"ArcTangent of x/y. Used to determine the angle of a vector [x,y]. Result in Degrees between -180 and 180. Note that this command can handle y being 0, unlike when using atan, and will return 90", atan2_scalar_scalar));
	add(unary(L"deg", sqf::type::SCALAR, L"Convert a number from Radians to Degrees.", deg_scalar));
	add(unary(L"log", sqf::type::SCALAR, L"Base-10 logarithm of x.", log_scalar));
	add(nular(L"pi", L"The value of PI.", pi_));
	add(unary(L"sin", sqf::type::SCALAR, L"Sine of x, argument in Degrees.", sin_scalar));
	add(unary(L"acos", sqf::type::SCALAR, L"ArcCosine of a number, result in Degrees.", acos_scalar));
	add(unary(L"exp", sqf::type::SCALAR, L"Let x be a number, then exp (x) is equal to e to the power of x (or e^x).", exp_scalar));
	add(unary(L"rad", sqf::type::SCALAR, L"Convert x from Degrees to Radians. 360 degrees is equal to 2 multiplied with pi.", rad_scalar));
	add(unary(L"sqrt", sqf::type::SCALAR, L"Returns square root of x.", rad_scalar));
	add(unary(L"tan", sqf::type::SCALAR, L"Tangent of x, argument in Degrees.", tan_scalar));
	add(unary(L"random", sqf::type::SCALAR, L"Random real (floating point) value from 0 (inclusive) to x (not inclusive).", random_scalar));
	add(unary(L"-", sqf::type::SCALAR, L"Zero minus a.", minus_scalar));
	add(unary(L"+", sqf::type::SCALAR, L"Returns a copy of a.", plus_scalar));
	add(unary(L"!", sqf::type::BOOL, L"Returns a negation of Boolean expression. That means true becomes false and vice versa.", exclamationmark_bool));
	add(binary(6, L"min", sqf::type::SCALAR, sqf::type::SCALAR, L"The smaller of a, b", min_scalar_scalar));
	add(binary(6, L"max", sqf::type::SCALAR, sqf::type::SCALAR, L"The greater of a, b.", max_scalar_scalar));
	add(unary(L"floor", sqf::type::SCALAR, L"Returns the next lowest integer in relation to x.", floor_scalar));
	add(unary(L"ceil", sqf::type::SCALAR, L"The ceil value of x.", ceil_scalar));
	add(unary(L"asin", sqf::type::SCALAR, L"Arcsine of a number, result in Degrees.", asin_scalar));
	add(unary(L"atan", sqf::type::SCALAR, L"ArcTangent of a number, result in Degrees.", atan_scalar));
	add(unary(L"atg", sqf::type::SCALAR, L"Equivalent to atan.", atan_scalar));
	add(unary(L"cos", sqf::type::SCALAR, L"Cosine of a number, argument in degrees.", cos_scalar));
	add(unary(L"ln", sqf::type::SCALAR, L"Natural logarithm of x.", ln_scalar));
	add(unary(L"round", sqf::type::SCALAR, L"Rounds up or down to the closest integer.", round_scalar));
	add(binary(7, L"mod", sqf::type::SCALAR, sqf::type::SCALAR, L"Remainder of a divided by b.", mod_scalar_scalar));
	add(binary(7, L"%", sqf::type::SCALAR, sqf::type::SCALAR, L"Remainder of a divided by b.", mod_scalar_scalar));
	add(binary(9, L"^", sqf::type::SCALAR, sqf::type::SCALAR, L"a raised to the power of b.", raisetopower_scalar_scalar));
	add(binary(6, L"+", sqf::type::SCALAR, sqf::type::SCALAR, L"b added to a.", plus_scalar_scalar));
	add(binary(6, L"-", sqf::type::SCALAR, sqf::type::SCALAR, L"Subtracts b from a.", minus_scalar_scalar));
	add(binary(7, L"*", sqf::type::SCALAR, sqf::type::SCALAR, L"Returns the value of a multiplied by b.", multiply_scalar_scalar));
	add(binary(7, L"/", sqf::type::SCALAR, sqf::type::SCALAR, L"a divided by b. Division by 0 throws \"Division by zero\" error, however script doesn't stop and the result of such division is assumed to be 0.", divide_scalar_scalar));
}
