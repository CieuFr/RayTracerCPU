#include "plane.hpp"

namespace RT_ISICG
{
	//https://gamedev.stackexchange.com/questions/172352/finding-texture-coordinates-for-plane
	const Vec2f Plane::getUV( const Vec3f & p_point ) const
	{
		Vec3f normal = _geometry.getNormal();
		Vec3f e1	 = glm::normalize( glm::cross( normal, Vec3f( 1, 0, 0 ) ) );

		// If normal and (1,0,0) are parallel, change e1
		if ( e1 == VEC3F_ZERO ) { 
			e1 = glm::normalize( glm::cross( normal, Vec3f( 0, 0, 1 ) ) );
		}

		Vec3f e2 = glm::normalize( glm::cross( normal,e1 ) );
		float u	 = glm::dot( e1, p_point );
		float v	 = glm::dot( e2, p_point );

		return Vec2f( u, v );
	}

	bool Plane::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float t;

		if ( _geometry.intersect( p_ray, t ) )
		{
			if ( t < p_tMin || t > p_tMax ) { return false; } // not in range

			// Intersection found, fill p_hitRecord.
			p_hitRecord._point	= p_ray.pointAtT( t );
			p_hitRecord._normal = _geometry.computeNormal( p_hitRecord._point );
			p_hitRecord.faceNormal( p_ray.getDirection() );
			p_hitRecord._distance = t;
			p_hitRecord._object	  = this;

			return true;
		}
		return false;
	}

	bool Plane::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		float t;

		if ( _geometry.intersect( p_ray, t ) )
		{
			if ( t < p_tMin || t > p_tMax ) { return false; } // not in range
			return true;
		}
		return false;
	}

} // namespace RT_ISICG
