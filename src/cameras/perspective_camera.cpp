#include "perspective_camera.hpp"
#include <glm/gtx/string_cast.hpp>

namespace RT_ISICG
{
	PerspectiveCamera::PerspectiveCamera( const float p_aspectRatio ) : _aspectRatio( p_aspectRatio )
	{
		_updateViewport();
	}

	PerspectiveCamera::PerspectiveCamera( const Vec3f & p_position,
										  const Vec3f & p_lookAt,
										  const Vec3f & p_up,
										  const float	p_fovy,
										  const float	p_aspectRatio )
		: BaseCamera( p_position ), _fovy( p_fovy ), _aspectRatio( p_aspectRatio )
	{
		/// TODO ! _u ? _v ? _w ?

		_updateViewport();
	}

	void PerspectiveCamera::_updateViewport()
	{
		float height = glm::tan( glm::radians(_fovy) ) * _focalDistance;
		float width	 = height * _aspectRatio;


		_viewportTopLeftCorner
			= _position - ( _w * _focalDistance ) + ( _v * (float)( height/2 ) ) - ( _u * (float)( width/2 ) );
		

		float _newV = _position.x - ( width/2 );
		float _newU = _position.y + ( height/2 );
		float _newW = _position.z + _focalDistance;

		_viewportTopLeftCorner = Vec3f( _newU, _newV, _newW );

		_viewportU = _u * width;
		_viewportV = _v * height;


	}

} // namespace RT_ISICG
