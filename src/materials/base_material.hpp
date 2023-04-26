#ifndef __RT_ISICG_BASE_MATERIAL__
#define __RT_ISICG_BASE_MATERIAL__

#include "hit_record.hpp"
#include "lights/light_sample.hpp"
#include "ray.hpp"
#include "texture.hpp"
#include "textures/solid_color.hpp"

namespace RT_ISICG
{
	class BaseMaterial
	{
	  public:
		BaseMaterial() = delete;
		BaseMaterial( const std::string & p_name ) : _name( p_name ) { _texture = new SolidColorTexture(); }
		BaseMaterial( const std::string & p_name, const Vec3f p_color ) : _name( p_name )
		{
			_texture = new SolidColorTexture( p_color );
		}
		BaseMaterial( const std::string & p_name, Texture * p_texture ) : _name( p_name ), _texture( p_texture ) {}
		
		virtual ~BaseMaterial() = default;

		virtual Vec3f shade( const Ray &		 p_ray,
							 const HitRecord &	 p_hitRecord,
							 const LightSample & p_lightSample ) const = 0;

		virtual inline const Vec3f & getFlatColor() const = 0;

		virtual const std::string & getName() const final { return _name; }

		virtual const bool isMirror() const { return false; }

		virtual const bool isTransparent() const { return false; }

		virtual const float getIOR() const { return 1; }

	  protected:
		std::string _name;
		Texture *	_texture = nullptr;
	};

} // namespace RT_ISICG

#endif // __RT_ISICG_BASE_MATERIAL__
