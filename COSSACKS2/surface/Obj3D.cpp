#include "stdheader.h"
#include "Obj3D.h"
#include "kSystemDialogs.h"
#include "kContext.h"
#include "IShadowManager.h"
#include "PropertyPanel.h"
#include "..\GameInterface.h"

ObjectsList SceneObjExtension::Objects;
bool SceneObjExtension::EditMode=false;
int SceneObjExtension::LastPick=-1;
int SceneObjExtension::CurrSelModel=-1;
void SceneObject::DeleteObject(){
	for(int i=0;i<SceneObjExtension::Objects.Objects.GetAmount();i++){
		if(SceneObjExtension::Objects.Objects[i]==this){
			SceneObjExtension::Objects.Objects.DelElement(i);
			PropertyPanel::Discard();
			PropertyPanel::Hide();
			SceneObjExtension::CurrSelModel=-1;
			return;
		}
	}
}
bool SceneObjExtension::ActivateState(const char* ObjectName,const char* StateName,SceneObject* Curr){
	if(ObjectName && ObjectName[0]){
		for(int i=0;i<Objects.Objects.GetAmount();i++){
			if(Objects.Objects[i]->Name.equal(ObjectName)){
				Curr=Objects.Objects[i];
				break;
			}
		}
	}
	if(Curr && StateName && StateName[0]){		
		for(int i=0;i<Curr->States.GetAmount();i++){
			if(Curr->States[i]->Name.equal(StateName)){
				Curr->CurrStateIndex=i;
				Curr->States[i]->CalcAB();
				Curr->States[i]->ActivationTime=TrueTime;
				return true;
			}
		}		
	}
	return false;
}
void TransformThisObject::Action(SceneObject* Current){
	SceneObjExtension::ActivateState("",NextStateName.cpchar(),Current);
}
void TransformOtherObject::Action(SceneObject* Current){
	SceneObjExtension::ActivateState(ObjectName.cpchar(),NextStateName.cpchar(),Current);
}
void BornWeaponOnObject::Action(SceneObject* Current){
	if(Weapon.Get()){
		if(ObjectName.pchar()[0]){
			for(int i=0;i<SceneObjExtension::Objects.Objects.GetAmount();i++){
				if(SceneObjExtension::Objects.Objects[i]->Name.equal(ObjectName)){
					Current=SceneObjExtension::Objects.Objects[i];
				}
			}
		}
		if(Current){
			Matrix4D& m=Current->States[Current->CurrStateIndex]->Transform;
			GameInterface::CreateSpecialEffect(Weapon.Get()->Name.pchar(),m.e30,m.e31,m.e32,m.e30,m.e31,m.e32+500);
		}
	}
}
void SceneObjectState::Init(){
	ModelID=-1;
	AnimID=-1;
	StartAnimTime=0;
	FinalAnimTime=1;
	StartColor=0xFF808080;
	FinalColor=0xFF808080;
	AnimPlaySpeed=1.0f;
}
SceneObjExtension::SceneObjExtension(){
	REG_CLASS(SceneObject);
	REG_CLASS(BaseSceneObject);
	REG_CLASS(SceneObjectState);
	REG_CLASS(BasicStateAction);
	REG_CLASS(TransformThisObject);
	REG_CLASS(TransformOtherObject);
	REG_CLASS(BornWeaponOnObject);	
	m_bActive=false;
}
void SceneObjExtension::ProcessingGame(){
	int n=Objects.Objects.GetAmount();
	for(int i=0;i<n;i++){
		SceneObject* SOB=Objects.Objects[i];
		int ns=SOB->States.GetAmount();
		int ci=SOB->CurrStateIndex;
		if(ci<ns){
            BaseSceneObject* SO=Objects.Objects[i]->States[SOB->CurrStateIndex];
			if(SO->ActivationTime==0)SO->ActivationTime=TrueTime;			
			int dtc=(TrueTime-SO->ActivationTime)*SO->AnimPlaySpeed;
			int dta=SO->StateTimeLength*(SO->FinalAnimTime-SO->StartAnimTime);
			SO->CurrStateTime=SO->StateTimeLength*SO->StartAnimTime+(dta>0?dtc:-dtc);
			SO->Stage=dta!=0 ? float(dtc)/abs(dta) : 0;
			if(dtc>=abs(dta)){
				int n=SO->ActionOnEndFrame.GetAmount();
				for(int j=0;j<n;j++){
					SO->ActionOnEndFrame[j]->Action(SOB);
				}
				SO->CurrStateTime=SO->StateTimeLength*SO->StartAnimTime;
				SO->Stage=0;
				SO->ActivationTime=TrueTime;
			}			
		}
		for(int j=0;j<ns;j++)if(j!=ci){
			Objects.Objects[i]->States[j]->ActivationTime=0;
		}
	}
}
bool SceneObjExtension::OnMapLoading(xmlQuote& xml){
	ErrorPager p;
	Objects.Load(xml,&Objects,&p,NULL);
	for(int i=0;i<Objects.Objects.GetAmount();i++){
		int ns=Objects.Objects[i]->States.GetAmount();
		for(int j=0;j<ns;j++){
			Objects.Objects[i]->States[j]->CalcAB();
		}
	}
    return false;
}
bool SceneObjExtension::OnMapSaving(xmlQuote& xml){
	Objects.Save(xml,&Objects);		
    return true;
}
char c2mDir[512]="Models\\";
int SceneObjExtension::BrowseForModel(){
	OpenFileDialog dlg;	
	dlg.AddFilter( "c2m-files",    "*.c2m"     );	
	dlg.SetDefaultExtension( "c2m" );
	_chdir(c2mDir);
	if (dlg.Show()){
		_getcwd(c2mDir,512);
		_chdir(IRM->GetHomeDirectory());		
		std::string s=dlg.GetFilePath();
		IRM->ConvertPathToRelativeForm(s);
		return IMM->GetModelID(s.c_str());
	}
	_chdir(IRM->GetHomeDirectory());
	return -1;
}
Matrix4D SceneObjExtension::CalcFullTransform(){
	Vector3D Tr(m_CoordX,m_CoordY,m_CoordZ);		
	Matrix4D ms;
	ms.scaling(m_Scale.x,m_Scale.y,m_Scale.z);
	Matrix4D mt;		
	mt.translation(Tr);
	mt.mulLeft(ms);
	mt.mulLeft(m_T);
	return mt;
}
void SceneObjectState::CalcAB(){
	AB=IMM->GetBoundBox(ModelID);
	if(AB.minv.x==AB.maxv.x){
		AB.minv=Vector3D(-10,-10,-10);
		AB.maxv=Vector3D(10,10,10);
	}
	if(AB.minv.x<=AB.maxv.x){
		Center=(AB.minv+AB.maxv)*0.5f;
		Transform.transformPt(Center);
		Vector3D d=AB.maxv-AB.minv;
		Transform.transformPt(d);
		Radius=d.norm()/2;
		InvTransform.inverse(Transform);
	}
	StateTimeLength=IMM->GetAnimTime(AnimID);
}

