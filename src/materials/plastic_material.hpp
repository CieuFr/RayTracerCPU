#ifndef __RT_ISICG_PLASTIC_MATERIAL__
#define __RT_ISICG_PLASTIC_MATERIAL__

#include "base_material.hpp"
#include "brdfs/lambert_brdf.hpp"
#include "brdfs/phong_brdf.hpp"

namespace RT_ISICG
{
	class PlasticMaterial : public BaseMaterial
	{
	  public:
		PlasticMaterial( const std::string & p_name, const Vec3f & p_diffuse, const float p_shininess )
			: BaseMaterial( p_name ), _brdfLambert( p_diffuse ), _brdfPhong( p_diffuse, p_shininess )
		{
		}

		virtual ~PlasticMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			const Vec3f wo = -p_ray.getDirection();
			const Vec3f wi = p_lightSample._direction;

			return ( ( 0.3f * _brdfPhong.evaluate( wo, wi, p_hitRecord._normal ) )
					 + ( 0.7f * _brdfLambert.evaluate() ) );
			// return _brdfPhong.evaluate( wo, wi, p_hitRecord._normal );
			//return Vec3f(0.f);
		}

		inline const Vec3f & getFlatColor() const override { return _brdfLambert.getKd(); }

	  protected:
		PhongBRDF	_brdfPhong;
		LambertBRDF _brdfLambert;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_MATTE_MATERIAL__
