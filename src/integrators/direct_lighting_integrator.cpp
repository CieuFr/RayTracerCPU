#include "direct_lighting_integrator.hpp"

namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
										const Ray &	  p_ray,
										const float	  p_tMin,
										const float	  p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			return _directLighting(
				hitRecord._object->getMaterial()->getFlatColor(), p_scene, hitRecord._point, hitRecord._normal );
		}
		else
		{
			return _backgroundColor;
		}
	}

	Vec3f DirectLightingIntegrator::_directLighting( const Vec3f   p_color,
													 const Scene & p_scene,
													 const Vec3f   p_point,
													 const Vec3f   p_normal ) const
	{
		Vec3f liColor = Vec3f( 0 );

		for ( BaseLight * light : p_scene.getLights() )
		{
			LightSample lightSample = light->sample( p_point );
			Ray			shadowRay	= Ray( p_point, lightSample._direction );
			shadowRay.offset( p_normal );

			float cosTheta	   = glm::dot( lightSample._direction, p_normal );
			float maxThetaZero = glm::max( cosTheta, 0.0f );

			if ( !p_scene.intersectAny( shadowRay, 0, lightSample._distance ) )
			{
				liColor += p_color * lightSample._radiance * maxThetaZero;
			}
		}
		return liColor;
	}

} // namespace RT_ISICG
