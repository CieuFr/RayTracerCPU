#include "scene.hpp"
#include "materials/color_material.hpp"
#include "materials/lambert_material.hpp"
#include "materials/matte_material.hpp"
#include "materials/metal_material.hpp"
#include "materials/mirror_material.hpp"
#include "materials/plastic_material.hpp"
#include "materials/transparent_material.hpp"
#include "materials/texture_material.hpp"
#include "objects/plane.hpp"
#include "objects/sphere.hpp"
#include "objects/triangle_mesh.hpp"
#include "objects/implicit_sphere.hpp"
#include "objects/implicit_death_str.hpp"
#include "objects/implicit_torus.hpp"
#include "objects/implicit_cylinder_infinite.hpp"
#include "objects/implicit_box.hpp"
#include "textures/image_texture.hpp"
#include "textures/checker_texture.hpp"
#include "textures/noise_texture.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace RT_ISICG
{
	Scene::Scene() { _addMaterial( new ColorMaterial( "default", WHITE ) ); }

	Scene::~Scene()
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			delete object.second;
		}
		for ( const MaterialMapPair & material : _materialMap )
		{
			delete material.second;
		}
		for ( const BaseLight * light : _lightList )
		{
			delete light;
		}
	}

	void Scene::initTP1()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere1" );
	}
	void Scene::initTP2()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		_addMaterial( new ColorMaterial( "Red", RED ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		// Add Lights
		_addLight( new PointLight( Vec3f( 1, 10, 1 ), Vec3f( 1.f ), 100.f ) );
	}
	void Scene::initTP3()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		_addMaterial( new ColorMaterial( "Red", RED ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Sphere1" );
		_attachMaterialToObject( "Red", "Plane1" );

		// Add Lights
		_addLight( new QuadLight( Vec3f( 1, 10, 2 ), Vec3f( -2, 0, 0 ), Vec3f( 0, 0, 2 ), Vec3f( 1.f ), 40.f ) );
	}
	void Scene::initTP4()
	{
		_addMaterial( new ColorMaterial( "RedColor", RED ) );
		_addMaterial( new ColorMaterial( "GreenColor", GREEN ) );
		_addMaterial( new ColorMaterial( "BlueColor", BLUE ) );
		_addMaterial( new ColorMaterial( "GreyColor", GREY ) );
		_addMaterial( new ColorMaterial( "MagentaColor", MAGENTA ) );
		_addMaterial( new ColorMaterial( "YellowColor", YELLOW ) );
		_addMaterial( new ColorMaterial( "CyanColor", CYAN ) );
		// = = = = = = = = = Add objects . = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		loadFileTriangleMesh( "Bunny", DATA_PATH + "Bunny.obj" );
		_attachMaterialToObject( "CyanColor", "Bunny_defaultobject" );
		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( "PlaneGround", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( "GreyColor", "PlaneGround" );
		_addObject( new Plane( "PlaneLeft", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "RedColor", "PlaneLeft" );
		_addObject( new Plane( "PlaneCeiling", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( "GreenColor", "PlaneCeiling" );
		_addObject( new Plane( "PlaneRight", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( "BlueColor", "PlaneRight" );
		_addObject( new Plane( "PlaneFront", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( "MagentaColor", "PlaneFront" );
		_addObject( new Plane( "PlaneRear", Vec3f( 0.f, 0.f, -10.f ), Vec3f( 0.f, 0.f, 1.f ) ) );
		_attachMaterialToObject( "YellowColor", "PlaneRear" );
		// = = = = = = = = = Add lights . = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		_addLight( new PointLight( Vec3f( 0.f, 3.f, -5.f ), WHITE, 100.f ) );
	}
	void Scene::initTP5()
	{
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( -3.f, 3.f, 0.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_addObject( new Sphere( "Sphere2", Vec3f( 3.f, 3.f, 0.f ), 1.f ) );
		loadFileTriangleMesh( "Bunny", DATA_PATH + "Bunny.obj" );
		// Add materials.
		_addMaterial( new PlasticMaterial( "GreyPlastic", GREY, 8.f ) );
		_addMaterial( new PlasticMaterial( "RedPlastic", RED, 8.f ) );
		_addMaterial( new MatteMaterial( "BlueMatte", BLUE, 0.4f ) );
		_addMaterial( new MetalMaterial( "GoldenBunny",  YELLOW, 0.3f, 0.5f, Vec3f( 1.f, 0.85f,
		0.57f ))); 
		
		// Link objects and materials.
		_attachMaterialToObject( "GreyPlastic", "Sphere1" );
		_attachMaterialToObject( "RedPlastic", "Plane1" );
		_attachMaterialToObject( "BlueMatte", "Sphere2" );
		_attachMaterialToObject( "GoldenBunny", "Bunny_defaultobject");

		// Add Lights
		_addLight( new PointLight( Vec3f( 0.f, 5.f, -6.f ), WHITE, 250.f ) );

	}
	void Scene::initTP6()
	{
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// = = = = = = = = = = = Add materials . = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		_addMaterial( new MatteMaterial( " WhiteMatte ", WHITE, 0.6f ) );
		_addMaterial( new MatteMaterial( " RedMatte ", RED, 0.6f ) );
		_addMaterial( new MatteMaterial( " GreenMatte ", GREEN, 0.6f ) );
		_addMaterial( new MatteMaterial( " BlueMatte ", BLUE, 0.6f ) );
		_addMaterial( new MatteMaterial( " GreyMatte ", GREY, 0.6f ) );
		_addMaterial( new MatteMaterial( " MagentaMatte ", MAGENTA, 0.6f ) );

		_addMaterial( new MirrorMaterial( "Mirror" ) );
		_addMaterial( new TransparentMaterial( "Transparent" ) );

		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// = = = = = = = = = = = =
		// Add objects .
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// = = = = = = = = = = = = =
		// Spheres .
		_addObject( new Sphere( " Sphere1 ", Vec3f( -2.f, 0.f, 3.f ), 1.5f ) );
		_attachMaterialToObject( "Mirror", " Sphere1 " );
		_addObject( new Sphere( " Sphere2 ", Vec3f( 2.f, 0.f, 3.f ), 1.5f ) );
		_attachMaterialToObject( "Transparent", " Sphere2 " );
		// Pseudo Cornell box made with infinite planes .
		_addObject( new Plane( " PlaneGround ", Vec3f( 0.f, -3.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_attachMaterialToObject( " GreyMatte ", " PlaneGround " );
		_addObject( new Plane( " PlaneLeft ", Vec3f( 5.f, 0.f, 0.f ), Vec3f( -1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( " RedMatte ", " PlaneLeft " );
		_addObject( new Plane( " PlaneCeiling ", Vec3f( 0.f, 7.f, 0.f ), Vec3f( 0.f, -1.f, 0.f ) ) );
		_attachMaterialToObject( " GreenMatte ", " PlaneCeiling " );
		_addObject( new Plane( " PlaneRight ", Vec3f( -5.f, 0.f, 0.f ), Vec3f( 1.f, 0.f, 0.f ) ) );
		_attachMaterialToObject( " BlueMatte ", " PlaneRight " );
		_addObject( new Plane( " PlaneFront ", Vec3f( 0.f, 0.f, 10.f ), Vec3f( 0.f, 0.f, -1.f ) ) );
		_attachMaterialToObject( " MagentaMatte ", " PlaneFront " );
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// = = = = = = = = = = = =
		// Add lights .
		// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =
		// = = = = = = = = = = = = =
		_addLight( new PointLight( Vec3f( 0.f, 5.f, 0.f ), WHITE, 100.f ) );
		// _addLight ( new QuadLight ( Vec3f ( 1.f , 5.f , -2.f ) ,
		// Vec3f ( -2.f , 0.f , 0.f ) ,
		// Vec3f ( 0.f , 1.f , 2.f ) , WHITE , 40.f ) );
	}

	void Scene::initTP7()
	{

		_addObject( new ImplicitSphere( "Sphere1", Vec3f( -1.5f, 0.5f, 5.f ), 1.f ) );
		_addObject( new ImplicitSphere( "Sphere2", Vec3f( -1.5f, 0.5f, 7.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -2.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );

		_addObject( new ImplicitBox( "Box", Vec3f( 1.f, 0.5f, 5.f ), 1.f ) );
		_addObject( new ImplicitDonut( "Donut", Vec3f( -1.f, -1.f, 2.f ), Vec2f( 0.5f, 0.2f ) ) );
		_addObject( new ImplicitInfiniteCylinder( "Cylinder", Vec3f( 1.5f, -1.f, 2.f ), Vec3f( 0.5f, 0.2f,0.1f ) ) );
		_addObject( new ImplicitDeathStar( "Death", Vec3f( 1.f, -0.5f, 2.f ),0.7f,0.5f,0.5f ));

		// Add materials.
		_addMaterial( new ColorMaterial( "Blue", BLUE ) );
		_addMaterial( new ColorMaterial( "Red", RED ) );
		_addMaterial( new ColorMaterial( "Green", GREEN ) );
		_addMaterial( new ColorMaterial( "Yellow", YELLOW ) );
		_addMaterial( new ColorMaterial( "Cyan", CYAN ) );

		// Link objects and materials.
		_attachMaterialToObject( "Blue", "Box" );
		_attachMaterialToObject( "Red", "Sphere1" );
		_attachMaterialToObject( "Blue", "Sphere2" );
		
		_attachMaterialToObject( "Green", "Donut" );
		_attachMaterialToObject( "Yellow", "Cylinder" );
		_attachMaterialToObject( "Cyan", "Death" );
		// Add Lights
		
		_addLight( new PointLight( Vec3f( 0.f, 5.f, -2.f ), WHITE, 60.f ) );
		
	
	}
	void Scene::initProjet() {

		ImageTexture * earthTexture = new ImageTexture();
		earthTexture->load( DATA_PATH + "textures/mars.jpg" );

		// CHECKER TEXTURE 
		SolidColorTexture * redTexture = new SolidColorTexture( BLACK );
		SolidColorTexture * whiteTexture = new SolidColorTexture( WHITE );
		CheckerTexture *	checkerTexture = new CheckerTexture( whiteTexture, redTexture );
		
		// NOISE TEXTURE 
		NoiseTexture * noiseTexture = new NoiseTexture(0.5f);
		
		
		// Add objects.
		_addObject( new Sphere( "Sphere1", Vec3f( 0.f, 0.f, 3.f ), 1.f ) );
		_addObject( new Plane( "Plane1", Vec3f( 0.f, -1.f, 0.f ), Vec3f( 0.f, 1.f, 0.f ) ) );
		_addObject( new ImplicitBox( "Box", Vec3f( -3.f, 0.f, 4.f ), 1.f , Vec3f(0.f,40.f,0.f) ));
		
		// Add materials.
		_addMaterial( new TextureMaterial( "mars", earthTexture ) );
		_addMaterial( new TextureMaterial( "checker", checkerTexture ) );
		_addMaterial( new TextureMaterial( "noise", noiseTexture ) );
		
		// Link objects and materials.

		_attachMaterialToObject( "noise", "Box" );
		_attachMaterialToObject( "mars", "Sphere1" );
		_attachMaterialToObject( "checker", "Plane1" );

		
		// Add Lights
		_addLight( new QuadLight( Vec3f( 10.f, 10.f, -10.f ), Vec3f( -5.f, 0.f, 0.f ), Vec3f( 0.f, 0.f, 5.f ), WHITE, 30.f ) );
	
	}

	void Scene::init() {}

	void Scene::loadFileTriangleMesh( const std::string & p_name, const std::string & p_path )
	{
		std::cout << "Loading: " << p_path << std::endl;
		Assimp::Importer importer;

		// Read scene and triangulate meshes
		const aiScene * const scene
			= importer.ReadFile( p_path, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords );

		if ( scene == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path ); }

		unsigned int cptTriangles = 0;
		unsigned int cptVertices  = 0;

		for ( unsigned int m = 0; m < scene->mNumMeshes; ++m )
		{
			const aiMesh * const mesh = scene->mMeshes[ m ];
			if ( mesh == nullptr ) { throw std::runtime_error( "Fail to load file: " + p_path + ": mesh is null" ); }

			const std::string meshName = p_name + "_" + std::string( mesh->mName.C_Str() );
			std::cout << "-- Load mesh " << m + 1 << "/" << scene->mNumMeshes << ": " << meshName << std::endl;

			cptTriangles += mesh->mNumFaces;
			cptVertices += mesh->mNumVertices;

			const bool hasUV = mesh->HasTextureCoords( 0 );

			MeshTriangle * triMesh = new MeshTriangle( meshName );
			// Vertices before faces otherwise face normals cannot be computed.
			for ( unsigned int v = 0; v < mesh->mNumVertices; ++v )
			{
				triMesh->addVertex( mesh->mVertices[ v ].x, mesh->mVertices[ v ].y, mesh->mVertices[ v ].z );
				triMesh->addNormal( mesh->mNormals[ v ].x, mesh->mNormals[ v ].y, mesh->mNormals[ v ].z );
				if ( hasUV ) triMesh->addUV( mesh->mTextureCoords[ 0 ][ v ].x, mesh->mTextureCoords[ 0 ][ v ].y );
			}
			for ( unsigned int f = 0; f < mesh->mNumFaces; ++f )
			{
				const aiFace & face = mesh->mFaces[ f ];
				triMesh->addTriangle( face.mIndices[ 0 ], face.mIndices[ 1 ], face.mIndices[ 2 ] );
			}

			_addObject( triMesh );

			const aiMaterial * const mtl = scene->mMaterials[ mesh->mMaterialIndex ];
			if ( mtl == nullptr )
			{
				std::cerr << "Material undefined," << meshName << " assigned to default material" << std::endl;
			}
			else
			{
				Vec3f kd = WHITE;
				Vec3f ks = BLACK;
				float s	 = 0.f;

				aiColor3D aiKd;
				if ( mtl->Get( AI_MATKEY_COLOR_DIFFUSE, aiKd ) == AI_SUCCESS ) kd = Vec3f( aiKd.r, aiKd.g, aiKd.b );
				aiColor3D aiKs;
				if ( mtl->Get( AI_MATKEY_COLOR_SPECULAR, aiKs ) == AI_SUCCESS ) ks = Vec3f( aiKs.r, aiKs.g, aiKs.b );
				float aiS = 0.f;
				if ( mtl->Get( AI_MATKEY_SHININESS, aiS ) == AI_SUCCESS ) s = aiS;
				aiString mtlName;
				mtl->Get( AI_MATKEY_NAME, mtlName );

				_addMaterial( new ColorMaterial( std::string( mtlName.C_Str() ), kd ) );
				_attachMaterialToObject( mtlName.C_Str(), meshName );
			}
			// BVH CONSTRUCTION
			triMesh->buildMeshBVH();

			std::cout << "-- [DONE] " << triMesh->getNbTriangles() << " triangles, " << triMesh->getNbVertices()
					  << " vertices." << std::endl;
		}
		std::cout << "[DONE] " << scene->mNumMeshes << " meshes, " << cptTriangles << " triangles, " << cptVertices
				  << " vertices." << std::endl;
	}

	bool Scene::intersect( const Ray & p_ray, const float p_tMin, const float p_tMax, HitRecord & p_hitRecord ) const
	{
		float tMax = p_tMax;
		bool  hit  = false;
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersect( p_ray, p_tMin, tMax, p_hitRecord ) )
			{
				tMax = p_hitRecord._distance; // update tMax to conserve the nearest hit
				hit	 = true;
			}
		}
		return hit;
	}

	bool Scene::intersectAny( const Ray & p_ray, const float p_tMin, const float p_tMax ) const
	{
		for ( const ObjectMapPair & object : _objectMap )
		{
			if ( object.second->intersectAny( p_ray, p_tMin, p_tMax ) ) { return true; }
		}
		return false;
	}

	void Scene::_addObject( BaseObject * p_object )
	{
		const std::string & name = p_object->getName();
		if ( _objectMap.find( name ) != _objectMap.end() )
		{
			std::cout << "[Scene::addObject] Object \'" << name << "\' already exists" << std::endl;
			delete p_object;
		}
		else
		{
			_objectMap[ name ] = p_object;
			_objectMap[ name ]->setMaterial( _materialMap[ "default" ] );
		}
	}

	void Scene::_addMaterial( BaseMaterial * p_material )
	{
		const std::string & name = p_material->getName();
		if ( _materialMap.find( name ) != _materialMap.end() )
		{
			std::cout << "[Scene::_addMaterial] Material \'" << name << "\' already exists" << std::endl;
			delete p_material;
		}
		else
		{
			std::cout << "Material \'" << name << "\' added." << std::endl;
			_materialMap[ name ] = p_material;
		}
	}

	void Scene::_addLight( BaseLight * p_light ) { _lightList.emplace_back( p_light ); }

	void Scene::_attachMaterialToObject( const std::string & p_materialName, const std::string & p_objectName )
	{
		if ( _objectMap.find( p_objectName ) == _objectMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Object \'" << p_objectName << "\' does not exist"
					  << std::endl;
		}
		else if ( _materialMap.find( p_materialName ) == _materialMap.end() )
		{
			std::cout << "[Scene::attachMaterialToObject] Material \'" << p_materialName << "\' does not exist, "
					  << "object \'" << p_objectName << "\' keeps its material \'"
					  << _objectMap[ p_objectName ]->getMaterial()->getName() << "\'" << std::endl;
		}
		else { _objectMap[ p_objectName ]->setMaterial( _materialMap[ p_materialName ] ); }
	}

} // namespace RT_ISICG
