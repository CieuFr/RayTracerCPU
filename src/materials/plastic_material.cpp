#ifndef __RT_ISICG_METAL_MATERIAL__
#define __RT_ISICG_METAL_MATERIAL__

#include "base_material.hpp"
#include "brdfs/phong_brdf.hpp"
#include "brdfs/lambert_brdf.hpp"


namespace RT_ISICG
{
	class MetalMaterial : public BaseMaterial
	{
	  public:
		MetalMaterial( const std::string & p_name, const Vec3f & p_diffuse, const float s )
			: BaseMaterial( p_name ), _brdfLambert( p_diffuse ), _brdfPhong(p_diffuse,s)
		{
		}

		virtual ~MetalMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			

			return _brdfPhong.evaluate( );
		}

		inline const Vec3f & getFlatColor() const override { return _brdfLambert.getKd(); }

	  protected:
		PhongBRDF _brdfPhong;
		LambertBRDF _brdfLambert;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_MATTE_MATERIAL__