void SceneObjectState::DrawAB(DWORD Color){
    float v[6];    	
	memcpy(&v[0],&AB.minv,3*4);
	memcpy(&v[3],&AB.maxv,3*4);
	Vector3D pos[8];
	for(int x=0;x<=1;x++){
		for(int y=0;y<=1;y++){
			for(int z=0;z<=1;z++){
                Vector3D p(v[x*3],v[y*3+1],v[z*3+2]);
				Transform.transformPt(p);
				pos[x*4+y*2+z]=p;
			}
		}
	}
	int idxs[]={0,1,2,3,4,5,6,7,0,2,1,3,5,7,4,6,1,5,3,7,2,6,0,4};
	for(int i=0;i<24;i+=2){
		rsLine(pos[idxs[i]],pos[idxs[i+1]],Color,Color);
	}
	rsFlush();
}
bool SceneObjectState::CheckPick(Ray3D& r){
	Ray3D ray=r;
	InvTransform.transformPt(ray.Orig());
	InvTransform.transformVec(ray.Dir());
	ray.Dir().normalize();
	return AB.Intersect(ray.Orig(),ray.Orig()+ray.Dir()*100000)==xsIntersects;    
}
extern int TrueTime;
void SceneObjectState::Render(SceneObject* ob){	
	PushEntityContext( DWORD(this) );
	int st=int(Stage*255);
	DWORD Color=MixDWORD(StartColor,FinalColor,255-st,st);
	IRS->SetColorConst(Color);
	IRS->SetTextureFactor(Color);
	IRS->SetShaderConst(0,1000000);
	IRS->SetShaderConst(1,1000000);
	IMM->StartModel(ModelID,Transform,CUR_CONTEXT);
	int T=IMM->GetAnimTime(AnimID);
	StateTimeLength=T;	
	IMM->AnimateModel(AnimID,CurrStateTime);
	IMM->DrawModel();
	IShadowMgr->AddCaster(ModelID,Transform);
	PopEntityContext();	
}
void SceneObjExtension::OnDrawOnMapAfterUnits(){
	LastPick=-1;
	int N=Objects.Objects.GetAmount();
	Frustum F=ICam->GetFrustum();
	Ray3D ray = ICam->GetPickRay( mouseX, mouseY );
	for(int i=0;i<N;i++){
		if(Objects.Objects[i]->States.GetAmount()>Objects.Objects[i]->CurrStateIndex){
			BaseSceneObject* st=Objects.Objects[i]->States[Objects.Objects[i]->CurrStateIndex];
			if( F.Overlap( Sphere( st->Center,st->Radius ) ) ){
				st->Render(Objects.Objects[i]);
				if(SceneObjExtension::EditMode){
					bool picked=st->CheckPick(ray);
					st->DrawAB(picked ? 0xFFFFFF00 : (i==CurrSelModel ? 0xFF00FF00 : 0xFFFF0000) );
					Vector3D p=(st->AB.minv+st->AB.maxv)*0.5f;
					st->Transform.transformPt(p);
					WorldToScreenSpace(p);
					static _str n;
					n=Objects.Objects[i]->Name.pchar();					
					if(n.pchar()[0]){
						if(st->Name.pchar()[0]){
							n+="[";
							n+=st->Name;
							n+="]";
						}
						int L=GetRLCStrWidth(n.pchar(),&WhiteFont);
						ShowString(p.x-L/2,p.y-10,n.pchar(),&WhiteFont);						
					}
					if(picked)LastPick=i; 
				}
			}
		}
	}	
	if(m_bActive){		
		PushEntityContext( Objects.Objects.GetAmount()*10000 + CurrModel + 123981 );		
		IRS->SetColorConst(0xFF808080);
		IRS->SetShaderConst(0,1000000);
		IRS->SetShaderConst(1,1000000);
		IMM->StartModel(CurrModel,CalcFullTransform(),CUR_CONTEXT);
		IMM->AnimateModel(CurrModel,0);
		IMM->DrawModel();
		PopEntityContext();
	}
}

