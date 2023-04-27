#ifndef __RT_ISICG_TEXTURE__
#define __RT_ISICG_TEXTURE__

#include "defines.hpp"
#include <string>
#include <vector>

namespace RT_ISICG
{
	class Texture
	{
	  public:
		virtual Vec3f value( const Vec2f & p_uv, const Vec3f & p_point ) const = 0;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_TEXTURE__


