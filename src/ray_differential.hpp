#ifndef __RT_ISICG_RAY_DIFFERENTIAL_
#define __RT_ISICG_RAY_DIFFERENTIAL_

#include "defines.hpp"
#include "ray.hpp"

namespace RT_ISICG
{
	class RayDifferential : public Ray
	{
	  public:
		RayDifferential() = delete;
		// p_direction must be normalized
		RayDifferential( const Vec3f & p_origin, const Vec3f & p_direction )
			: Ray( p_origin,p_direction )
		{
		}

	  private:

	};
} // namespace RT_ISICG

#endif // __RT_ISICG_RAY_DIFFERENTIAL_
