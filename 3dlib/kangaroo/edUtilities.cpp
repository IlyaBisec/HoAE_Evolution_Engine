/*****************************************************************************/
/*	File:	edUtilities.cpp
/*	Author:	Ruslan Shestopalyuk
/*	Desc:	Glues sequence of pictures onto single picture
/*	Date:	15.04.2004
/*****************************************************************************/
#include "stdafx.h"
#include "sg.h"
#include <direct.h>
#include "edUtilities.h"
#include "kDirIterator.h"
#include "sgAnimationBlock.h"
#include "sgEffect.h"
#include "kFilePath.h"



/*****************************************************************************/
/*	Utilities implementation
/*****************************************************************************/
Utilities::Utilities()
{
}

void Utilities::Expose( PropertyMap& pm )
{
	pm.start<Parent>( "Utilities", this );
    pm.m( "ArrangeTextures", ArrangeTextures );
}

void Utilities::Process()
{
} // Utilities::Process

struct TextureEntry
{
    std::string     m_Path;
    bool            m_bEffect;
    bool            m_bRemoved;
    std::string     m_Model;

    TextureEntry( const char* path ) : 
            m_Path( path ), m_bEffect( false ), m_bRemoved( true ) {}
    TextureEntry() : m_bEffect( false ), m_bRemoved( true ) {}

}; // struct TextureEntry

struct ModelEntry
{
    std::string     m_Path;
    ModelEntry( const char* path ) : m_Path( path ){}
    ModelEntry(){}

}; // struct ModelEntry

void GatherTextureEntries( const char* path, std::vector<TextureEntry>& entries )
{
    DirIterator dit( path );
    while (dit)
    {
        if (dit.IsFile())
        {
            entries.push_back( TextureEntry( dit.GetFullFilePath() ) );
        }
        else if (dit.IsDirectory())
        {
            GatherTextureEntries( dit.GetFullFilePath(), entries );
        }
        ++dit;
    }
} // GatherTextureEntries

void GatherModelEntries( const char* path, std::vector<ModelEntry>& entries )
{
    DirIterator dit( path );
    while (dit)
    {
        if (dit.IsFile())
        {
            entries.push_back( ModelEntry( dit.GetFullFilePath() ) );
        }
        else if (dit.IsDirectory())
        {
            GatherModelEntries( dit.GetFullFilePath(), entries );
        }
        ++dit;
    }
} // GatherModelEntries

void SetTextureUsed( const char* tName, std::vector<TextureEntry>& entries, bool bEffect = false )
{
    FilePath fPath( tName );
    //  find texture with that name, set it as used
    for (int j = 0; j < entries.size(); j++)
    {
        FilePath ePath( entries[j].m_Path.c_str() );
        if (!strcmp( fPath.GetFileName(), ePath.GetFileName() ))
        {
            entries[j].m_bRemoved = false;
            entries[j].m_bEffect = bEffect;
        }
    }
} // SetTextureUsed

/*---------------------------------------------------------------------------*/
/*	Func:	Utilities::ArrangeTextures
/*	Desc:	Cleans up a mess in the textures folder
/*---------------------------------------------------------------------------*/
void Utilities::ArrangeTextures()
{
    assert( false );
    const char* rootDir = "";//GetRootDirectory();
    char texPath[_MAX_PATH], mdlPath[_MAX_PATH], effPath[_MAX_PATH];
    sprintf( texPath, "%s\\Textures\\", rootDir );
    sprintf( mdlPath, "%s\\Models\\", rootDir );
    sprintf( effPath, "%s\\Models\\Effects\\", rootDir );

    chdir( texPath );

    //  create common directories we need
    mkdir( "_Removed"   );
    mkdir( "Common"     );
    mkdir( "Particles"  );

    //  create registry of all textures in the textures/ folder subtree
    std::vector<TextureEntry> texFiles;
    GatherTextureEntries( texPath, texFiles );
    
    //  create registry of all models in the Models/ folder 
    std::vector<ModelEntry> mdlFiles;
    GatherModelEntries( mdlPath, mdlFiles );
    //  scan through every models and find which textures it uses
    int nM = mdlFiles.size();
    for (int i = 0; i < nM; i++)
    {
        ModelEntry& me = mdlFiles[i];
        FInStream is( me.m_Path.c_str() );
        if (is.NoFile()) continue;
        SNode* pNode = SNode::UnserializeSubtree( is );
        if (pNode->IsA<AnimationBlock>()) continue;
        if (!pNode) continue;
        Iterator it( pNode );
        while (it)
        {
            const char* tName = "";
            SNode* pCurNode = (SNode*)*it;
            if (pCurNode->IsA<Texture>())
            {
                Texture* pTex = (Texture*)pCurNode;
                tName = pTex->GetName();
                SetTextureUsed( tName, texFiles, false );
            }
            else if (pCurNode->IsA<PRenderer>())
            {
                PRenderer* pR = (PRenderer*)pCurNode;
                tName = pR->GetTexName();
                SetTextureUsed( tName, texFiles, true );
                tName = pR->GetTexName2();
                SetTextureUsed( tName, texFiles, true );
            }
            ++it;
        }
    }
    
    //  sort all textures into corresponding folders
    int nTex = texFiles.size();
    char newPath[_MAX_PATH];
    for (int i = 0; i < nTex; i++)
    {
        TextureEntry& t = texFiles[i];
        FilePath fPath( t.m_Path.c_str() );
        if (t.m_bRemoved)
        {
            sprintf( newPath, "%s_Removed\\%s", texPath, fPath.GetFileWithExt() );
        }
        else if (t.m_bEffect)
        {
            sprintf( newPath, "%sParticles\\%s", texPath, fPath.GetFileWithExt() );
        }
        else
        {
            sprintf( newPath, "%sCommon\\%s", texPath, fPath.GetFileWithExt() );
        }
        if (!stricmp( t.m_Path.c_str(), newPath )) continue;
        CopyFile( t.m_Path.c_str(), newPath, FALSE );
        DeleteFile( t.m_Path.c_str() );
    }

} // Utilities::ArrangeTextures



