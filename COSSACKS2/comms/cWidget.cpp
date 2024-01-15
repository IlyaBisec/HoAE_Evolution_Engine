#include "comms.h"

cList<cWidget *> cWidgets::m_Widgets[cWidgets::TopLayer + 1];

//-----------------------------------------------------------------------------
// cWidgets::AddWidget
//-----------------------------------------------------------------------------
int cWidgets::AddWidget(cWidget *pWidget, const int Layer /*= 0*/) {
	cAssert(Layer >= 0 && Layer <= TopLayer);
	
	int index = -1;
	for(int i = 0; i < m_Widgets[Layer].Count(); i++) { // Searching for empty widget slot:
		if(NULL == m_Widgets[Layer][i]) {
			m_Widgets[Layer][i] = pWidget;
			index = i;
			break;
		}
	}
	if(-1 == index) { // There is no empty slot. Add one:
		index = m_Widgets[Layer].Add(pWidget);
	}
	return index + (Layer << LayerIndexShift);
} // cWidgets::AddWidget

//-----------------------------------------------------------------------------
// cWidgets::GetWidget
//-----------------------------------------------------------------------------
cWidget * cWidgets::GetWidget(const int id) {
	const int index = id & ((1 << LayerIndexShift) - 1);
	const int Layer = id >> LayerIndexShift;

	cAssert(Layer >= 0 && Layer <= TopLayer);
	cAssert(index >= 0 && index < m_Widgets[Layer].Count());

	if(Layer >= 0 && Layer <= TopLayer && index >= 0 && index < m_Widgets[Layer].Count()) {
		return m_Widgets[Layer][index];
	} else {
		return NULL;
	}
} // cWidgets::GetWidget

//-----------------------------------------------------------------------------
// cWidgets::GetWidget
//-----------------------------------------------------------------------------
cWidget * cWidgets::GetWidget(const char *Name) {
	for(int Layer = 0; Layer <= TopLayer; Layer++) {
		for(int index = 0; index < m_Widgets[Layer].Count(); index++) {
			cWidget *w = m_Widgets[Layer][index];
			if(!w) {
				continue;
			}
			if(w->GetName() == Name) {
				return w;
			}
		}
	}
	return NULL;
} // cWidgets::GetWidget

//-----------------------------------------------------------------------------
// cWidgets::FreeWidget
//-----------------------------------------------------------------------------
void cWidgets::FreeWidget(const int id) {
	const int index = id & ((1 << LayerIndexShift) - 1);
	const int Layer = id >> LayerIndexShift;
	
	cAssert(Layer >= 0 && Layer <= TopLayer);
	cAssert(index >= 0 && index < m_Widgets[Layer].Count());

	if(Layer >= 0 && Layer <= TopLayer && index >= 0 && index < m_Widgets[Layer].Count()) {
		if(m_Widgets[Layer][index] != NULL) {
			delete m_Widgets[Layer][index];
			m_Widgets[Layer][index] = NULL;
		}
	}
} // cWidgets::FreeWidget

//-----------------------------------------------------------------------------
// cWidgets::FreeLayer
//-----------------------------------------------------------------------------
void cWidgets::FreeLayer(const int Layer) {
	cAssert(Layer >= 0 && Layer <= TopLayer);

	if(Layer >= 0 && Layer <= TopLayer) {
		for(int index = 0; index < m_Widgets[Layer].Count(); index++) {
			if(m_Widgets[Layer][index] != NULL) {
				delete m_Widgets[Layer][index];
				m_Widgets[Layer][index] = NULL;
			}
		}
	}
} // cWidgets::FreeLayer

//-----------------------------------------------------------------------------
// cWidgets::FreeAll
//-----------------------------------------------------------------------------
void cWidgets::FreeAll() {
	for(int Layer = 0; Layer <= TopLayer; Layer++) {
		FreeLayer(Layer);
	}
} // cWidgets::FreeAll

int cWidgets::m_idSendTo = -1;
int cWidgets::m_idCurProcessing = -1;

// cWidgets::SetCapture
void cWidgets::SetCapture() {
	cAssertM(-1 == m_idSendTo, "SetCapture() - Input is already captured");
	m_idSendTo = m_idCurProcessing;
}

// cWidgets::ReleaseCapture
void cWidgets::ReleaseCapture() {
	m_idSendTo = -1;
}

//-----------------------------------------------------------------------------
// cWidgets::HandleInputEvents
//-----------------------------------------------------------------------------
void cWidgets::HandleInputEvents() {
	while(cInput::Event *pEvent = cInput::GetEvent()) {
		cWidget *SendTo = m_idSendTo != -1 ? GetWidget(m_idSendTo) : NULL;
		if(cInput::Event::TYPE_BUTTON == pEvent->Type) {
			if(/*pEvent->IsMouseButton && */SendTo) { // Mouse button and mouse input is captured:
				pEvent->IsPressed ? SendTo->OnButtonDown(pEvent->Code) : SendTo->OnButtonUp(pEvent->Code);
				if(pEvent->DeltaWheel != 0) {
					SendTo->OnMouseWheel(pEvent->DeltaWheel);
				}
			} else { // Send to all widgets.
				bool Skip = false;
				for(int Layer = TopLayer; Layer >= 0; Layer--) {
					for(int i = 0; i < m_Widgets[Layer].Count(); i++) {
						cWidget *w = m_Widgets[Layer][i];
						if(!w) {
							continue;
						}
						m_idCurProcessing = i + (Layer << LayerIndexShift);
						bool r = pEvent->IsPressed ? w->OnButtonDown(pEvent->Code) : w->OnButtonUp(pEvent->Code);
						if(r) {
							Skip = true;
							break;
						}
						if(pEvent->DeltaWheel != 0) {
							w->OnMouseWheel(pEvent->DeltaWheel);
						}
					}
					if(Skip) {
						break;
					}
				}
			}
		} else if(cInput::Event::TYPE_MOUSEMOVE == pEvent->Type) {
			if(SendTo) { // Mouse input is captured.
				SendTo->OnMouseMove(pEvent->DeltaX, pEvent->DeltaY);
			} else {
				bool Skip = false;
				for(int Layer = TopLayer; Layer >= 0; Layer--) {
					for(int i = 0; i < m_Widgets[Layer].Count(); i++) {
						cWidget *w = m_Widgets[Layer][i];
						if(!w) {
							continue;
						}
						m_idCurProcessing = i + (Layer << LayerIndexShift);
						if(w->OnMouseMove(pEvent->DeltaX, pEvent->DeltaY)) {
							Skip = true;
							break;
						}
					}
					if(Skip) {
						break;
					}
				}
			}
		}
		delete pEvent;
	}
} // cWidgets::HandleInputEvents

//-----------------------------------------------------------------------------
// cWidgets::Render
//-----------------------------------------------------------------------------
void cWidgets::Render(const int Layer /*= -1*/) {
	for(int l = 0; l <= TopLayer; l++) {
		if(l == Layer || Layer == -1) {
			for(int i = 0; i < m_Widgets[l].Count(); i++) {
				cWidget *w = m_Widgets[l][i];
				if(!w) {
					continue;
				}
				w->OnRender();
			}
		}
	}
} // cWidgets::Render