bool SceneObjExtension::OnMouseHandling(int mx, int my, bool &LeftPressed, bool &RightPressed,int MapCoordX, int MapCoordY, bool OverMiniMap){
	if(EditMode){
		if(LeftPressed && LastPick!=-1){
			PropertyPanel::ShowProperty(Objects.Objects[LastPick]);            
			CurrSelModel=LastPick;
			return true;
		}
	}
	if(!m_bActive) return false;
	Ray3D ray = ICam->GetPickRay( mx, my );
	Vector3D iPt;
	ray.IntersectPlane(Plane::xOy,iPt);
	m_CoordX = iPt.x, m_CoordY = iPt.y;	
	if(LeftPressed) {
        SceneObject* obj=new SceneObject;
		SceneObjectState* st=new SceneObjectState;
		st->Transform=CalcFullTransform();
		st->ModelID=CurrModel;
		st->AnimID=-1;
		st->StartColor=0xFF808080;
		st->FinalColor=0xFF808080;
		st->CalcAB();
		obj->States.Add(st);
		Objects.Objects.Add(obj);
		m_bActive=false;
	}
	return true;
}
bool SceneObjExtension::OnMouseWheel(int Delta){
	if(!m_bActive) return false;
	const float cRotSpeed = 0.2f;
	const int cAlongZSpeed = 10;

	float Gear = GetKeyState(VK_TAB) < 0 ? 4.0f : 1.0f;
	float CurRotSpeed = cRotSpeed / Gear;
	float CurAlongZSpeed = cAlongZSpeed / Gear;
	bool fShift = GetKeyState(VK_LSHIFT) & 0x8000, fScale = GetKeyState(VK_MENU) & 0x8000,
		fGlobalRotation = GetKeyState(VK_LCONTROL) & 0x8000;
	Matrix4D T;

	// Mask of ZXC keys:
	cVec3 Mask = cVec3::Zero;
	if(GetKeyState('z') < 0 || GetKeyState('Z') < 0) {
		Mask += cVec3::AxisZ;
	}
	if(GetKeyState('x') < 0 || GetKeyState('X') < 0) {
		Mask += cVec3::AxisX;
	}
	if(GetKeyState('c') < 0 || GetKeyState('C') < 0) {
		Mask += cVec3::AxisY;
	}

	if(fShift) {
		if(Delta > 0) m_CoordZ += CurAlongZSpeed;
		else m_CoordZ -= CurAlongZSpeed;
	} else if(fScale) {
		if(Mask.IsZero()) Mask.Set(1);
		if(Delta > 0) m_Scale += Mask * m_ScalingSpeed * cVec3(1);
		else m_Scale -= Mask * m_ScalingSpeed;
		if(m_Scale.x < m_ScaleMin) m_Scale.x = m_ScaleMin;
		if(m_Scale.y < m_ScaleMin) m_Scale.y = m_ScaleMin;
		if(m_Scale.z < m_ScaleMin) m_Scale.z = m_ScaleMin;
	} else if(fGlobalRotation) {
		if(Mask.IsZero()) Mask = cVec3::AxisZ;
		if(Mask.z > 0) {
			T.rotation(Vector3D::oZ, cMath::Sign(Delta) * CurRotSpeed);
			m_T *= T;
		}
		if(Mask.x > 0) {
			T.rotation(Vector3D::oX, cMath::Sign(Delta) * CurRotSpeed);
			m_T *= T;
		}
		if(Mask.y > 0) {
			T.rotation(Vector3D::oY, cMath::Sign(Delta) * CurRotSpeed);
			m_T *= T;
		}
	} else {
		if(Mask.IsZero()) Mask = cVec3::AxisZ;
		if(Mask.z > 0) {
			T.rotation(Vector3D::oZ, cMath::Sign(Delta) * CurRotSpeed);
			m_T = T *= m_T;
		}
		if(Mask.x > 0) {
			T.rotation(Vector3D::oX, cMath::Sign(Delta) * CurRotSpeed);
			m_T = T *= m_T;
		}
		if(Mask.y > 0) {
			T.rotation(Vector3D::oY, cMath::Sign(Delta) * CurRotSpeed);
			m_T = T *= m_T;
		}
	}

	return true;
}
bool SceneObjExtension::CheckActivityOfEditor() { 
	return m_bActive; 
}
extern bool g_fZXCKeysAreBlocked;
extern bool g_fWheelIsBlocked;
void SceneObjExtension::ClearActivityOfEditor() {
	m_bActive = false;
	EditMode = false;
	g_fZXCKeysAreBlocked = false;
	g_fWheelIsBlocked = false;
}
void SceneObjExtension::ActivateEditor(){
    int mid=BrowseForModel();
	if(mid!=-1){
        m_bActive=true;
		CurrModel=mid;
		m_Scale.Set(1);
		m_ScalingSpeed = 0.1;
		m_ScaleMin = 0.1;
		m_T.setIdentity();
		g_fZXCKeysAreBlocked = true;
		g_fWheelIsBlocked = true;
	}
}
void SceneObjExtension::OnUnloading(){
	OnMapUnLoading();
}
bool SceneObjExtension::OnMapUnLoading(){
	Objects.Objects.Clear();
	return true;
}

