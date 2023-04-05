#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__

#include "defines.hpp"

namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF( const Vec3f & p_ks, const float p_s ) : _ks( p_ks ), _s(p_s) {};

		
		inline Vec3f evaluate( const Vec3f & p_wo, const Vec3f & p_wi, const Vec3f & p_n ) const
		{ 
			Vec3f wr = glm::reflect( p_wi, p_n );
			float cosAlpha		= glm::dot( p_wo, wr );
			float cosThetaI = glm::dot(p_n, -p_wi );
			
			return ( _ks / cosThetaI ) *  pow( cosAlpha, _s );
			
		}

		inline const Vec3f & getKs() const { return _ks; }

	  private:
		Vec3f _ks = WHITE;
		float _s;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_PHONG__
