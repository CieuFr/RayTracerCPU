#include "implicit_surface.hpp"

namespace RT_ISICG
{
	bool ImplicitSurface::intersect( const Ray & p_ray,
									 const float p_tMin,
									 const float p_tMax,
									 HitRecord & p_hitRecord ) const
	{
		float t = p_tMin;
		for ( int i = 0; i < 10; i++ )
		{
			float distanceFromObject = _sdf( p_ray.pointAtT( t ) );

			if ( distanceFromObject < _minDistance )
			{
				p_hitRecord._distance = distanceFromObject;
				p_hitRecord._point	  = p_ray.pointAtT( t );
				p_hitRecord._normal	  = _evaluateNormal( p_ray.pointAtT( t ) );
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._object	  = this;
				return true;
			}

			t += abs( distanceFromObject );
			if ( t > p_tMax ) return false;

		}

		return false;
	}

	bool ImplicitSurface::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float t = p_tMin;
		for ( int i = 0; i < 10; i++ )
		{
			float distanceFromObject = _sdf( p_ray.pointAtT( t ) );

			if ( distanceFromObject < _minDistance  )
			{
				return true;
			}

			t += abs( distanceFromObject );
			if ( t > p_tMax ) return false;
		}

		return false;
	}
} // namespace RT_ISICG
