#ifndef __RT_ISICG_SOLID_COLOR__
#define __RT_ISICG_SOLID_COLOR__

#include "defines.hpp"
#include "texture.hpp"

namespace RT_ISICG
{
	class SolidColorTexture : public Texture
	{
	  public:
		SolidColorTexture() = default;
		SolidColorTexture( Vec3f p_color ) : _color( p_color ) {}
		~SolidColorTexture() = default;

		virtual Vec3f value(const Vec2f& p_uv, const Vec3f & p_point ) const { return _color; }

	  private:
		Vec3f _color = WHITE;
	};


} // namespace RT_ISICG
#endif