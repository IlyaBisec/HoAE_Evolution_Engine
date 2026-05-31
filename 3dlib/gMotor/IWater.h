/*****************************************************************************/
/*    File:    IWater.h
/*    Desc:    Interface for working with water
/*    Author:    Ruslan Shestopalyuk
/*    Date:    2 Sep 2004
/*****************************************************************************/
#ifndef __IWATER_H__
#define __IWATER_H__

typedef DWORD (*WaterColorCB)( float x, float y );

/*****************************************************************************/
/*  Class:  IWaterscape
/*  Desc:   Interface for manipulating water mass
/*****************************************************************************/
class IWaterscape
{
public:
    virtual void                Render          () = 0;
	virtual void				Reset			() = 0;
    virtual void                Refresh         ( const Rct& area ) = 0;
    virtual void                SetWaterColorCB ( WaterColorCB cb ) = 0;
	virtual void				OrthoModeEnable	() = 0;
	virtual void				OrthoModeDisable() = 0;
    virtual void                SetShaderQuality( int Level ) = 0;// 0 - low quality 1 - high 2 - ultra high ...
	virtual void				SetWaterline(const float Waterline) = 0;
	virtual float				GetWaterline() const = 0;
	virtual const Rct			CalcVisibleWaterRect() const = 0;
	virtual void				SetBoilingCoef(const float BoilingCoef) = 0;
}; // class IWaterscape

extern IWaterscape*     IWater;
#endif // __IWATER_H__