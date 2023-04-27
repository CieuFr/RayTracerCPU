#ifndef __RT_ISICG_IMPLICIT_DONUT__
#define __RT_ISICG_IMPLICIT_DONUT__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitDonut : public ImplicitSurface
	{
	  public:
		ImplicitDonut()			 = delete;
		virtual ~ImplicitDonut() = default;

		ImplicitDonut( const std::string & p_name, const Vec3f & p_centre, const Vec2f p_rayons )
			: ImplicitSurface( p_name ), _centre( p_centre ), _rayons( p_rayons )
		{
		}

		float _sdf( const Vec3f & p_point ) const override
		{
			Vec2f q = Vec2f( length( Vec2f( _centre.x, _centre.z ) ) - _rayons.x, _centre.y );
			return length( q ) - _rayons.y;
		}

	  private:
		// Signed Distance Function
		Vec3f _centre = VEC3F_ZERO;
		Vec2f _rayons;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_SPHERE__
