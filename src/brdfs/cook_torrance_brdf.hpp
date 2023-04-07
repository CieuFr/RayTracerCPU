#ifndef __RT_ISICG_BRDF_COOK_TORRANCE__
#define __RT_ISICG_BRDF_COOK_TORRANCE__

#include "defines.hpp"

namespace RT_ISICG
{
	class CookTorranceBRDF
	{
	  public:
		CookTorranceBRDF( const Vec3f & p_kd, const float p_sigma, const Vec3f & p_f0) : _kd( p_kd ), _sigma( p_sigma ), _f0(p_f0) {};

		inline Vec3f evaluate( const Vec3f & p_wo, const Vec3f & p_wi, const Vec3f & p_n ) const
		{
			float alpha = _sigma * _sigma;
			Vec3f h		= normalize( p_wo + p_wi );
			return (D(h,p_n) * F(p_wo,h) * G(dot(p_n,p_wo)) * G(dot(p_n,p_wi))) / (4.f*dot(p_wo,p_n)*dot(p_wi,p_n));
		}
		
		inline const float D( const Vec3f & p_h, const Vec3f & p_n ) const { 
			float alpha = _sigma * _sigma;
			return (alpha / (PIf*pow(pow(dot(p_n,p_h),2.f)*(alpha*alpha-1.f)+1.f,2.f)));
		}

		inline const float G( const float x ) const {
			float k = pow( _sigma + 1.f, 2.f ) / 8.f;
			return (x / ( x * ( 1.f - k ) + k ));
		}
		
		inline const Vec3f F( const Vec3f & p_wo, const Vec3f & p_h ) const
		{
			return _f0 + ( 1.f - _f0 ) * pow(  1.f -  dot( p_h, p_wo ), 5.f );
		}
		
		inline const Vec3f & getKd() const { return _kd; }

	  private:
		Vec3f _kd = WHITE;
		float _sigma;
		Vec3f _f0;
	};
} // namespace RT_ISICG

#endif // __RT_ISICG_BRDF_TORRANCE__
