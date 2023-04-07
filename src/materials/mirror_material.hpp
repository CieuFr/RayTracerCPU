#ifndef __RT_ISICG_MIRROR_MATERIAL__
#define __RT_ISICG_MIRROR_MATERIAL__

#include "base_material.hpp"
#include "brdfs/cook_torrance_brdf.hpp"
#include "brdfs/oren_nayar_brdf.hpp"

namespace RT_ISICG
{
	class MirrorMaterial : public BaseMaterial
	{
	  public:
		MirrorMaterial( const std::string & p_name)
			: BaseMaterial( p_name )
		{
		}

		virtual ~MirrorMaterial() = default;

		Vec3f shade( const Ray &		 p_ray,
					 const HitRecord &	 p_hitRecord,
					 const LightSample & p_lightSample ) const override
		{
			return BLACK;
		}

		const bool isMirror() const { return true; }

		inline const Vec3f & getFlatColor() const override { return BLACK; }

	  protected:
	
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_METAL_MATERIAL__
