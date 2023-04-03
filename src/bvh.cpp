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

		_buildRec( _root, 0, p_triangles->size() - 1, 0 );

		chr.stop();

		std::cout << "[DONE]: " << chr.elapsedTime() << "s" << std::endl;
	}

	bool BVH::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		_intersectRec( _root, p_ray, p_tMin, p_tMax, p_hitRecord );
		return false;
	}

	bool BVH::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		_intersectAnyRec( _root, p_ray, p_tMin, p_tMax );
		return false;
	}

	bool isBehindAxis( const TriangleMeshGeometry & triangle, Vec3f _milieu ) { return true; }

	void BVH::_buildRec( BVHNode *			p_node,
						 const unsigned int p_firstTriangleId,
						 const unsigned int p_lastTriangleId,
						 const unsigned int p_depth )
	{
		

		for ( size_t i = p_firstTriangleId; i <= p_lastTriangleId; i++ )
		{
			
			p_node->_aabb.extend( _triangles->at( i ).getAABB() );
		}

		if ( !( ( p_depth >= _maxDepth ) || ( ( p_lastTriangleId - p_firstTriangleId ) <= _maxTrianglesPerLeaf ) ) )
		{
			p_node->_left  = new BVHNode();
			p_node->_right = new BVHNode();	
			float axe	 = p_node->_aabb.largestAxis();
			Vec3f centre = p_node->_aabb.centroid();

			float _milieu = centre[ axe ];

			std::vector<TriangleMeshGeometry>::iterator it;
			it			 = std::partition( _triangles->begin() + p_firstTriangleId,
								   _triangles->begin() + p_lastTriangleId,
								   [ _milieu, axe ]( const TriangleMeshGeometry & triangle )
								   { return triangle.getAABB().centroid()[ axe ] < _milieu; } );
			int cutIndex = it - _triangles->begin();
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
			float  tClosest		 = p_tMax;			   // Hit distance.
			size_t hitTri		 = _triangles->size(); // Hit triangle id.
			Vec3f  normalClosest = VEC3F_ZERO;
			for ( size_t i = p_node->_firstTriangleId; i <= p_node->_lastTriangleId; i++ )
			{
				float t;
				Vec3f n;

				if ( _triangles->at( i ).intersect( p_ray, t, n ) )
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
				p_hitRecord._distance = tClosest;
				//MeshTriangle * mesh	  = _triangles->at( hitTri ).getRefMesh();
				//BaseObject *		 pBaseObject = dynamic_cast<BaseObject*>( mesh );
				// TODO DECOMMENT 
				//p_hitRecord._object = _triangles->at( hitTri ).getRefMesh();				


				return true;
			}

			return false;
		}

		bool returnValue = false;
		if ( p_node->_left->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
		{
			returnValue = returnValue || _intersectRec( p_node->_left, p_ray, p_tMin, p_tMax, p_hitRecord );
		}

		if ( p_node->_right->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
		{
			returnValue = returnValue || _intersectRec( p_node->_right, p_ray, p_tMin, p_tMax, p_hitRecord );
		}

		return returnValue;
	}

	bool BVH::_intersectAnyRec( const BVHNode * p_node,
								const Ray &		p_ray,
								const float		p_tMin,
								const float		p_tMax ) const
	{
		if ( p_node->isLeaf() )
		{
			Vec3f normal;
			for ( size_t i = p_node->_firstTriangleId; i <= p_node->_lastTriangleId; i++ )
			{
				float t;
				if ( _triangles->at( i ).intersect( p_ray, t, normal ) )
				{
					if ( t >= p_tMin && t <= p_tMax ) return true; // No need to search for the nearest.
				}
			}
			return false;
		}

		if ( p_node->_left->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
		{
			return _intersectAnyRec( p_node->_left, p_ray, p_tMin, p_tMax );
		}

		if ( p_node->_right->_aabb.intersect( p_ray, p_tMin, p_tMax ) )
		{
			return _intersectAnyRec( p_node->_right, p_ray, p_tMin, p_tMax );
		}

		return false;

	} // namespace RT_ISICG

} // namespace RT_ISICG