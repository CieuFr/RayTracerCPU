#include "texture.hpp"
#include "io/stb_include.hpp"

namespace RT_ISICG
{
	void Texture::saveJPG( const std::string & p_path, const int p_quality )
	{
		stbi_write_jpg( p_path.c_str(), int( _width ), _height, _nbChannels, _pixels.data(), p_quality );
	}

	Vec3f Texture::value( Vec2f p_uv, const Vec3f & p_point ) { return VEC3F_ZERO; }
	
	
	
} // namespace RT_ISICG
