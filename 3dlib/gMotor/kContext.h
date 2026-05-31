/*****************************************************************
/*  File:   kContext.h                                          
/*  Desc:   Hierarchical game entity context                                  
/*  Author: Silver, Copyright (C) GSC Game World                 
/*  Date:   August 2004                                        
/*****************************************************************/
#ifndef __KCONTEXT_H__
#define __KCONTEXT_H__

const int    c_MaxContextDepth = 8;
/*****************************************************************************/
/*    Struct:    EntityContext
/*  Desc:   Describes unique stack-based context for the entity instance
/*****************************************************************************/
struct EntityContext
{
    DWORD        m_Context[c_MaxContextDepth];    //  context in which we were instanced
    int            m_Depth;                        //    depth of the context

    DWORD hash() const 
    {
        DWORD h = 0;
        for (int i = 0; i < m_Depth; i++)
        {
            h = (h * 729 + m_Context[i] * 37) ^ (h >> 1);
        }
        return h;
    }

    void operator =( const EntityContext& ctx )
    {
        m_Depth = ctx.m_Depth;
        for (int i = 0; i < m_Depth; i++) m_Context[i] = ctx.m_Context[i];
    }

    bool operator ==( const EntityContext& ctx ) const
    {
        if (m_Depth != ctx.m_Depth) return false;
        for (int i = 0; i < m_Depth; i++) 
        {
            if (m_Context[i] != ctx.m_Context[i]) return false;
        }
        return true;
    }

    void Push( DWORD val )
    {
        assert( m_Depth < c_MaxContextDepth );
        m_Context[m_Depth++] = val;
    }

    DWORD Pop()
    {
        if (m_Depth == 0) return 0xFFFFFFFF;
        return m_Context[--m_Depth];
    }

    void Reset() { m_Depth = 0; }

}; // struct EntityContext

//  global context methods
void                    PushEntityContext   ( DWORD ctx );
DWORD                    PopEntityContext    ();
void                    ResetEntityContext    ();
const EntityContext&    GetEntityContext    ();
void                    SetEntityContext    ( const EntityContext& context );

#endif // __KCONTEXT_H__