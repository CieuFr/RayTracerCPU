#ifndef __RT_ISICG_TEXTURE_MATERIAL__
#define __RT_ISICG_TEXTURE_MATERIAL__

#include "base_material.hpp"
#include "defines.hpp"
#include "textures/solid_color.hpp"
#include "textures/texture.hpp"

namespace RT_ISICG
{
	class TextureMaterial : public BaseMaterial
	{
	  public:
		TextureMaterial( const std::string & p_name, const Vec3f & p_color ) : BaseMaterial( p_name )
		{
			_texture = new SolidColorTexture( p_color );
		}
		TextureMaterial( const std::string & p_name, Texture * p_texture )
			: BaseMaterial( p_name ), _texture( p_texture )
		{
		}

		virtual ~TextureMaterial() = default;

		Vec3f shade( const Ray & p_ray, const HitRecord & p_hit, const LightSample & p_lightSample ) const override
		{
			return _texture->value( p_hit._uv, p_hit._point );
		}

		inline const Vec3f & getFlatColor() const override { return _texture->value( VEC2F_ZERO, VEC3F_ZERO ); }

	  private:
		Texture * _texture = nullptr;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_COLOR_MATERIAL__
