/*****************************************************************************/
/*    File:    kXMLParser.cpp
/*    Author:    Ruslan Shestopalyuk
/*    Date:    04.04.2003
/*****************************************************************************/
#include "stdafx.h"
#include "kXMLParser.h"

/*****************************************************************************/
/*    XMLParser implementation
/*****************************************************************************/
XMLParser::XMLParser()
{
    m_Buffer        = NULL;
    m_pErrToken        = 0;
    m_FileName[0]    = 0;
    m_bInited        = false;
} // XMLParser::XMLParser

bool XMLParser::ParseFile( const char* fName )
{
    strcpy( m_FileName, fName );
    FILE* fp = fopen( m_FileName, "rb" );
    if (!fp) return false;
    fseek( fp, 0, SEEK_END );
    int fileSize = ftell( fp ); 
    fseek( fp, 0, SEEK_SET );
    char* buf = new char[fileSize + 1];
    fread( buf, fileSize, 1, fp );
    fclose( fp );
    buf[fileSize] = 0;
    
    ClearStringPool();

    ParseBuffer( buf );
    
    ClearStringPool();
    delete []buf;
    return true;
} // XMLParser::ParseFile

const char* XMLParser::GetCurLocation() const
{
    static const int c_MaxLocationStr = 256;
    static char buf[c_MaxLocationStr];
    buf[0] = 0;
    return buf;
}

int    XMLParser::yyInput( char* buf, int max_size )
{
    assert( m_Buffer );
    int nSym = 0;
    while (*m_BufPtr && *m_BufPtr != '\n')
    {
        buf[nSym++] = *(m_BufPtr++);
        assert( nSym < max_size );
    }
    if (*m_BufPtr == '\n')
    {
        buf[nSym++] = '\n';
        m_BufPtr++;
    } 

    return nSym;
} // XMLParser::yyInput

void XMLParser::Error( char* msg, int curLine )
{
    if (m_pErrToken)
    {
        Log.Error( "Error in file %s, line %d: %s. Current token: %s", 
                    m_FileName, curLine, msg, m_pErrToken );
    }
    else
    {
        Log.Error( "Error in file %s, line %d: %s.", 
                    m_FileName, curLine, msg );
    }
} // XMLParser::Error

void XMLParser::OnOpen( const char* tag )
{
}

void XMLParser::OnClose( const char* tag )
{
}

void XMLParser::OnValue( const char* value )
{
}

void XMLParser::OnAttribute( const char* name, const char* value )
{
} // XMLParser::OnAttribute

/*****************************************************************************/
/*    XMLNode implementation
/*****************************************************************************/
int        XMLNode::s_Indent            = 0;
bool    XMLNode::s_bCaseIndependent = true;

XMLNode::XMLNode( InStream& is )
{
    m_pChild        = NULL;    
    m_pAttr            = NULL;
    m_pParent        = NULL;    
    m_pNext            = NULL;    
    Read( is );
}

XMLNode::XMLNode()
{
    m_pChild        = NULL;    
    m_pAttr            = NULL;
    m_pParent        = NULL;    
    m_pNext            = NULL;    
}

XMLNode::XMLNode( char* buf )
{
    m_pChild        = NULL;    
    m_pAttr            = NULL;
    m_pParent        = NULL;    
    m_pNext            = NULL;    
    XMLTreeParser parser( this );
    parser.ParseBuffer( buf );
}

XMLNode::~XMLNode()
{
    while (m_pChild)
    {
        XMLNode* pChild = m_pChild->NextSibling(); 
        delete m_pChild;
        m_pChild = pChild;
    }
} // XMLNode::~XMLNode

bool XMLNode::Read( InStream& is )
{
    int size = is.GetTotalSize();
    XMLTreeParser parser( this );
    char* pData = new char[size + 1];
    pData[size] = 0;
    bool allOK = false;
    if (is.Read( pData, size ))
    {
        allOK = parser.ParseBuffer( pData );
    }
    delete []pData;
    return allOK;
}

static const int c_XMLWriteBufSize = 65536;
void XMLNode::WriteAttr( OutStream& os )
{
    static char buf[c_XMLWriteBufSize];
    sprintf( buf, " %s=\"%s\"", m_Tag.c_str(), m_Value.c_str() );
    os.Write( buf, strlen( buf ) );
    if (m_pNext) m_pNext->WriteAttr( os );
} // XMLNode::WriteAttr

void XMLNode::Write( OutStream& os )
{
    static char buf[c_XMLWriteBufSize];

    sprintf( Indent( buf ), "<%s", m_Tag.c_str() );
    os.Write( buf, strlen( buf ) );

    if (m_pAttr)
    {
        m_pAttr->WriteAttr( os );
    }

    if (m_pChild) sprintf( buf, ">\r\n" ); 
    else if (m_Value[0]) sprintf( buf, ">" );
    else sprintf( buf, "/>\r\n" );

    os.Write( buf, strlen( buf ) );
    
    if (m_pChild)
    {
        s_Indent++;
        while (m_pChild)
        {
            m_pChild->Write( os );
            m_pChild = m_pChild->NextSibling();
        }
        s_Indent--;
        sprintf( Indent( buf ), "</%s>\r\n", m_Tag.c_str() );
    }
    else if (m_Value[0])
    {
        os.Write( m_Value.c_str(), m_Value.size() );
        sprintf( buf, "</%s>\r\n", m_Tag.c_str() );
    } else return;

    os.Write( buf, strlen( buf ) );    
} // XMLNode::Write

