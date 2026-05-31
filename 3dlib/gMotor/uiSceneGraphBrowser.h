/*****************************************************************************/
/*	File:	uiSceneGraphBrowser.h
/*	Author:	Ruslan Shestopalyuk
/*	Date:	07-07-2003
/*****************************************************************************/
#ifndef __EDSGBROWSER_H__
#define __EDSGBROWSER_H__

namespace sg{
class ObjectInspector;
/*****************************************************************************/
/*	Class:	SceneGraphBrowser
/*	Desc:	Browser of the scene graph
/*****************************************************************************/
class SceneGraphBrowser : public NodeTree
{
public:
						SceneGraphBrowser		();
	bool				OnChar					( DWORD charCode, DWORD flags );
	bool				OnKeyDown				( DWORD keyCode, DWORD flags );
	
	virtual void		Render					();

	NODE(SceneGraphBrowser, NodeTree, SGBR);

protected:
	ObjectInspector*		m_pInspector;
	Node*					m_pBindNode;			
}; // class SceneGraphBrowser

}; // namespace sg

#endif // __EDSGBROWSER_H__
