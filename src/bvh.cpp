#include "bvh.hpp"
#include "geometry/triangle_mesh_geometry.hpp"
#include "utils/chrono.hpp"

namespace RT_ISICG
{
	void BVH::build( std::vector<TriangleMeshGeometry> * p_triangles )
	{
		std::cout << "Building BVH..." << std::endl;
		if ( p_triangles == nullptr || p_triangles->empty() )
		{
			throw std::exception( "BVH::build() error: no triangle provided" );
		}
		_triangles = p_triangles;

		Chrono chr;
		chr.start();

		_buildRec( _root, 0, p_triangles->size() - 1, 0 );

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		/// TODO
		return false;
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		/// TODO
		return false;
	}

	bool isBehindAxis( const TriangleMeshGeometry & triangle, Vec3f _milieu ) { return true; }

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		/*for ( size_t i = p_firstTriangleId; i <= p_lastTriangleId; i++ )
		{
			p_node->_aabb.extend( _triangles->at( i ).getAABB() );
		}

		if ( !( ( p_depth >= _maxDepth ) || ( ( p_lastTriangleId - p_firstTriangleId ) <= _maxTrianglesPerLeaf ) ) )
		{
			Vec3f axe = p_node->_aabb.largestAxis();
			Vec3f centre = p_node->_aabb.centroid();

			float _milieu = centre[axe];

			std::vector<TriangleMeshGeometry>::iterator it;
			it = std::partition( _triangles->begin() + p_first,
								 _triangles->begin() + p_last,
								 [ _milieu ]( const TriangleMeshGeometry & triangle )
								 { return isBehindAxis( triangle, _milieu ); } );
			it - begin 
		 size_t index = std::distance( _triangles.begin(), it );
			_buildRec( p_node->_left, p_firstTriangleId, , p_depth + 1 );
			_buildRec( p_node->_right,, p_lastTriangleId, p_depth + 1 );*/

		

		/*axePartition = plus grand axe de aabb
		milieu = centre de axePartition
		idPartition	= <partition( axePartition, milieu )>
		constr_rec_BVH( noeud.gauche, idPremierTri, idPartition )
		constr_rec_BVH( noeud.droit, idPartition, idDernierTri )*/
		/// TODO
	}

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		/// TODO
		return false;
	}
	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		/// TODO
		return false;
	}
} // namespace RT_ISICG
