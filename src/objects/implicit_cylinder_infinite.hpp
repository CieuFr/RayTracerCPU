#ifndef __RT_ISICG_IMPLICIT_INFINITE_CYLINDER__
#define __RT_ISICG_IMPLICIT_INFINITE_CYLINDER__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitInfiniteCylinder : public ImplicitSurface
	{
	  public:
		ImplicitInfiniteCylinder()			= delete;
		virtual ~ImplicitInfiniteCylinder() = default;

		ImplicitInfiniteCylinder( const std::string & p_name, const Vec3f & p_centre, const Vec3f p_cylinder )
			: ImplicitSurface( p_name ), _centre( p_centre ), _cylinder( p_cylinder )
		{
		}

		float _sdf( const Vec3f & p_point ) const override
		{
			return length( Vec2f( _centre.x, _centre.z ) - Vec2f( _cylinder.x, _cylinder.y ) ) - _cylinder.z;
		}

	  private:
		// Signed Distance Function
		Vec3f _centre	= VEC3F_ZERO;
		Vec3f _cylinder = VEC3F_ZERO;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_INFINITE_CYLINDER__
