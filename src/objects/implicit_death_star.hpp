#pragma once
#pragma once
#ifndef __RT_ISICG_IMPLICIT_DEATH_STAR
#define __RT_ISICG_IMPLICIT_DEATH_STAR

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitDeathStar : public ImplicitSurface
	{
	  public:
		ImplicitDeathStar()			 = delete;
		virtual ~ImplicitDeathStar() = default;

		ImplicitDeathStar( const std::string & p_name,
						   const Vec3f &	   p_centre,
						   const float		   p_smallSphereRadius,
						   const float		   p_bigSphereRadius,
						   const float		   p_distanceBetweenSpheres )
			: ImplicitSurface( p_name ), _centre( p_centre ), _smallSphereRadius( p_smallSphereRadius ),
			  _bigSphereRadius( p_bigSphereRadius ), _distanceBetweenSpheres( p_distanceBetweenSpheres )
		{
		}

		float _sdf( const Vec3f & p_point ) const override
		{
			// sampling independent computations (only depend on shape)
			float a = ( _bigSphereRadius * _bigSphereRadius - _smallSphereRadius * _smallSphereRadius
						+ _distanceBetweenSpheres * _distanceBetweenSpheres )
					  / ( 2.0f * _distanceBetweenSpheres );
			float b = sqrt( glm::max( _bigSphereRadius * _bigSphereRadius - a * a, 0.f ) );

			// sampling dependant computations
			Vec2f p = Vec2f( _centre.x, length( Vec2f( _centre.y, _centre.z ) ) );
			if ( p.x * b - p.y * a > _distanceBetweenSpheres * glm::max( b - p.y, 0.f ) )
				return length( p - Vec2f( a, b ) );
			else
				return glm::max( ( length( p ) - _bigSphereRadius ),
								 -( length( p - Vec2f( _distanceBetweenSpheres, 0.f ) ) - _smallSphereRadius ) );
		}

	  private:
		// Signed Distance Function
		Vec3f _centre = VEC3F_ZERO;
		float _bigSphereRadius;
		float _smallSphereRadius;
		float _distanceBetweenSpheres;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_DEATH_STAR
