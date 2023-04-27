#include "textures/image_texture.hpp"
#include "io/stb_include.hpp"

namespace RT_ISICG
{
	void ImageTexture::saveJPG( const std::string & p_path, const int p_quality )
	{
		stbi_write_jpg( p_path.c_str(), int( _width ), _height, _nbChannels, _pixels.data(), p_quality );
	}

	Vec3f ImageTexture::value( const Vec2f & p_uv, const Vec3f & p_point ) const { 

		return Vec3f( 0.f, 0.f, 1.f );
		//// If we have no texture data, then return solid cyan as a debugging aid.
		//if ( _pixels == nullptr ) return Vec3f( 0, 1, 1 );

		//// Clamp input texture coordinates to [0,1] x [1,0]
		//u = clamp( u, 0.0, 1.0 );
		//v = 1.0 - clamp( v, 0.0, 1.0 ); // Flip V to image coordinates

		//auto i = static_cast<int>( u * _width );
		//auto j = static_cast<int>( v * _height );

		//// Clamp integer mapping, since actual coordinates should be less than 1.0
		//if ( i >= width ) i = width - 1;
		//if ( j >= height ) j = height - 1;

		//const auto color_scale = 1.0 / 255.0;
		//auto	   pixel	   = data + j * bytes_per_scanline + i * bytes_per_pixel;

		//return Vec3f( color_scale * pixel[ 0 ], color_scale * pixel[ 1 ], color_scale * pixel[ 2 ] );


	}
	
	
} // namespace RT_ISICG
