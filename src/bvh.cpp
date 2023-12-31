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

		_root = new BVHNode();

		Chrono chr;
		chr.start();

		_buildRec( _root, 0, p_triangles->size(), 0 );

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		if ( !_root->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) return false;
		return _intersectRec( _root, p_ray, p_tMin, p_tMax, p_hitRecord );
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		return _intersectAnyRec( _root, p_ray, p_tMin, p_tMax );
	}

	bool isBehindAxis( const TriangleMeshGeometry & triangle, Vec3f _milieu ) { return true; }

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		for ( size_t i = p_firstTriangleId; i < p_lastTriangleId; i++ )
		{
			p_node->_aabb.extend( _triangles->at( i ).getAABB() );
		}

		p_node->_firstTriangleId = p_firstTriangleId;
		p_node->_lastTriangleId	 = p_lastTriangleId;

		if ( !( ( p_depth >= _maxDepth ) || ( ( p_lastTriangleId - p_firstTriangleId ) <= _maxTrianglesPerLeaf ) ) )
		{
			p_node->_left  = new BVHNode();
			p_node->_right = new BVHNode();
			size_t axe	   = p_node->_aabb.largestAxis();
			Vec3f centre   = p_node->_aabb.centroid();

			float milieu = centre[ axe ];

			std::vector<TriangleMeshGeometry>::iterator it;
			it			 = std::partition( _triangles->begin() + p_firstTriangleId,
								   _triangles->begin() + p_lastTriangleId,
								   [ milieu, axe ]( const TriangleMeshGeometry & triangle )
								   { return triangle.getAABB().centroid()[ axe ] < milieu; } );
			int cutIndex = int(it - _triangles->begin());
			_buildRec( p_node->_left, p_firstTriangleId, cutIndex, p_depth + 1 );
			_buildRec( p_node->_right, cutIndex, p_lastTriangleId, p_depth + 1 );
		}
	}

	bool BVH::_intersectRec( const BVHNode * p_node,
							 const Ray &	 p_ray,
							 const float	 p_tMin,
							 const float	 p_tMax,
							 HitRecord &	 p_hitRecord ) const
	{
		if ( p_node->isLeaf() )
		{
			float tClosest = p_tMax;
			float u;
			float v;
			if ( p_hitRecord._object != nullptr ) { tClosest = p_hitRecord._distance; }

			size_t hitTri		 = _triangles->size(); // Hit triangle id.
			Vec3f  normalClosest = VEC3F_ZERO;
			for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
			{
				float t;
				
				Vec3f n;

				if ( _triangles->at( i ).intersect( p_ray, t, n,u,v ) )
				{
					if ( t >= p_tMin && t <= tClosest )
					{
						tClosest	  = t;
						hitTri		  = i;
						normalClosest = n;
					}
				}
			}
			if ( hitTri != _triangles->size() ) // Intersection found.
			{
				p_hitRecord._point	= p_ray.pointAtT( tClosest );
				p_hitRecord._normal = normalClosest;
				p_hitRecord.faceNormal( p_ray.getDirection() );
				p_hitRecord._uv		  = Vec2f( u, v );
				p_hitRecord._distance = tClosest;
				p_hitRecord._object	  = reinterpret_cast<BaseObject *>( _triangles->at( hitTri ).getRefMesh() );

				return true;
			}

			return false;
		}

		bool returnValueLeft  = false;
		bool returnValueRight = false;

		if ( p_node->_left->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
		{
			returnValueLeft = _intersectRec( p_node->_left, p_ray, p_tMin, p_tMax, p_hitRecord );
		}

		if ( p_node->_right->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
		{
			returnValueRight = _intersectRec( p_node->_right, p_ray, p_tMin, p_tMax, p_hitRecord );
		}

		return ( returnValueLeft || returnValueRight );
	}

	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		if ( !p_node->_aabb.intersect( p_ray, p_tMin, p_tMax ) ) return false;

		if ( p_node->isLeaf() )
		{
			Vec3f normal;
			float u;
			float v;
			for ( size_t i = p_node->_firstTriangleId; i < p_node->_lastTriangleId; i++ )
			{
				float t;
				if ( _triangles->at( i ).intersect( p_ray, t, normal,u,v ) )
				{
					if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
				}
			}
			return false;
		}

		return _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax )
			   || _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax );

	} // namespace RT_ISICG

} // namespace RT_ISICG