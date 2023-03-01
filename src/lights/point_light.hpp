#ifndef __RT_ISICG_POINT_LIGHT__
#define __RT_ISICG_POINT_LIGHT__

#include "base_light.hpp"

namespace RT_ISICG
{
	class PointLight : public BaseLight
	{
	  public:
		PointLight( const Vec3f p_positon, const Vec3f p_color, const float p_power = 1.f ): BaseLight( p_color, p_power ), _position( p_positon ) {}
		
		~PointLight() = default;

		LightSample sample( const Vec3f & p_point ) const 
		{
			Vec3f AB		= _position - p_point;
			float distance	= glm::length( AB );
			Vec3f direction = glm::normalize( AB );
			Vec3f radiance	= _color * _power * ( 1 / ( distance * distance ) );
			float pdf		= 1;
			return LightSample( direction, distance, radiance, pdf );
		}

		Vec3f getPosition() const { return _position; }

	  protected:
		// TODO: give a name, like objects and materials
		Vec3f _position;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_LIGHT__
