#ifndef __RT_ISICG_METAL_MATERIAL__
#define __RT_ISICG_METAL_MATERIAL__

#include "base_material.hpp"
#include "brdfs/cook_torrance_brdf.hpp"

namespace RT_ISICG
{
	class MetalMaterial : public BaseMaterial
	{
	  public:
		MetalMaterial( const std::string & p_name, const Vec3f & p_diffuse, const float p_sigma )
			: BaseMaterial( p_name ), _brdf( p_diffuse )
		{
		}

		virtual ~MetalMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			

			return _brdf.evaluate( );
		}

		inline const Vec3f & getFlatColor() const override { return _brdf.getKd(); }

	  protected:
		CookTorranceBRDF _brdf;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_METAL_MATERIAL__
