#ifndef _SQF_SCALAR_TYPE_H_
#define _SQF_SCALAR_TYPE_H_

#ifndef _SQF_H_
#error "SQF_types.h" has to be included after "SQF.h" 
#endif // !_SQF_H_


PCMD SCALAR_TYPE(void);

/**
* Adds the given scalars and returns a new value.
* There is no checking performed whether the given values are
* actually scalars so make sure of that before calling this function!
*/
inline VALUE addScalarPointer(const PVALUE left, const PVALUE right) { return value(SCALAR_TYPE(), base_float(left->val.f + right->val.f)); }
/**
* Adds the given scalars and returns a new value.
* There is no checking performed whether the given values are
* actually scalars so make sure of that before calling this function!
*/
inline VALUE addScalar(const VALUE left, const VALUE right) { return addScalarPointer((PVALUE)&left, (PVALUE)&right); }
/**
* Multiplies the given scalars and returns a new value.
* There is no checking performed whether the given values are
* actually scalars so make sure of that before calling this function!
*/
inline VALUE multiplyScalarPointer(const PVALUE left, const PVALUE right) { return value(SCALAR_TYPE(), base_float(left->val.f * right->val.f)); }
/**
* Multiplies the given scalars and returns a new value.
* There is no checking performed whether the given values are
* actually scalars so make sure of that before calling this function!
*/
inline VALUE multiplyScalar(const VALUE left, const VALUE right) { return value(SCALAR_TYPE(), base_float(left.val.f * right.val.f)); }
/**
* Subtracts the given scalars and returns a new value.
* There is no checking performed whether the given values are
* actually scalars so make sure of that before calling this function!
*/
inline VALUE substractScalarPointer(const PVALUE left, const PVALUE right) { return value(SCALAR_TYPE(), base_float(left->val.f - right->val.f)); }
/**
* Subtracts the given scalars and returns a new value.
* There is no checking performed whether the given values are
* actually scalars so make sure of that before calling this function!
*/
inline VALUE substractScalar(const VALUE left, const VALUE right) { return substractScalarPointer(left.val.ptr, right.val.ptr); }
/**
* Divides the given scalars and returns a new value.
* There is no checking performed whether the given values are
* actually scalars so make sure of that before calling this function!
*/
inline VALUE divideScalarPointer(const PVALUE left, const PVALUE right) { return value(SCALAR_TYPE(), base_float(left->val.f / right->val.f)); }
/**
* Divides the given scalars and returns a new value.
* There is no checking performed whether the given values are
* actually scalars so make sure of that before calling this function!
*/
inline VALUE divideScalar(const VALUE left, const VALUE right) { return value(SCALAR_TYPE(), base_float(left.val.f / right.val.f)); }
#endif // !_SQF_SCALAR_TYPE_H_
