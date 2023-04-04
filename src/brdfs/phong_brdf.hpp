#ifndef __RT_ISICG_BRDF_PHONG__
#define __RT_ISICG_BRDF_PHONG__

#include "defines.hpp"

namespace RT_ISICG
{
	class PhongBRDF
	{
	  public:
		PhongBRDF( const Vec3f & p_kd, const float p_s ) : _kd( p_kd ), _s(p_s) {};

		
		inline Vec3f evaluate() const { return _kd * INV_PIf; }

		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		float _s;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_PHONG__
