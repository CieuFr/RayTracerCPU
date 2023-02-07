#include "sphere_geometry.hpp"

namespace RT_ISICG
{
	bool SphereGeometry::intersect( const Ray & p_ray, float & p_t1, float & p_t2 ) const
	{

		Vec3f oc = p_ray.getOrigin() - _center;
		// Direction normalisée donc a = 1
		// float a = dot(r.direction, r.direction);
		float b		= 2 * glm::dot( oc, p_ray.getDirection() );
		float c		= glm::dot( oc, oc ) - _radius * _radius;
		float delta = b * b - 4 * c;
		if ( delta >= 0 )
		{
			p_t1 = ( -b - sqrt( delta ) ) / 2;
			p_t2 = p_t1;
			if ( p_t1 > 0 ) { return true; }
			else
			{ // On est peut-etre à l'interieur de la sphère
				p_t2 = ( -b + sqrt( delta ) ) / 2;
				return true;
			}
		}

		return false;
	}

} // namespace RT_ISICG
