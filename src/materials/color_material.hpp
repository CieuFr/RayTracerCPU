#ifndef __RT_ISICG_COLOR_MATERIAL__
#define __RT_ISICG_COLOR_MATERIAL__

#include "base_material.hpp"
#include "defines.hpp"

namespace RT_ISICG
{
	class ColorMaterial : public BaseMaterial
	{
	  public:
		ColorMaterial( const std::string & p_name, const Vec3f & p_color ) : BaseMaterial( p_name, p_color ) {}
		ColorMaterial( const std::string & p_name, ImageTexture * p_texture ) : BaseMaterial( p_name, p_texture ) {}

		virtual ~ColorMaterial() = default;

		Vec3f shade( const Ray & p_ray, const HitRecord & p_hit, const LightSample & p_lightSample ) const override
		{
			return _texture->value( p_hit._uv, p_hit._point );
		}

		inline const Vec3f & getFlatColor() const override { return _texture->value( VEC2F_ZERO, VEC3F_ZERO ); }

	};

} // namespace RT_ISICG

#endif // __RT_ISICG_COLOR_MATERIAL__
