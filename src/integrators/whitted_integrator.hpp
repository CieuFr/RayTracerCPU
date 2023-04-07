#ifndef __RT_ISICG_WHITTED_INTEGRATOR__
#define __RT_ISICG_WHITTED_INTEGRATOR__

#include "base_integrator.hpp"

namespace RT_ISICG
{
	class WhittedIntegrator : public BaseIntegrator
	{
	  public:
		WhittedIntegrator() : BaseIntegrator() {}
		virtual ~WhittedIntegrator() = default;

		const IntegratorType getType() const override { return IntegratorType::DIRECT_LIGHTING; }

		// Return incoming luminance.
		Vec3f Li( const Scene & p_scene, const Ray & p_ray, const float p_tMin, const float p_tMax ) const override;

		Vec3f _directLighting( const Scene & p_lightList, const HitRecord & p_hitRecord, const Ray & p_ray ) const;

		Vec3f _recursiveLighting( const Scene &	  p_scene,
										const Ray &		  p_ray,
										const float		  p_tMin,
										const float		  p_tMax,
										const int		  p_currentNumberOfBounce,
										const float previousMaterialIOR) const;

	  private:
		int _maxNbOfBounces = 5;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_WHITTED_INTEGRATOR__
