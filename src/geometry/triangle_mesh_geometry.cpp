#include "triangle_mesh_geometry.hpp"
#include "objects/triangle_mesh.hpp"

namespace RT_ISICG
{
	TriangleMeshGeometry::TriangleMeshGeometry( const unsigned int p_v0,
												const unsigned int p_v1,
												const unsigned int p_v2,
												MeshTriangle *	   p_refMesh )
		: _v0( p_v0 ), _v1( p_v1 ), _v2( p_v2 ), _refMesh( p_refMesh )
	{
		_faceNormal = glm::normalize( glm::cross( _refMesh->_vertices[ p_v1 ] - _refMesh->_vertices[ p_v0 ],
												  _refMesh->_vertices[ p_v2 ] - _refMesh->_vertices[ p_v0 ] ) );
		_aabb.extend( Vec3f( _refMesh->_vertices[ p_v0 ] ));
		_aabb.extend( Vec3f( _refMesh->_vertices[ p_v1 ] ));
		_aabb.extend( Vec3f( _refMesh->_vertices[ p_v2 ] ));

	}

	// cf https://www.youtube.com/watch?v=fK1RPmF_zjQ and
	// https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf

	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t, Vec3f & p_n, float & p_u, float & p_v ) const
	{
		const Vec3f & o	 = p_ray.getOrigin();
		const Vec3f & d	 = p_ray.getDirection();
		const Vec3f & v0 = _refMesh->_vertices[ _v0 ];
		const Vec3f & v1 = _refMesh->_vertices[ _v1 ];
		const Vec3f & v2 = _refMesh->_vertices[ _v2 ];

		Vec3f edge1, edge2, tvec, pvec, qvec;
		float det, inv_det, t, u, v;
		edge1 = v1 - v0;
		edge2 = v2 - v0;
		pvec  = glm::cross( d, edge2 );
		det	  = glm::dot( edge1, pvec );

		if ( _testCulling )
		{
			if ( det < _epsilon ) return false;

			tvec = o - v0;
			u	 = glm::dot( tvec, pvec );
			if ( u < 0 || u > det ) return false;

			qvec = glm::cross( tvec, edge1 );
			v	 = glm::dot( d, qvec );
			if ( v < 0 || u + v > det ) return false;

			t		= glm::dot( edge2, qvec );
			inv_det = 1.f / det;
			t *= inv_det;
			u *= inv_det;
			v *= inv_det;
		}
		else
		{
			if ( det > -_epsilon && det < _epsilon ) return 0;
			inv_det = 1.f / det;
			tvec	= o - v0;
			u		= glm::dot( tvec, pvec ) * inv_det;
			if ( u < 0 || u > 1 ) return false;
			qvec = glm::cross( tvec, edge1 );
			v	 = glm::dot( d, qvec ) * inv_det;
			if ( v < 0 || u + v > 1 ) return false;
			t = glm::dot( edge2, qvec ) * inv_det;
		}
		p_t		 = t;
		p_u		 = u;
		p_v		 = v;
		Vec3f n0 = _refMesh->_normals[ _v0 ];
		Vec3f n1 = _refMesh->_normals[ _v1 ];
		Vec3f n2 = _refMesh->_normals[ _v2 ];
		Vec3f n	 = ( 1 - u - v ) * n0 + u * n1 + v * n2;
		p_n		 = n;

		return true;
	}
} // namespace RT_ISICG
