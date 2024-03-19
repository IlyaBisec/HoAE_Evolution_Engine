/*****************************************************************************/
/*	File:	vSkeletonAnimation.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	10-04-2004
/*****************************************************************************/
#ifndef __VSKELETONANIMATION_H__
#define __VSKELETONANIMATION_H__



#include "vSkeletonController.h"
#include "mFloatCurve.h"
#include "mQuatCurve.h"

class ModelObject;
/*****************************************************************************/
/*  Class:  PRSTrack
/*  Desc:   Single bone animation track
/*****************************************************************************/
class PRSTrack : public IReflected
{
    int                 m_BoneID;               //  id of the bone track is attached to
    std::string         m_BoneName;             //  name of the bone 
    bool                m_bPose;                //  whether track is a pose
    Matrix4D            m_Pose;                 //  pose matrix, when constant

    FloatCurve		    m_PosX, m_PosY, m_PosZ; //  position animation tracks
    QuatCurve		    m_Rot;                  //  rotation animation track
    FloatCurve		    m_ScX, m_ScY, m_ScZ;    //  scale animation track

public:
                        PRSTrack    ();
    void                SetIsPose   ( bool bPose = true )    { m_bPose    = bPose;  } 
    void                SetPose     ( const Matrix4D& pose );
    void                SetName     ( const char* name )     { m_BoneName = name;   }
    virtual void        Expose      ( PropertyMap& pm );
    virtual const char* ClassName   () const { return "PRSTrack"; }
    virtual const char* GetName     () const { return m_BoneName.c_str(); }
    int                 GetBoneID   () const { return m_BoneID; }
    void                SetBoneID   ( int id ) { m_BoneID = id; }

    void                AddPosXKey  ( float t, float v ) { m_PosX.AddKey( t, v ); }
    void                AddPosYKey  ( float t, float v ) { m_PosY.AddKey( t, v ); }
    void                AddPosZKey  ( float t, float v ) { m_PosZ.AddKey( t, v ); }
    void                AddRotKey   ( float t, const Quaternion& v ) { m_Rot.AddKey( t, v ); } 
    void                AddSXKey    ( float t, float v ) { m_ScX.AddKey( t, v ); }  
    void                AddSYKey    ( float t, float v ) { m_ScY.AddKey( t, v ); }  
    void                AddSZKey    ( float t, float v ) { m_ScZ.AddKey( t, v ); }  

    Matrix4D            GetTransform( float t ) const
    {
        if (m_bPose) return m_Pose;
        Vector3D	sc( m_ScX.GetValue( t ), m_ScY.GetValue( t ), m_ScZ.GetValue( t ) );
        Vector3D	tr( m_PosX.GetValue( t ), m_PosY.GetValue( t ), m_PosZ.GetValue( t ) );
        Quaternion	quat = m_Rot.GetValue( t );
        return Matrix4D( sc, quat, tr );
    }

    void                GetTransform( float t, Vector3D& sc, Quaternion& rot, Vector3D& tr ) const
    {
        sc.x = m_ScX.GetValue( t );
        sc.y = m_ScY.GetValue( t );
        sc.z = m_ScZ.GetValue( t );
        tr.x = m_PosX.GetValue( t );
        tr.y = m_PosY.GetValue( t );
        tr.z = m_PosZ.GetValue( t );
        rot = m_Rot.GetValue( t );
    }
}; // struct PRSTrack

const DWORD c_AnimVersion = 0x00000001;
/*****************************************************************************/
/*  Class:  SkeletonAnimation
/*  Desc:   Pre-made skeleton animation
/*****************************************************************************/
class SkeletonAnimation : public IReflected
{
    std::vector<PRSTrack>   m_Tracks;   //  animation track list
    std::string             m_Name;     //  animation name
    int                     m_ID;       //  animation ID at the model manager
    float                   m_PlayTime; //  
    bool                    m_bBound;   //  whether animation is bound to the bones by ID

public:
                        SkeletonAnimation();
    virtual int         GetNTracks      () const { return m_Tracks.size(); }
    virtual void        Serialize       ( OutStream& os ){}
    virtual bool        Unserialize     ( InStream& is ){ return false; }
    virtual DWORD       GetVersion      () { return c_AnimVersion; }

    virtual void        Expose          ( PropertyMap& pm );
    virtual const char* ClassName       () const { return "SkeletonAnimation"; }
    virtual const char* GetName         () const { return m_Name.c_str(); }
    void                SetName         ( const char* name ) { m_Name = name; }
    void                AddTrack        ( const PRSTrack& track ) { m_Tracks.push_back( track ); }
    void                SetID           ( int id ) { m_ID = id; }
    PRSTrack&           GetTrack        ( int idx ) { return m_Tracks[idx]; }     

    float               GetPlayTime     () const { return m_PlayTime; }
    void                SetPlayTime     ( float val ) { m_PlayTime = val; }
    bool                IsBound         () const { return m_bBound; }
    void                Bind            ( ModelObject* pModel );

}; // class SkeletonAnimation

#endif // __VSKELETONANIMATION_H__