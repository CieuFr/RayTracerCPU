#include "whitted_integrator.hpp"

namespace RT_ISICG
{
	Vec3f WhittedIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		return _recursiveLighting( p_scene, p_ray, p_tMin, p_tMax, 0, false );
	}

	Vec3f WhittedIntegrator::_recursiveLighting( const Scene & p_scene,
												 const Ray &   p_ray,
												 const float   p_tMin,
												 const float   p_tMax,
												 const int	   p_currentNumberOfBounce,
												 const bool	   p_insideMaterial ) const
	{
		if ( p_currentNumberOfBounce > _maxNbOfBounces ) return BLACK;
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			if ( hitRecord._object->getMaterial()->isMirror() )
			{
				const Vec3f reflectedDirection = normalize( glm::reflect( p_ray.getDirection(), hitRecord._normal ) );
				Ray			reflectedRay	   = Ray( hitRecord._point, reflectedDirection );
				reflectedRay.offset( hitRecord._normal );
				return _recursiveLighting( p_scene,
										   reflectedRay,
										   0,
										   p_tMax,
										   p_currentNumberOfBounce + 1,
										   p_insideMaterial );
			}
			else if ( hitRecord._object->getMaterial()->isTransparent() )
			{
				// https://en.wikipedia.org/wiki/Schlick%27s_approximation
				float			 n1				= 1;
				float n2				= hitRecord._object->getMaterial()->getIOR();
				const float		 eta			= p_insideMaterial ? ( n2 / n1 ) : ( n1 / n2 );
				float			 cosTheta		= glm::max(dot( -p_ray.getDirection(), hitRecord._normal ),0.f);
				float			 r0				= pow( ( n1 - n2 ) / ( n1 + n2 ), 2.f );
				float			 reflectionCoef = r0 + ( 1 - r0 ) * pow( ( 1 - cosTheta ), 5.f );

				const Vec3f reflectedDirection = glm::reflect( p_ray.getDirection(), hitRecord._normal );
				Ray			reflectedRay	   = Ray( hitRecord._point, reflectedDirection );
				reflectedRay.offset( hitRecord._normal );
				Vec3f reflectedColor = _recursiveLighting( p_scene,
														   reflectedRay,
														   0,
														   p_tMax,
														   p_currentNumberOfBounce + 1,
														   p_insideMaterial );
				if ( reflectionCoef >= 1 ) { return reflectedColor; }
				const Vec3f refractedDirection = glm::refract( p_ray.getDirection(), hitRecord._normal, eta );
				Ray			refractedRay	   = Ray( hitRecord._point, refractedDirection );
				refractedRay.offset( -hitRecord._normal );
				Vec3f refractedColor = _recursiveLighting( p_scene,
														   refractedRay,
														   0,
														   p_tMax,
														   p_currentNumberOfBounce + 1,
														   !p_insideMaterial );

				return reflectionCoef * reflectedColor + ( 1 - reflectionCoef ) * refractedColor;
			}
			else { return _directLighting( p_scene, hitRecord, p_ray ); }
		}
		else { return _backgroundColor; }
	}

	Vec3f WhittedIntegrator::_directLighting( const Scene &		p_scene,
											  const HitRecord & p_hitRecord,
											  const Ray &		p_ray ) const
	{
		Vec3f liColor = VEC3F_ZERO;
		for ( BaseLight * light : p_scene.getLights() )
		{
			Vec3f currentColor		 = VEC3F_ZERO;
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
					currentColor += p_hitRecord._object->getMaterial()->shade( p_ray, p_hitRecord, lightSample )
							   * lightSample._radiance * maxThetaZero;
				}
			}

			liColor += ( currentColor / float(numberOfShadowRays) );
		}

		liColor /= p_scene.getLights().size();

		return liColor;
	}

} // namespace RT_ISICG
