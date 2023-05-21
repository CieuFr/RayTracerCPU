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
		_data = stbi_load( p_path.c_str(), &_width, &_height, &_nbChannels, 0 );
		if ( _data == nullptr )
		{
			std::cerr << "[ERROR] - Failed to load texture " << p_path << std::endl;
			std::cerr << "=> " << stbi_failure_reason << std::endl;
		}
		std::cout << "load file " << std::endl;

		return _data != nullptr;
	}

	// https://raytracing.github.io/books/RayTracingTheNextWeek.html#imagetexturemapping

	Vec3f ImageTexture::value( const Vec2f & p_uv, const Vec3f & p_point ) const
	{
		const static int bytes_per_pixel	= 3;
		const static int bytes_per_scanline = bytes_per_pixel * _width;
		// If we have no texture data, then return solid cyan as a debugging aid.
		if ( _data == nullptr ) return Vec3f( 0, 1, 1 );

		// Clamp input texture coordinates to [0,1] x [1,0]
		float u = glm::clamp( p_uv.x, 0.0f, 1.0f );
		float v = 1.0f - glm::clamp( p_uv.y, 0.0f, 1.0f ); // Flip V to image coordinates


		if (bilinearFiltering) {

			float i = u * _width;
			float j = v * _height;

			// Clamp integer mapping, since actual coordinates should be less than 1.0
			if ( i >= _width ) i = _width - 1.f;
			if ( j >= _height ) j = _height - 1.f;

			int	  iUp	= int(std::ceil( i ));
			int	  iDown = int(std::floor( i ));
			int	  jUp	= int(std::ceil( j ));
			int	  jDown = int(std::floor( j ));
			float iFrac = i - iDown;
			float jFrac = j - jDown;

			const float		color_scale	  = 1.0f / 255.0f;
			unsigned char * topLeftPixel  = _data + jDown * bytes_per_scanline + iDown * bytes_per_pixel;
			unsigned char * topRightPixel = _data + jDown * bytes_per_scanline + iUp * bytes_per_pixel;
			unsigned char * botLeftPixel  = _data + jUp * bytes_per_scanline + iDown * bytes_per_pixel;
			unsigned char * botRightPixel = _data + jUp * bytes_per_scanline + iUp * bytes_per_pixel;
			Vec3f			topLeftColor  = Vec3f(
				   color_scale * topLeftPixel[ 0 ], color_scale * topLeftPixel[ 1 ], color_scale * topLeftPixel[ 2 ] );
			Vec3f topRightColor = Vec3f(
				color_scale * topRightPixel[ 0 ], color_scale * topRightPixel[ 1 ], color_scale * topRightPixel[ 2 ] );
			Vec3f botLeftColor = Vec3f(
				color_scale * botLeftPixel[ 0 ], color_scale * botLeftPixel[ 1 ], color_scale * botLeftPixel[ 2 ] );
			Vec3f botRightColor = Vec3f(
				color_scale * botRightPixel[ 0 ], color_scale * botRightPixel[ 1 ], color_scale * botRightPixel[ 2 ] );

			Vec3f interpolationTop = topLeftColor * iFrac + topRightColor * ( 1 - iFrac );
			Vec3f interpolationBot = botLeftColor * iFrac + botRightColor * ( 1 - iFrac );

			Vec3f interpolationTopBot = interpolationTop * jFrac + interpolationBot * ( 1 - jFrac );

			return interpolationTopBot;

		}
		else
		{
			int i = static_cast<int>( u * _width );
			int j = static_cast<int>( v * _height );

			// Clamp integer mapping, since actual coordinates should be less than 1.0
			if ( i >= _width ) i = _width - 1;
			if ( j >= _height ) j = _height - 1;

			const float color_scale = 1.0f / 255.0f;
			unsigned char *	   pixel	   = _data + j * bytes_per_scanline + i * bytes_per_pixel;

			return Vec3f( color_scale * pixel[ 0 ], color_scale * pixel[ 1 ], color_scale * pixel[ 2 ] );
	
		
		}
	
		


		

		}


} // namespace RT_ISICG
