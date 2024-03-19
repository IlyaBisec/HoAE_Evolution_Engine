#ifndef __CameraInterpolator__
#define __CameraInterpolator__

#include "..\GameExtension.h"
#include "Surface.h"

namespace Surface {
	
class CameraInterpolator : public GameExtension {
public:
	CameraInterpolator();
	bool IsActive() const;

	void Clear();
	void SaveCurCamera();
	void Play();
	void Stop();

	void SetupCamera();
//private:
	bool m_fActive;
	cList<Matrix4D> m_TMs;
	float m_CurPos;

	void SetupCamera(float s);
};

} // Surface

extern Surface::CameraInterpolator g_CameraInterpolator;

#endif // __CameraInterpolator__