/*****************************************************************************/
/*    File:    kXMLParser.h
/*    Desc:    Simplified parser of the XML files
/*    Author:    Ruslan Shestopalyuk
/*    Date:    03.04.2003
/*****************************************************************************/
#ifndef __KXMLPARSER_H__
#define __KXMLPARSER_H__

#include <stack>
#include <string>

/*****************************************************************************/
/*    Class:    XMLParser
/*    Desc:    .xml file parsing handler
/*****************************************************************************/
class XMLParser
{
public:
                    XMLParser        ();
    virtual            ~XMLParser        (){}


    bool            ParseFile        ( const char* fName            );
    bool            ParseBuffer        ( char* buffer                );
    int                yyInput            ( char* buf, int max_size    );
    void            Error            ( char* msg, int curLine    );
    
    void            SetErrorTokenPtr( char* pErrToken )    { m_pErrToken = pErrToken; }
    void            SetCurLine        ( int line          )    { m_CurLine = line; } 
    const char*        GetCurLocation    () const;
    virtual void    OnOpen            ( const char* tag );
    virtual void    OnClose            ( const char* tag );
    virtual void    OnValue            ( const char* value );
    virtual void    OnAttribute        ( const char* name, const char* value );
    void            Init()            { if (!m_bInited) OnInit(); m_bInited = true; }

protected:
    virtual void    OnInit()        {}

    bool            m_bInited;

private:
    char*            m_Buffer;
    char*            m_BufPtr;
    char*            m_pErrToken;
    int                m_CurLine;
    char            m_FileName[_MAX_PATH];

}; // class XMLParser

/*****************************************************************************/
/*    Class:    XMLNode
/*    Desc:    .xml tag node
/*****************************************************************************/
class XMLNode
{
    XMLNode*                m_pChild;        //  pointer to the first subnode
    XMLNode*                m_pAttr;        //  pointer to the first attribute
    XMLNode*                m_pParent;        //  pointer to the parent tag
    std::string                m_Tag;            //    tag
    std::string                m_Value;        //  value string
    XMLNode*                m_pNext;        //  pointer to the next sibling node

    static int                s_Indent;            //  indentation counter when writing 
    static bool                s_bCaseIndependent; //  whether tags are case independent

public:
                            XMLNode            ();
                            XMLNode            ( InStream& is );
                            XMLNode            ( char* buf );
                            ~XMLNode        ();


    int                        GetNChildren    () const;
    XMLNode*                GetChild        ( int idx );

    int                        GetNAttr        () const;
    
    //  (un)flattens xml-convention symbols (such as <,> etc) 
    void                    Flatten         ();
    void                    Unflatten       ();

    XMLNode*                FirstChild        () { return m_pChild; }
    XMLNode*                FirstAttr        () { return m_pAttr;  }
    
    const XMLNode*            FirstChild        () const { return m_pChild; }
    const XMLNode*            FirstAttr        () const { return m_pAttr;  }


    XMLNode*                FindChild        ( const char* tag ) const;
    XMLNode*                FindAttr        ( const char* name ) const;
    XMLNode*                NextSibling        () { return m_pNext; }
    const XMLNode*            NextSibling        () const { return m_pNext; }

    void                    SetTag            ( const char* tag    ) { m_Tag = tag; }
    bool                    HasTag            ( const char* tag );
    const char*                GetTag            () const { return m_Tag.c_str(); }
    const char*                GetValue        () const { return m_Value.c_str(); }

    XMLNode*                AddChild        ( const char* tag );
    XMLNode*                AddChild        ( XMLNode* pChild );

    template <class TVal> XMLNode* AddAttr    ( const char* tag, const TVal&value )
    {
        XMLNode* newAttr = new XMLNode();
        newAttr->SetTag( tag );
        newAttr->SetValue( value );
        newAttr->m_pParent = this;
        if (!m_pAttr) { m_pAttr = newAttr; return newAttr; }
        XMLNode* pHead = m_pAttr;
        while (pHead->m_pNext) pHead = pHead->m_pNext;
        pHead->m_pNext = newAttr;
        return newAttr;
    }

    template <class TVal> void SetValue( const TVal& value ) 
    { 
        m_Value = ToString( value );
    }

    template <class TVal> XMLNode* AddValue( const char* tag, const TVal& value ) 
    { 
        XMLNode* pNode = new XMLNode();
        pNode->SetTag( tag );
        pNode->SetValue( value );
        AddChild( pNode );
        return pNode;
    }

    bool                    Read            ( InStream& is );
    void                    Write            ( OutStream& os );
    void                    WriteAttr        ( OutStream& os );
    operator                bool            (){ return false; }

    template <class TVal> bool GetValue( const char* name, TVal& val )
    {
        XMLNode* pChild = FindChild( name );
        if (!pChild) return false;
        return TypeTraits<TVal>::FromString( val, pChild->GetValue() );
    }
    
    template <class TVal> bool GetValue( TVal& val )
    {
        return TypeTraits<TVal>::FromString( val, GetValue() );
    }

    template <class TVal> bool GetAttr( const char* name, TVal& val )
    {
        XMLNode* pAttr = FindAttr( name );
        if (!pAttr) return false;
        return TypeTraits<TVal>::FromString( val, pAttr->GetValue() );
    }
    
    typedef TreeIterator<XMLNode> Iterator;

protected:
    static char*    Indent( char* buf )
    {
        for (int i = 0; i < s_Indent; i++) *(buf++) = '\t';
        return buf;
    }
}; // class XMLNode

/*****************************************************************************/
/*    Class:    XMLTreeParser
/*    Desc:    Parses xml file to the tree of the XMLNode's
/*****************************************************************************/
class XMLTreeParser : public XMLParser
{
    XMLNode*                m_pRoot;
    XMLNode*                m_pCurNode;
    std::stack<XMLNode*>    m_NodeStack;

public:
                    XMLTreeParser    ( XMLNode* pRoot ) { m_pRoot = pRoot; m_pCurNode = pRoot; }
    virtual void    OnOpen            ( const char* tag );
    virtual void    OnClose            ( const char* tag );
    virtual void    OnValue            ( const char* value );
    virtual void    OnAttribute        ( const char* name, const char* value );

}; // class XMLTreeParser 

const char* GetPooledString( DWORD id );
DWORD       CreatePooledString( const char* pStr, int len );
void        ClearStringPool();

#endif // __KXMLPARSER_H__