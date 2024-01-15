/*****************************************************************/
/*  File:   rsRenderPool.cpp
/*  Author: Silver, Copyright (C) GSC Game World
/*  Date:   Feb 2002
/*****************************************************************/
#include "stdafx.h"
#include "rsRenderPool.h"

/*****************************************************************/
/*    RenderAttributes implementation
/*****************************************************************/
RenderAttributes::AttributeStatus    RenderAttributes::s_AttrOrder[atNumAttributes];

const char*    RenderAttributes::GetAttributeName( int idx )
{
    if (idx == atTex0              ) return "Texture 0";
    if (idx == atTex1              ) return "Texture 1";
    if (idx == atTex2              ) return "Texture 2";
    if (idx == atTex3              ) return "Texture 3";
    if (idx == atDSS               ) return "Device StateBlock";
    if (idx == atMaterial          ) return "Material";
    if (idx == atVS                ) return "Vertex Shader";
    if (idx == atVSConstant        ) return "Vertex Shader Constants";
    if (idx == atPS                ) return "Pixel Shader";
    if (idx == atPSConstant        ) return "Pixel Shader Constants";
    if (idx == atTransparency      ) return "Transparency";
    if (idx == atZ                 ) return "Depth Value";
    if (idx == atWorldTransform    ) return "World Transform";
    if (idx == atVertexBuffer      ) return "Vertex Buffer";
    if (idx == atIndexBuffer       ) return "Index Buffer";
    if (idx == atVBPos             ) return "VB Position";
    if (idx == atNumVert           ) return "Num Vertices";
    if (idx == atIBPos             ) return "IB Position";
    if (idx == atNumInd            ) return "Num Indices";
    if (idx == atMesh              ) return "Mesh Handle";    
    return "Unknown";
} // RenderAttributes::GetAttributeName

void RenderAttributes::DumpAttrOrder()
{
    printf( "AttrOrder: \n" );
    for (int i = 0; i < atNumAttributes; i++ )
    {
        const AttributeStatus& as = s_AttrOrder[i];
        printf( "  %s(%d)", GetAttributeName( as.m_Priority ), as.m_Priority );
        if (as.m_bEnabled) printf( "+\n" ); else printf( "-\n" );
    }
    printf( "\n" );
} // RenderAttributes::DumpAttrOrder

int    RenderAttributes::Compare( const void* p1, const void* p2 )
{
    const RenderAttributes* pEl1 = *(RenderAttributes**)p1;
    const RenderAttributes* pEl2 = *(RenderAttributes**)p2;

    for (int i = 0; i < atNumAttributes; i++)
    {
        if (pEl1->m_Attr[s_AttrOrder[i].m_Priority] < 
            pEl2->m_Attr[s_AttrOrder[i].m_Priority]) return -1;
        else if (    pEl1->m_Attr[s_AttrOrder[i].m_Priority] > 
                    pEl2->m_Attr[s_AttrOrder[i].m_Priority]) return 1; 
    }
    return 0;
} // RenderAttributes::Compare

void RenderAttributes::EnableAttribute( int attrID, bool enable )
{
    for (int i = 0; i < atNumAttributes; i++ )
    {
        AttributeStatus& as = s_AttrOrder[i];
        if (as.m_Priority == attrID) { as.m_bEnabled = enable; return; }
    }
} // RenderAttributes::EnableAttribute

void RenderAttributes::MoveAttributeUp( int curPos )
{
    if (curPos < 0 || curPos >= atNumAttributes) return;

    int destPos = curPos + 1; 
    if (destPos >= atNumAttributes) destPos = atNumAttributes - 1;

    AttributeStatus tmp     = s_AttrOrder[destPos];
    s_AttrOrder[destPos]    = s_AttrOrder[curPos];
    s_AttrOrder[curPos]     = tmp;
} // RenderAttributes::MoveAttributeUp

void RenderAttributes::MoveAttributeDown( int curPos )
{
    if (curPos < 0 || curPos >= atNumAttributes) return;

    int destPos = curPos - 1; 
    if (destPos < 0) destPos = 0;

    AttributeStatus tmp     = s_AttrOrder[destPos];
    s_AttrOrder[destPos]    = s_AttrOrder[curPos];
    s_AttrOrder[curPos]     = tmp;
} // RenderAttributes::MoveAttributeDown

void RenderAttributes::Random( DWORD maxVal )
{
    for (int i = 0; i < atNumAttributes; i++)
    {
        m_Attr[i] = rand() % maxVal;
    }
} // RenderAttributes::Random

void RenderAttributes::Dump()
{
    for (int i = 0; i < atNumAttributes; i++)
    {
        printf( "%d ", m_Attr[i] );
    }
    printf( "\n" );
} // RenderAttributes::Dump

/*****************************************************************/
/*    RenderPool implementation
/*****************************************************************/
RenderPool::RenderPool()
{
    for (int i = 0; i < RenderAttributes::atNumAttributes; i++)
    {
         RenderAttributes::s_AttrOrder[i].m_Priority  = i;
         RenderAttributes::s_AttrOrder[i].m_bEnabled = true;
    }
} // RenderPool::RenderPool

void RenderPool::Sort()
{
    if (m_RenderElem.size() != m_ElemOrder.size()) 
    {
        m_ElemOrder.clear();
        int nElem = GetNumElements();
        for (int i = 0; i < nElem; i++)
        {
            m_ElemOrder.push_back( &m_RenderElem[i] );
        }
    }
    qsort( &m_ElemOrder[0], GetNumElements(), sizeof( RenderAttributes* ), RenderAttributes::Compare );
} // RenderPool::Sort

void RenderPool::Dump()
{
    RenderAttributes::DumpAttrOrder();
    
    for (int i = 0; i < m_ElemOrder.size(); i++)
    {
        m_ElemOrder[i]->Dump();
    }
} // RenderPool::Dump









