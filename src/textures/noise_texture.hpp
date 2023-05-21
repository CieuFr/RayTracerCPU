#ifndef __RT_ISICG_NOISE_TEXTURE__
#define __RT_ISICG_NOISe_TEXTURE__

#include "defines.hpp"
#include "stb/stb_image.h"
#include "textures/texture.hpp"
#include "utils/file_path.hpp"
#include <algorithm>
#include <string>
#include <vector>
#include "perlin_noise.hpp"

namespace RT_ISICG
{
	class NoiseTexture : public Texture
	{
	  public:

		~NoiseTexture() = default;
		NoiseTexture( float p_scale ) : _scale(p_scale) {}

		virtual Vec3f value( const Vec2f & p_uv, const Vec3f & p_point ) const override
		{
			return Vec3f( 1.f, 1.f, 1.f ) * 0.5f * ( 1 + sin( _scale *p_point.z + 10 * _noise.turb( p_point ) ) );
		}
		
		PerlinNoise _noise;
		float _scale;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMAGE__
