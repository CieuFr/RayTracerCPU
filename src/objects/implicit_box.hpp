#ifndef __RT_ISICG_IMPLICIT_BOX__
#define __RT_ISICG_IMPLICIT_BOX__

#include "implicit_surface.hpp"

namespace RT_ISICG
{
	class ImplicitBox : public ImplicitSurface
	{
	  public:
		ImplicitBox()		  = delete;
		virtual ~ImplicitBox() = default;

		ImplicitBox( const std::string & p_name, const Vec3f & p_centre, const float p_s )
			: ImplicitSurface( p_name ), _centre( p_centre ), _s( p_s )
		{
		}

		float _sdf( const Vec3f & p_point ) const override { 

			Vec3f p = p_point - _centre;
			Vec3f q = abs( p ) - Vec3f(_s);

			return glm::length( glm::max( q, Vec3f( 0.f ) ) ) + glm::min( glm::max( q.x, glm::max( q.y, q.z ) ), 0.0f ); 
			//return glm::length( glm::max( glm::abs( _centre ) - _s, 0.f ) ); 
		
		}

	  private:
		// Signed Distance Function
		Vec3f _centre = VEC3F_ZERO;
		float _s;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_IMPLICIT_BOX__
