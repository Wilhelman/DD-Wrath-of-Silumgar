// ----------------------------------------------------
 // Point class    -----------
 // ----------------------------------------------------

#ifndef __CTPOINT_H__
#define __CTPOINT_H__

#include "ctDefs.h"
#include <math.h>

	template<class TYPE>
class ctPoint
{
public:

	TYPE x, y;

	ctPoint()
	{}

	ctPoint(const ctPoint<TYPE>& v)
	{
		this->x = v.x;
		this->y = v.y;
	}

	ctPoint(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;
	}

	ctPoint& create(const TYPE& x, const TYPE& y)
	{
		this->x = x;
		this->y = y;

		return(*this);
	}

	// Math ------------------------------------------------
	ctPoint operator -(const ctPoint &v) const
	{
		ctPoint r;

		r.x = x - v.x;
		r.y = y - v.y;

		return(r);
	}

	ctPoint operator + (const ctPoint &v) const
	{
		ctPoint r;

		r.x = x + v.x;
		r.y = y + v.y;

		return(r);
	}

	const ctPoint& operator -=(const ctPoint &v)
	{
		x -= v.x;
		y -= v.y;

		return(*this);
	}

	const ctPoint& operator +=(const ctPoint &v)
	{
		x += v.x;
		y += v.y;

		return(*this);
	}

	bool operator ==(const ctPoint& v) const
	{
		return (x == v.x && y == v.y);
	}

	bool operator !=(const ctPoint& v) const
	{
		return (x != v.x || y != v.y);
	}

	// Utils ------------------------------------------------
	bool IsZero() const
	{
		return (x == 0 && y == 0);
	}

	ctPoint& SetToZero()
	{
		x = y = 0;
		return(*this);
	}

	ctPoint& Negate()
	{
		x = -x;
		y = -y;

		return(*this);
	}

	// Distances ---------------------------------------------
	TYPE DistanceTo(const ctPoint& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return sqrtf((fx*fx) + (fy*fy));
	}

	TYPE DistanceNoSqrt(const ctPoint& v) const
	{
		TYPE fx = x - v.x;
		TYPE fy = y - v.y;

		return (fx*fx) + (fy*fy);
	}

	TYPE DistanceManhattan(const ctPoint& v) const
	{
		return abs(v.x - x) + abs(v.y - y);
	}
};

typedef ctPoint<int> iPoint;
typedef ctPoint<float> fPoint;

#endif // __CTPOINT_H__
