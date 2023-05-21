#ifndef __RT_ISICG_IMAGE__
#define __RT_ISICG_IMAGE__

#include "defines.hpp"
#include "textures/texture.hpp"
#include <string>
#include <vector>
#include <algorithm>
#include "stb/stb_image.h"
#include "utils/file_path.hpp"

namespace RT_ISICG
{
	class ImageTexture : public Texture
	{
	  public:
		ImageTexture() = default;
		~ImageTexture() = default;
		ImageTexture( const int p_width, const int p_height, const int _nbChannels = 3 )
			: _width( p_width ), _height( p_height ), _pixels( _width * _height * _nbChannels, 0 )
		{
			_pixels.shrink_to_fit();
		}
		
		
		inline const int						  getWidth() const { return _width; }
		inline const int						  getHeight() const { return _height; }
		inline std::vector<unsigned char> &		  getPixels() { return _pixels; }
		inline const std::vector<unsigned char> & getPixels() const { return _pixels; }

		void setPixel( const int p_i, const int p_j, const Vec3f & p_color )
		{
			assert( _nbChannels == 3 );
			const int pixelId	   = ( p_i + p_j * _width ) * _nbChannels;
			_pixels[ pixelId ]	   = static_cast<unsigned char>( p_color.r * 255.f );
			_pixels[ pixelId + 1 ] = static_cast<unsigned char>( p_color.g * 255.f );
			_pixels[ pixelId + 2 ] = static_cast<unsigned char>( p_color.b * 255.f );
		}

		void setPixel( const int p_i, const int p_j, const Vec4f & p_color )
		{
			assert( _nbChannels == 4 );
			const int pixelId	   = ( p_i + p_j * _width ) * _nbChannels;
			_pixels[ pixelId ]	   = static_cast<unsigned char>( p_color.r * 255.f );
			_pixels[ pixelId + 1 ] = static_cast<unsigned char>( p_color.g * 255.f );
			_pixels[ pixelId + 2 ] = static_cast<unsigned char>( p_color.b * 255.f );
			_pixels[ pixelId + 3 ] = static_cast<unsigned char>( p_color.a * 255.f );
		}

		void saveJPG( const std::string & p_path, const int p_quality = 100 );	   

		bool load( const std::string & p_path );	

		virtual Vec3f value(const Vec2f & p_uv, const Vec3f & p_point ) const override;		

	  private:
		int _nbChannels = 3;
		int		  _width	  = 0;
		int		  _height	  = 0;

		// if false : nearest 
		bool bilinearFiltering = true;
		
		// Duplication des données à ce niveau car je n'ai pas réussi à faire fonctionner stbi avec les vectors
		// J'ai gardé le code pour l'image de sortie dans la variable _pixels
		// et les textures sont dans la variable _data
		std::vector<unsigned char> _pixels;
		unsigned char *			   _data = nullptr;

		
		
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMAGE__
