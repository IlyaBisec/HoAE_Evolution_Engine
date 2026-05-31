#include "stdfx.h"

class BasicSceleElement : public SNode{
public:
    AABoundBox  m_AABB;
    bool        m_VisibleOnThisFrame;    
};
class StaticSceneGeometry : public BasicSceneElement{
public:    
    DWORD  m_VB_offset;
    DWORD  m_IB_offset;
    DWORD  m_IB_start_index;
    DWORD  m_VB_size;
    DWORD  m_IB_size;
    DWORD  m_FVF;
    DWORD  m_Shader;
    DWORD  m_Texture[8];
};
class StaticGeometryManager{
public:
    DWORD m_VBID;
    DWORD m_IBID;    
};