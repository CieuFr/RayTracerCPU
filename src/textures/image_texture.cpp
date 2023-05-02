#include "textures/image_texture.hpp"
#include "io/stb_include.hpp"

namespace RT_ISICG
{
	 

	void ImageTexture::saveJPG( const std::string & p_path, const int p_quality )
	{
		stbi_write_jpg( p_path.c_str(), int( _width ), _height, _nbChannels, _pixels.data(), p_quality );
	}

	bool ImageTexture::load( const std::string & p_path )
	{
		unsigned char * pix = nullptr;
		pix					= stbi_load( p_path.c_str(), &_width, &_height, &_nbChannels, 0 );
		if ( pix == nullptr )
		{
			std::cerr << "[ERROR] - Failed to load texture " << p_path << std::endl;
			std::cerr << "=> " << stbi_failure_reason << std::endl;
		}

		if ( pix != nullptr )
		{
			const size_t numBytes = sizeof( pix );

			std::vector<unsigned char> _pixels( numBytes );
			std::copy( pix, pix + numBytes, _pixels.begin() );
		}
		return pix != nullptr;
	}


	Vec3f ImageTexture::value( const Vec2f & p_uv, const Vec3f & p_point ) const { 

		const static int bytes_per_pixel = 3;
		const static int bytes_per_scanline				 = bytes_per_pixel * _width;

		// If we have no texture data, then return solid cyan as a debugging aid.
		if ( _pixels.data() == nullptr ) return Vec3f( 0, 1, 1 );

		// Clamp input texture coordinates to [0,1] x [1,0]
		float u = glm::clamp( p_uv.x, 0.0f, 1.0f );
		float v = 1.0 - glm::clamp( p_uv.y, 0.0f, 1.0f ); // Flip V to image coordinates

		auto i = static_cast<int>( u * _width );
		auto j = static_cast<int>( v * _height );

		// Clamp integer mapping, since actual coordinates should be less than 1.0
		if ( i >= _width ) i = _width - 1;
		if ( j >= _height ) j = _height - 1;

		const auto color_scale = 1.0 / 255.0;
		auto	   pixel	   = _pixels.data() + j * bytes_per_scanline + i * bytes_per_pixel;

		return Vec3f( color_scale * pixel[ 0 ], color_scale * pixel[ 1 ], color_scale * pixel[ 2 ] );


	}
	
	
} // namespace RT_ISICG
