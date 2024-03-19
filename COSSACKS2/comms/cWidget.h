#pragma once

class cWidget {
public:
	virtual bool OnButtonDown(const int Code) { return false; }
	virtual bool OnButtonUp(const int Code) { return false; }
	virtual bool OnMouseMove(const int DeltaX, const int DeltaY) { return false; }
	virtual bool OnMouseWheel(const int Delta) { return false; }
	virtual void OnRender() {}

	const cStr & GetName() const { return m_Name; }
	void SetName(const char *Name) { m_Name = Name; }
private:
	cStr m_Name;
};

class cWidgets {
public:
	static const int TopLayer = 32;
	
	static int AddWidget(cWidget *pWidget, const int Layer = 0);
	static cWidget * GetWidget(const int id);
	static cWidget * GetWidget(const char *Name);
	static void FreeWidget(const int id);
	static void FreeLayer(const int Layer);
	static void FreeAll();
	
	static void HandleInputEvents();
	static void SetCapture(); // Causes the calling this function widget to take processing of all input events
	static void ReleaseCapture(); // till this function is called.
	
	static void Render(const int Layer = -1); // -1 - all
private:
	static const int LayerIndexShift = 16;
	static cList<cWidget *> m_Widgets[TopLayer + 1];
	static int m_idSendTo, m_idCurProcessing;
};