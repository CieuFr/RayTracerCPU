#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__

#include "defines.hpp"

namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF( const Vec3f & p_ks, const float p_shininess ) : _ks( p_ks ), _shininess( p_shininess ) {};

		inline Vec3f evaluate( const Vec3f & p_wo, const Vec3f & p_wi, const Vec3f & p_n ) const
		{
			float cosAlpha = 0.f;
			if ( _blinnphong )
			{
				Vec3f h	 = normalize( p_wo + p_wi );
				cosAlpha = glm::max( 0.f, glm::dot( p_n, h ) );
			}
			else
			{
				Vec3f wr = glm::reflect( -p_wi, p_n );
				cosAlpha = glm::max( 0.f, glm::dot( p_wo, wr ) );
			}
			float cosThetaI = glm::max( 0.f, glm::dot( p_n, p_wi ) );
			
			return ( _ks / cosThetaI ) * pow( cosAlpha, _shininess );
		}

		inline const Vec3f & getKs() const { return _ks; }

	  private:
		Vec3f _ks		  = WHITE;
		bool  _blinnphong = true;
		float _shininess;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_PHONG__
