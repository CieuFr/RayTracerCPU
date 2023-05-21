#ifndef __RT_ISICG_PERLIN_H
#define __RT_ISICG_PERLIN_H

#include "defines.hpp"
#include "utils/random.hpp"
//https://raytracing.github.io/books/RayTracingTheNextWeek.html#perlinnoise

namespace RT_ISICG
{
	class PerlinNoise
	{
	  public:
		PerlinNoise()
		{
			_ranvec	 = new Vec3f[ _point_count ];
			_ranfloat = new double[ _point_count ];
			for ( int i = 0; i < _point_count; ++i )
			{
				_ranvec[ i ] = Vec3f( randomFloat()*2-1 );
				
			}

			_perm_x = perlin_generate_perm();
			_perm_y = perlin_generate_perm();
			_perm_z = perlin_generate_perm();
		}

		~PerlinNoise()
		{
			delete[] _ranvec;
			delete[] _ranfloat;
			delete[] _perm_x;
			delete[] _perm_y;
			delete[] _perm_z;
		}

		float noise( const Vec3f & p_point ) const
		{
			auto u = p_point.x - floor( p_point.x );
			auto v = p_point.y - floor( p_point.y );
			auto w = p_point.z - floor( p_point.z );
			u	   = u * u * ( 3 - 2 * u );
			v	   = v * v * ( 3 - 2 * v );
			w	   = w * w * ( 3 - 2 * w );

			auto   i = static_cast<int>( floor( p_point.x ) );
			auto   j = static_cast<int>( floor( p_point.y ) );
			auto   k = static_cast<int>( floor( p_point.z ) );
			Vec3f c[ 2 ][ 2 ][ 2 ];

			for ( int di = 0; di < 2; di++ )
				for ( int dj = 0; dj < 2; dj++ )
					for ( int dk = 0; dk < 2; dk++ )
						c[ di ][ dj ][ dk ] = _ranvec[ _perm_x[ ( i + di ) & 255 ] ^ _perm_y[ ( j + dj ) & 255 ]
														^ _perm_z[ ( k + dk ) & 255 ] ];

			return perlin_interp( c, u, v, w );
		}

		float turb( const Vec3f & p_point, int p_depth = 7 ) const
		{
			float accum	= 0.0f;
			Vec3f temp_p = p_point;
			float weight = 1.0f;

			for ( int i = 0; i < p_depth; i++ )
			{
				accum += weight * noise( temp_p );
				weight *= 0.5f;
				temp_p *= 2.f;
			}

			return fabs( accum );
		}

	  private:
		Vec3f *			 _ranvec;
		static const int _point_count = 256;
		double *		 _ranfloat;
		int *			 _perm_x;
		int *			 _perm_y;
		int *			 _perm_z;

		static int * perlin_generate_perm()
		{
			auto p = new int[ _point_count ];

			for ( int i = 0; i < PerlinNoise::_point_count; i++ )
				p[ i ] = i;

			permute( p, _point_count );

			return p;
		}

		static void permute( int * p_p, int p_n )
		{
			for ( int i = p_n - 1; i > 0; i-- )
			{
				int target	= int(randomFloat());
				target		= target % ( i + 1 );
				int tmp		= p_p[ i ];
				p_p[ i ]		= p_p[ target ];
				p_p[ target ] = tmp;
			}
		}

		static float perlin_interp( Vec3f p_c[ 2 ][ 2 ][ 2 ], float p_u, float p_v, float p_w )
		{
			auto uu	   = p_u * p_u * ( 3 - 2 * p_u );
			auto vv	   = p_v * p_v * ( 3 - 2 * p_v );
			auto ww	   = p_w * p_w * ( 3 - 2 * p_w );
			auto accum = 0.0f;

			for ( int i = 0; i < 2; i++ )
				for ( int j = 0; j < 2; j++ )
					for ( int k = 0; k < 2; k++ )
					{
						Vec3f weight_v( p_u - i, p_v - j, p_w - k );
						accum += ( i * uu + ( 1 - i ) * ( 1 - uu ) ) * ( j * vv + ( 1 - j ) * ( 1 - vv ) )
								 * ( k * ww + ( 1 - k ) * ( 1 - ww ) ) * glm::dot( p_c[ i ][ j ][ k ], weight_v );
					}

			return accum;
		}

		
		

	};
} // namespace RT_ISICG

#endif