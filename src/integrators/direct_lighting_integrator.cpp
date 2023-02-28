#include "direct_lighting_integrator.hpp"

namespace RT_ISICG
{
	Vec3f DirectLightingIntegrator::Li( const Scene & p_scene,
								 const Ray &   p_ray,
								 const float   p_tMin,
								 const float   p_tMax ) const
	{
		HitRecord hitRecord;
		if ( p_scene.intersect( p_ray, p_tMin, p_tMax, hitRecord ) )
		{
			
		    float cosTheta	   = glm::dot( -p_ray.getDirection(), hitRecord._normal );
			float maxThetaZero = glm::max( cosTheta, 0.0f );
			/*return _directLighting2(
				maxThetaZero, hitRecord._object->getMaterial()->getFlatColor(), p_scene.getLights(), hitRecord._point );
			*/
			return _directLighting(
				maxThetaZero, hitRecord._object->getMaterial()->getFlatColor(), p_scene, hitRecord._point );
		
		}
		else { return _backgroundColor; }
	}
	
	Vec3f DirectLightingIntegrator::_directLighting( const float	 p_cosTheta,
													 const Vec3f	 p_color,
													 const Scene & p_scene,
													 const Vec3f	 p_point ) const
	{
		LightList _lightList = p_scene.getLights();
		Vec3f liColor = Vec3f( 0 );
		
		for ( BaseLight* light :  p_scene.getLights() ) 
		{

			liColor += p_color * light->sample( p_point )._radiance * p_cosTheta;
		}
		return liColor;
	}




} // namespace RT_ISICG