bool XMLNode::HasTag( const char* tag )
{
    if (s_bCaseIndependent)
    {
        return (stricmp( m_Tag.c_str(), tag ) == 0);
    }
    else
    {
        return (strcmp( m_Tag.c_str(), tag ) == 0);
    }
} // XMLNode::HasTag

XMLNode* XMLNode::FindChild( const char* tag ) const
{
    XMLNode* cNode = m_pChild;
    while (cNode)
    {
        if (cNode->HasTag( tag )) return cNode;
        cNode = cNode->m_pNext;
    }
    return NULL;
} // XMLNode::FindChild

XMLNode* XMLNode::FindAttr( const char* name ) const
{
    XMLNode* cNode = m_pAttr;
    while (cNode)
    {
        if (cNode->HasTag( name )) return cNode;
        cNode = cNode->m_pNext;
    }
    return NULL;
} // XMLNode::FindAttr

XMLNode* XMLNode::AddChild( const char* tag )
{
    XMLNode* newNode = new XMLNode();
    newNode->m_pParent = this;
    newNode->SetTag( tag );
    if (!m_pChild) { m_pChild = newNode; return newNode; }
    XMLNode* pHead = m_pChild;
    while (pHead->m_pNext) pHead = pHead->m_pNext;
    pHead->m_pNext = newNode;
    return newNode;
}

XMLNode* XMLNode::AddChild( XMLNode* pChild )
{
    if (!m_pChild) { m_pChild = pChild; return pChild; }
    XMLNode* pHead = m_pChild;
    while (pHead->m_pNext) pHead = pHead->m_pNext;
    pHead->m_pNext = pChild;
    return pChild;
}

int    XMLNode::GetNChildren() const
{
    int nCh = 0;
    XMLNode* pHead = m_pChild;
    while (pHead){ pHead = pHead->m_pNext; nCh++; }
    return nCh;
}

int    XMLNode::GetNAttr() const
{
    int nCh = 0;
    XMLNode* pHead = m_pAttr;
    while (pHead){ pHead = pHead->m_pNext; nCh++; }
    return nCh;
}

XMLNode* XMLNode::GetChild( int idx )
{
    XMLNode* pChild = FirstChild();
    while (idx-- > 0 && pChild) pChild = pChild->NextSibling();
    return pChild;
} // XMLNode::GetChild

static const char* c_SpecialSymbols[][2] = {
    { "&lt;",   "<"  }, 
    { "&rt",    ">"  }, 
    { "&#xA;",  "\n" },
    { "&amp;",  "&"  },
    { "&apos;", "'"  },
    { "&quot;", "\""  }
};

void XMLNode::Flatten()
{
    const char* pStr = m_Value.c_str();  
} // XMLNode::Flatten

void XMLNode::Unflatten()
{

} // XMLNode::Unflatten

/*****************************************************************************/
/*    XMLTreeParser implementation
/*****************************************************************************/
void XMLTreeParser::OnOpen( const char* tag )
{
    XMLNode* pNode = NULL;
    if (m_NodeStack.size() > 0)
    {
        XMLNode* pTop = m_NodeStack.top();
        pNode =  pTop->AddChild( tag );
    }
    else
    {
        m_pRoot->SetTag( tag );
        pNode = m_pRoot;
    }
    m_NodeStack.push( pNode );
    m_pCurNode = pNode;
} // XMLTreeParser::OnOpen

void XMLTreeParser::OnClose( const char* tag )
{
    if (!m_pCurNode || !m_pCurNode->HasTag( tag ))
    {
        Log.Warning( "Unexpected closing tag </%s> in .xml root node %s", m_pRoot->GetTag() );
        return;
    }
    if (m_NodeStack.size() == 0) { m_pCurNode = NULL; return; }
    m_NodeStack.pop();
    m_pCurNode = (m_NodeStack.size() > 0) ? m_NodeStack.top() : NULL;
} // XMLTreeParser::OnClose

void XMLTreeParser::OnValue( const char* value )
{
    if (m_pCurNode) m_pCurNode->SetValue( value );
    else Log.Error( "Unexpected value %s in .xml root node %s", value, m_pRoot->GetTag() );
} // XMLTreeParser::OnValue

void XMLTreeParser::OnAttribute( const char* name, 
                                const char* value )
{
    if (m_pCurNode) m_pCurNode->AddAttr( name, value );
    else Log.Error( "Unexpected attribute %s in .xml root node %s", name, m_pRoot->GetTag() );
} // XMLTreeParser::OnAttribute

/*****************************************************************************/
/*    String pooling
/*****************************************************************************/

static std::vector<const char*>    s_StrPool;
const char* GetPooledString( DWORD id )
{
    if (id >= s_StrPool.size()) return "";
    return s_StrPool[id];
}

DWORD CreatePooledString( const char* pStr, int len )
{
    char* pAllocStr = new char[len + 1];
    strncpy( pAllocStr, pStr, len );
    pAllocStr[len] = 0;
    DWORD id = s_StrPool.size();
    s_StrPool.push_back( pAllocStr );
    return id;
}

void ClearStringPool()
{
    int nStr = s_StrPool.size(); 
    for (int i = 0; i < nStr; i++) delete []s_StrPool[i];
    s_StrPool.clear();
}






