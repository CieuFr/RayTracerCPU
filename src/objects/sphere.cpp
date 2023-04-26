#include "sphere.hpp"

namespace RT_ISICG
{
	bool Sphere::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float t1;
		float t2;
		if ( _geometry.intersect( p_ray, t1, t2 ) )
		{
			if ( t1 > p_tMax ) { return false; }				// first intersection too far
			if ( t1 < p_tMin ) { t1 = t2; }						// first intersection too near, check second one
			if ( t1 < p_tMin || t1 > p_tMax ) { return false; } // not in range

			// Intersection found, fill p_hitRecord.
			p_hitRecord._point	= p_ray.pointAtT( t1 );
			p_hitRecord._normal = _geometry.computeNormal( p_hitRecord._point );
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._distance = t1;
			p_hitRecord._object	  = this;
			p_hitRecord._uv		  = getUV( p_hitRecord._point );

			return true;
		}
		return false;
	}

	bool Sphere::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float t1;
		float t2;
		if ( _geometry.intersect( p_ray, t1, t2 ) )
		{
			if ( t1 > p_tMax ) { return false; }				// first intersection too far
			if ( t1 < p_tMin ) { t1 = t2; }						// first intersection too near, check second one
			if ( t1 < p_tMin || t1 > p_tMax ) { return false; } // not in range
			return true;
		}
		return false;
	}


	const Vec2f Sphere::getUV( const Vec3f & p_point ) const
	{
		// p: a given point on the sphere of radius one, centered at the origin.
		// u: returned value [0,1] of angle around the Y axis from X=-1.
		// v: returned value [0,1] of angle from Y=-1 to Y=+1.
		//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
		//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
		//     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

		auto theta = acos( -p_point.y );
		auto phi   = atan2( -p_point.z, p_point.x ) + PIf;

		float u = phi / ( 2 * PIf );
		float v = theta / PIf;

		return Vec2f( u, v );
	}

} // namespace RT_ISICG
