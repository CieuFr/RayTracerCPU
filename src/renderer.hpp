#ifndef __RT_ISICG_RENDERER__
#define __RT_ISICG_RENDERER__

#include "cameras/base_camera.hpp"
#include "defines.hpp"
#include "integrators/base_integrator.hpp"
#include "textures/image_texture.hpp"
#include "utils/chrono.hpp"

namespace RT_ISICG
{
	class Renderer
	{
	  public:
		Renderer();
		~Renderer() { delete _integrator; }

		void			 setIntegrator( const IntegratorType p_integratorType );
		BaseIntegrator * getIntegrator() { return _integrator; }
		void			 setBackgroundColor( const Vec3f & p_color );
		void			 setNumberOfLightSamples( const int & p_numberOfLightSamples );
		
		Vec3f			 Tonemap_ACES( const Vec3f p_color )
		{
			// Narkowicz 2015, "ACES Filmic Tone Mapping Curve"
			const float a = 2.51f;
			const float b = 0.03f;
			const float c = 2.43f;
			const float d = 0.59f;
			const float e = 0.14f;
			return ( p_color * ( a * p_color + b ) ) / ( p_color * ( c * p_color + d ) + e );
		}


		inline void setNbPixelSamples( const int p_nbPixelSamples ) { _nbPixelSamples = p_nbPixelSamples; }

		float renderImage( const Scene & p_scene, const BaseCamera * p_camera, ImageTexture & p_texture );

	  private:
		BaseIntegrator * _integrator	 = nullptr;
		int				 _nbPixelSamples = 1;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_RENDERER__
