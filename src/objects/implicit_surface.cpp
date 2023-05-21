#include "implicit_surface.hpp"

namespace RT_ISICG
{
	bool ImplicitSurface::intersect( const Ray & p_ray,
									 const float p_tMin,
									 const float p_tMax,
									 HitRecord & p_hitRecord ) const
	{
		float t = p_tMin;

		while (t <= p_tMax && t >= p_tMin) {

			float distanceFromObject = _sdf( p_ray.pointAtT( t ) );

			if ( abs( distanceFromObject ) < _minDistance )
			{
				p_hitRecord._distance = t;
				p_hitRecord._point	  = p_ray.pointAtT( t );
				p_hitRecord._normal	  = _evaluateNormal( p_ray.pointAtT( t ) );
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._object	  = this;
				return true;
			}
			t += abs( distanceFromObject );
		}


		return false;
	}

	bool ImplicitSurface::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float t = p_tMin;

		while ( t <= p_tMax && t >= p_tMin )
		{
			float distanceFromObject = _sdf( p_ray.pointAtT( t ) );

			if ( abs( distanceFromObject ) < _minDistance )
			{
				return true;
			}
			t += abs( distanceFromObject );
		}

		return false;

	}

	Mat3f ImplicitSurface::rotateX( float p_theta ) const
	{
		float c = cos( p_theta );
		float s = sin( p_theta );
		return Mat3f( Vec3f( 1, 0, 0 ), Vec3f( 0, c, -s ), Vec3f( 0, s, c ) );
	}

	// Rotation matrix around the Y axis.
	Mat3f ImplicitSurface::rotateY( float p_theta ) const
	{
		float c = cos( p_theta );
		float s = sin( p_theta );
		return Mat3f( Vec3f( c, 0, s ), Vec3f( 0, 1, 0 ), Vec3f( -s, 0, c ) );
	}

	// Rotation matrix around the Z axis.
	Mat3f ImplicitSurface::rotateZ( float p_theta ) const
	{
		float c = cos( p_theta );
		float s = sin( p_theta );
		return Mat3f( Vec3f( c, -s, 0 ), Vec3f( s, c, 0 ), Vec3f( 0, 0, 1 ) );
	}

	// Identity matrix.
	Mat3f ImplicitSurface::identity() { return Mat3f( Vec3f( 1, 0, 0 ), Vec3f( 0, 1, 0 ), Vec3f( 0, 0, 1 ) ); }


} // namespace RT_ISICG
