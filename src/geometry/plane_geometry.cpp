#include "plane_geometry.hpp"

namespace RT_ISICG
{
	bool PlaneGeometry::intersect( const Ray & p_ray, float & p_t) const
	{
		p_t = -1;
		// On suppose les vecteurs normalises
		float den = glm::dot( _normal, p_ray.getDirection() );
		// Si le plan et le rayon ne sont pas parall�les
		if ( den != 0 )
		{ // Attention � la comparaison avec 0
			Vec3f po = _point - p_ray.getOrigin();
			p_t = glm::dot( po, _normal ) / den;
			return true;
		}
		// Sinon, pas d�intersection
		return false;
	}

} // namespace RT_ISICG
