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
			return _directLighting( p_scene, hitRecord, p_ray );
		}
		else { return _backgroundColor; }
	}

	Vec3f DirectLightingIntegrator::_directLighting( const Scene &	   p_scene,
													 const HitRecord & p_hitRecord,
													 const Ray &	   p_ray ) const
	{
		Vec3f liColor = Vec3f( 0 );

		for ( BaseLight * light : p_scene.getLights() )
		{
			int numberOfShadowRays = 1;
			if ( light->getIsSurface() ) { numberOfShadowRays = _nbLightSamples; }

			for ( int i = 0; i < numberOfShadowRays; i++ )
			{
				LightSample lightSample = light->sample( p_hitRecord._point );
				Ray			shadowRay	= Ray( p_hitRecord._point, lightSample._direction );
				shadowRay.offset( p_hitRecord._normal );

				float cosTheta	   = glm::dot( lightSample._direction, p_hitRecord._normal );
				float maxThetaZero = glm::max( cosTheta, 0.0f );

				if ( !p_scene.intersectAny( shadowRay, 0, lightSample._distance ) )
				{
					liColor += p_hitRecord._object->getMaterial()->shade( p_ray, p_hitRecord, lightSample )
							   * lightSample._radiance * maxThetaZero;
				}
			}

			liColor /= numberOfShadowRays;
		}

		return liColor;
	}

} // namespace RT_ISICG
