#include "whitted_integrator.hpp"

namespace RT_ISICG
{
	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		return _recursiveLighting( p_scene, p_ray, p_tMin, p_tMax, 0, 1 );
	}

	Vec3f WhittedIntegrator::_recursiveLighting( const Scene & p_scene,
												 const Ray &   p_ray,
												 const float   p_tMin,
												 const float   p_tMax,
												 const int	   p_currentNumberOfBounce,
												 const float   p_previousMaterialIOR ) const
	{
		if ( p_currentNumberOfBounce > _maxNbOfBounces ) return BLACK;
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			if ( hitRecord._object->getMaterial()->isMirror() )
			{
				const Vec3f reflectedDirection = normalize( glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				const Ray	reflectedRay	   = Ray( hitRecord._point, reflectedDirection );
				return _recursiveLighting( p_scene,
										   reflectedRay,
										   0,
										   p_tMax,
										   p_currentNumberOfBounce + 1,
										   hitRecord._object->getMaterial()->getIOR() );
			}
			else if ( hitRecord._object->getMaterial()->isTransparent())
			{
				const Vec3f reflectedDirection = glm::reflect( p_ray.getDirection(), hitRecord._normal );
				const Ray	reflectedRay	   = Ray( hitRecord._point, reflectedDirection );
				const float eta				   = ( p_previousMaterialIOR / hitRecord._object->getMaterial()->getIOR() );
				const Vec3f refractedDirection = glm::refract( p_ray.getDirection(), hitRecord._normal, eta );
				const Ray	refractedRay	   = Ray( hitRecord._point, refractedDirection );
				return WHITE;
			}
			else { return _directLighting( p_scene, hitRecord, p_ray ); }
		}
		else { return _backgroundColor; }
	}

	Vec3f WhittedIntegrator::_directLighting( const Scene &		p_scene,
											  const HitRecord & p_hitRecord,
											  const Ray &		p_ray ) const
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
