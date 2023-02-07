#include "ray_cast_integrator.hpp"

namespace RT_ISICG
{
	Vec3f RayCastIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			float cosTheta		   = glm::dot( p_ray.getDirection(), hitRecord._normal );
			float maxThetaZero = glm::max( 1 - cosTheta, 0.0f );
			return hitRecord._object->getMaterial()->getFlatColor() * maxThetaZero;
		}
		else
		{
			return _backgroundColor;
		}
	}
} // namespace RT_ISICG
