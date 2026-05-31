/*****************************************************************************/
/*	File:	NewMon3D.h
/*	Desc:	3D units
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10.12.2002
/*****************************************************************************/
#ifndef __NEWMON3D_H__
#define __NEWMON3D_H__

#ifdef _USE3D

const int c_MaxMdlFileNameLen = 64;
class AnimModel;
class NewMonster;
class OneObject;

/*****************************************************************************/
/*	Class:	NewMon3D
/*	Desc:	3D unit class
/*****************************************************************************/
class NewMon3D
{
	char		mdlFile[c_MaxMdlFileNameLen];
	AnimModel*	mdl;
	NewMonster* papaMonster;
	bool		isLoaded;

public:
				NewMon3D( NewMonster* papa = 0 );
				~NewMon3D();
	void		SetModelFile( const char* fname );
	void		Draw( OneObject* OB );
	void		ProgressAnim( int frame );

}; // class NewMon3D

/*****************************************************************************/
/*	Class:	OneObjectEx
/*	Desc:	Extended game object description
/*****************************************************************************/
class OneObjectEx
{
	Matrix4D		transform;		//  world space transform for object's model
	OneObject*		coreObj;		//  pointer to main game OneObject structure

public:
	OneObjectEx( OneObject* origObj ) : coreObj(origObj), transform(Matrix4D::identity) {}
	void		CalculateWorldTransform();
	void		GetUpWorldDirection( int x, int y, Vector4D& up );
	Matrix4D&	getTransform() { return transform; }
	void		SetInsideCamera( Camera& cam );
	
}; // class OneObjectEx


#endif _USE3D

#endif // __NEWMON3D_H__