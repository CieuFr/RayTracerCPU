#ifndef __RT_ISICG_METAL_MATERIAL__
#define __RT_ISICG_METAL_MATERIAL__

#include "base_material.hpp"
#include "brdfs/cook_torrance_brdf.hpp"
#include "brdfs/oren_nayar_brdf.hpp"

namespace RT_ISICG
{
	class MetalMaterial : public BaseMaterial
	{
	  public:
		MetalMaterial( const std::string & p_name,
					   const Vec3f &	   p_diffuse,
					   const float		   p_sigma,
					   const float		   p_metalness,
					   const Vec3f &	   p_f0 )
			: BaseMaterial( p_name ), _brdfCookTorrance( p_diffuse, p_sigma, p_f0 ),
			  _brdfOrenNayar( p_diffuse, p_sigma ), _metalness( p_metalness )
		{
		}

		virtual ~MetalMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			const Vec3f wo = -p_ray.getDirection();
			const Vec3f wi = p_lightSample._direction;

			return ( 1 - _metalness ) * _brdfOrenNayar.evaluate( wo, wi, p_hitRecord._normal )
				   + _metalness * _brdfCookTorrance.evaluate( wo, wi, p_hitRecord._normal );
		}

		inline const Vec3f & getFlatColor() const override { return _brdfCookTorrance.getKd(); }

	  protected:
		CookTorranceBRDF _brdfCookTorrance;
		OrenNayarBRDF	 _brdfOrenNayar;
		float			 _metalness;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_METAL_MATERIAL__
