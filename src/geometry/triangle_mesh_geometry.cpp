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
	}

	// cf https://www.youtube.com/watch?v=fK1RPmF_zjQ and
	// https://cadxfem.org/inf/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf

	bool TriangleMeshGeometry::intersect( const Ray & p_ray, float & p_t, Vec3f & normal ) const
	{



		// t = distance to the plane
		// u , v = coordinate inside the triangle
		const Vec3f & origin	= p_ray.getOrigin();
		const Vec3f & direction = p_ray.getDirection();
		const Vec3f & v0		= _refMesh->_vertices[ _v0 ];
		const Vec3f & v1		= _refMesh->_vertices[ _v1 ];
		const Vec3f & v2		= _refMesh->_vertices[ _v2 ];

		Vec3f edge1;
		Vec3f edge2;
		Vec3f tvec;
		Vec3f pvec;
		Vec3f qvec;
		float t;
		float u;
		float v;

		float det;
		float inv_det;

		/* find vectors for two edges sharing vert */
		edge1 = ( v1 - v0 );
		edge2 = ( v2 - v0 );

		/* begin calculating determinant also used to calculate U parameter */
		pvec = glm::cross( direction, edge2 );

		/* if determinant is near zeroray lies in plane of triangle */
		det = glm::dot( edge1, pvec );

		if (_testCulling) {

			if ( det < _epsilon ) return false;
			/* calculate distance from vert to ray origin */
			tvec = ( origin - v0 );

			/* calculate U parameter and test bounds*/

			u = glm::dot( tvec, pvec );
			if ( u < 0.0 || u > det ) return false;

			/*prepare to test V parameter*/
			qvec = glm::cross( tvec, edge1 );

			/*calculate V parameter and test bounds*/
			v = dot( direction, qvec );
			if ( ( v < 0.0 ) || ( ( u + v ) > det ) ) return false;

			/*caculate t , scale parameters, ray intersects triangle*/
			t = dot( edge2, qvec );

			inv_det = ( 1.0 / det );
			t *= inv_det;
			u *= inv_det;
			v *= inv_det;

		} else {

			if ( det > -_epsilon && det < _epsilon ) return 0;

			inv_det = ( 1.0 / det );

			/*calculate distance from v0 to origin*/
			tvec = ( origin - v0 );

			/*calculate u parameter and test bounds*/
			u = dot( tvec, pvec ) * inv_det;
			if ( ( u < 0.0 ) || ( u > 1.0 ) ) return false;

			/*prepare to test V parameter*/
			qvec = glm::cross(tvec, edge1 );

			/*calculate V parameter and test bounds*/

			v = dot( direction, qvec ) * inv_det;
			if ( ( v < 0.0 ) || ( ( u + v ) > 1.0 ) ) return false;

			/*calculate t , ray intersects triangle*/
			t = dot( edge2, qvec ) * inv_det;

		}

		Vec3f n0 = _refMesh->_normals[ _v0 ];
		Vec3f n1 = _refMesh->_normals[ _v1 ];
		Vec3f n2 = _refMesh->_normals[ _v2 ];
		
		normal = ( 1 - u - v ) * n0 + u * n1 + v * n2;
		p_t = t;
		return true;
	}

} // namespace RT_ISICG
