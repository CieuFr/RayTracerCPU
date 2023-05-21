#ifndef __RT_ISICG_CHECKER_TEXTURE__
#define __RT_ISICG_CHECKER_TEXTURE__

#include "defines.hpp"
#include "stb/stb_image.h"
#include "textures/texture.hpp"
#include "utils/file_path.hpp"
#include <algorithm>
#include <string>
#include <vector>

namespace RT_ISICG
{
	class CheckerTexture : public Texture
	{
	  public:
		CheckerTexture() = default;
		~CheckerTexture() { 
			delete _even;
			delete _odd;
		}
		CheckerTexture( Texture * p_even, Texture * p_odd ) : _even( p_even ), _odd( p_odd ) {}


		virtual Vec3f value(const Vec2f& p_uv, const Vec3f& p_point) const override {
			float sines = sin( 5 * p_point.x ) * sin( 5 * p_point.y ) * sin( 5 * p_point.z );
			if ( sines < 0 )
				return _odd->value( p_uv, p_point );
			else
				return _even->value( p_uv, p_point );
		}

		Texture * _even = nullptr;
		Texture*  _odd = nullptr;
		
		
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMAGE__
