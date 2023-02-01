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
		float height = glm::tan( glm::radians(_fovy/2) ) * _focalDistance * 2;
		float width	 = height * _aspectRatio;
	
		Vec3f topLeftZ = - ( _w * _focalDistance);
		Vec3f topLeftX = - ( _u * (float)( width / 2 ) );
		Vec3f topLeftY = ( _v * (float)( height / 2 ) );

		_viewportTopLeftCorner = _position + topLeftZ + topLeftX + topLeftY;
		

		_viewportU = _u * width;
		_viewportV = _v * height;


	}

} // namespace RT_ISICG
