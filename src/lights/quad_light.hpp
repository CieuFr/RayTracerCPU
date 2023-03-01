#ifndef __RT_ISICG_QUAD_LIGHT__
#define __RT_ISICG_QUAD_LIGHT__

#include "base_light.hpp"
#include "utils/random.hpp"

namespace RT_ISICG
{
	class QuadLight : public BaseLight
	{
	  public:
		QuadLight( const Vec3f p_positon,
				   const Vec3f p_u,
				   const Vec3f p_v,
				   const Vec3f p_color,
				   const float p_power = 1.f )
			: BaseLight( p_color, p_power, true ), _position( p_positon ), _u( p_u ), _v( p_v )
		{
			_area = length( _u ) * length( _v );
			_n	  = normalize( cross( _u, _v ) );
		}

		~QuadLight() = default;

		LightSample sample( const Vec3f & p_point ) const
		{
			Vec3f randomPoint	 = _position + _u * randomFloat() + _v * randomFloat();
			Vec3f vecPHitToLight = randomPoint - p_point;
			Vec3f direction		 = glm::normalize( vecPHitToLight );
			float distance		 = glm::length( vecPHitToLight );
			float pdf			 = ( ( 1 / _area ) * ( distance * distance ) ) / ( glm::dot( _n, direction ) );

			Vec3f radiance = ( _color * _power ) / ( pdf );

			return LightSample( direction, distance, radiance, pdf );
		}

		Vec3f getPosition() const { return _position; }

	  protected:
		// TODO: give a name, like objects and materials
		Vec3f _position;
		Vec3f _u;
		Vec3f _v;
		Vec3f _n;
		float _area;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_LIGHT__
