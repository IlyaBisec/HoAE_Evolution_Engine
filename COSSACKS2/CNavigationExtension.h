#ifndef __CNAVIGATIONEXTENSION__
#define __CNAVIGATIONEXTENSION__

#include "stdheader.h"
#include "GameExtension.h"

extern bool MMPressed;
extern float fMapX;
extern float fMapY;
extern bool CheckIfNewTerrain(void);
float fRotateCameraByAngle = 0;
bool bLandscapeNavigation = false;
extern bool g_NewTerrainExtensionIsActive;

class CNavigationExtension : public GameExtension
{
public:
	CNavigationExtension() {}
//	~CNavigationExtension() {}
	virtual bool OnMouseHandling(int mx, int my, bool &LeftPressed,
		bool &RightPressed, int MapCoordX, int MapCoordY, bool OverMiniMap)
	{
		// Dragging or rotating landscape
		static int LastMx = 0, LastMy = 0, Sdx = 0, _mouseX = 0, _mouseY = 0;
		bool bRotation = ((RightPressed == true) && (GetKeyState(VK_CONTROL) & 0x8000));
		if(CheckIfNewTerrain() == false) bRotation = false;
		if((MMPressed == true) || bRotation)
		{
			if(bLandscapeNavigation == true)
			{
				float dx = LastMx - mx, dy = LastMy - my;
				if(bRotation)
				{
					// 1.75
					const float RSpeed = 1.0f, RSingular = 300., // r / RSpeed == 1
						RHole = 200.;
					float xA = _mouseX - RealLx / 2, yA = _mouseY - RealLy / 2,
						xB = mouseX - RealLx / 2, yB = mouseY - RealLy / 2;
					Vector3D v3dA = Vector3D::null, v3dB = Vector3D::null,
						v3dR = Vector3D::null;
					// Considering shift along y axis
					float cx = (xA + xB) / 2;
					v3dA.x = cx, v3dA.y = yA, v3dA.z = 0;
					v3dB.x = cx, v3dB.y = yB, v3dB.z = 0;
					if(v3dA.normalize() && v3dB.normalize())
					{
						v3dR.cross(v3dA, v3dB);
						float cy = (yA + yB) / 2;
						float r = sqrt(cx * cx + cy * cy);
						if(r > RHole) fRotateCameraByAngle -= v3dR.z / RSpeed;
						else
						{
							float k = r / RHole;
							k *= k;
							fRotateCameraByAngle -= k * v3dR.z / RSpeed; 
						}
					}
					// Considering shift along x axis
					v3dA = Vector3D(xA, RSingular, 0);
					v3dB = Vector3D(xB, RSingular, 0);
					v3dA.normalize(); // length always > c_Epsilon
					v3dB.normalize();
					v3dR.cross(v3dA, v3dB);
					fRotateCameraByAngle -= v3dR.z / RSpeed;
				}
				else // Navigation
				{
					// considering angle around Z
					Vector3D v3dDir;
					v3dDir = ICam->GetDir();
					Vector3D v3dXY;
					v3dXY.x = v3dDir.x,
						v3dXY.y = v3dDir.y,
						v3dXY.z = 0;
					const float fSpeed = 0.0625f;
					dx *= (fSpeed / 2.0f), dy *= fSpeed;
					float DX = 0, DY = 0;
					float l = 0;
					l = v3dXY.norm();
					if(l > 0)
					{
						v3dXY.x /= l,
							v3dXY.y /= l;
						DX = - float(dx) * v3dXY.y - float(dy) * v3dXY.x;
						DY = float(dx) * v3dXY.x - float(dy) * v3dXY.y;
					}
					else
						DX = dx,
						DY = dy;
					float adx = 0, ady = 0;
					adx = fabs(DX), ady = fabs(DY);
					if(DX >= 0) fMapX += adx;
					else fMapX -= adx;
					if(DY >= 0) fMapY += ady;
					else fMapY -= ady;
					LastMx = mx, LastMy = my;
					//SetCurPtr(nCursor);
				}
				LastMx = mx, LastMy = my;
				_mouseX = mouseX, _mouseY = mouseY;
			}
			else
			{
				bLandscapeNavigation = true;
				LastMx = mx, LastMy = my;
				_mouseX = mouseX, _mouseY = mouseY;
			}
			return true;
		}
		bLandscapeNavigation = false;
		return false;
	}
private:
};

CNavigationExtension g_NavigationExtension;

#endif