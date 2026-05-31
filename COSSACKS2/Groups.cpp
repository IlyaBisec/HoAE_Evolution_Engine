#include "stdheader.h"
#include "Groups.h"
word GetDir(int,int);
SortClass::SortClass(){
    NUids=0;
    MaxUID=0;
    Uids=NULL;
    //Usn=NULL;
    Parms=NULL;
};
SortClass::~SortClass(){
    if(MaxUID){
        free(Uids);
        free(Parms);
        //free(Usn);
    };
};
void SortClass::Sort(){
    if(NUids<2)return;
    int uids=int(Uids);
    //int sns=int(Usn);
    //int snsn;
    int parms=int(Parms);
    int nuids=NUids-1;
    byte fault;

    __asm{
        push    esi
        push    edi
        mov     esi,uids
        mov     edi,parms
        mov     ecx,nuids
lpp1:  
        //mov     eax,sns
        //mov     snsn,eax
        xor     eax,eax
        mov     esi,uids
        mov     edi,parms
        mov     ecx,nuids
        mov     ecx,nuids
        mov     fault,0
lpp3:
        mov     ebx,[edi]
        mov     edx,[edi+4]
        cmp     ebx,edx
        jle     lpp2
        mov     [edi],edx
        mov     [edi+4],ebx
        //xchg    ebx,snsn
        //mov     dx,[ebx]
        //xchg    dx,[ebx+2]
        //mov     [ebx],dx
        //xchg    ebx,snsn
        mov     bx,[esi]
        xchg    bx,[esi+2]
        mov     [esi],bx
        mov     fault,1
lpp2:   add     esi,2
        add     edi,4
        //add     snsn,2
        dec     ecx
        jnz     lpp3
        cmp     fault,0
        jnz      lpp1
        pop     esi
        pop     edi
    };
};
void SortClass::CheckSize(int Size){
    if(!Size)return;
    if(Size>MaxUID){
        if(MaxUID){
            free(Uids);
            free(Parms);
            //free(Usn);
        };
        Uids=znew(word,Size);
        Parms=znew(int,Size);
        MaxUID=Size;
    };
};
word SortClass::CreateByX(word* UIDS,int NUIDS,bool Direction){
    CheckSize(NUIDS);
    word realn=0;
    if(Direction){
        for(int i=0;i<NUIDS;i++){
            word MID=UIDS[i];
            if(MID!=0xFFFF){
                OneObject* OB=Group[MID];
                if(OB&&!OB->Sdoxlo){
                    Uids[realn]=MID;
                    //Usn[realn]=USN[i];
                    UIDS[realn]=MID;
                    Parms[realn]=OB->RealX;
                    realn++;
                };
            };
        };
    }else{
        for(int i=0;i<NUIDS;i++){
            word MID=UIDS[i];
            if(MID!=0xFFFF){
                OneObject* OB=Group[MID];
                if(OB&&!OB->Sdoxlo){
                    Uids[realn]=MID;
                    //Usn[realn]=USN[i];
                    UIDS[realn]=MID;
                    Parms[realn]=-OB->RealX;
                    realn++;
                };
            };
        };
    };
    NUids=realn;
    return realn;
};
word SortClass::CreateByY(word* UIDS,int NUIDS,bool Direction){
    CheckSize(NUIDS);
    word realn=0;
    if(Direction){
        for(int i=0;i<NUIDS;i++){
            word MID=UIDS[i];
            if(MID!=0xFFFF){
                OneObject* OB=Group[MID];
                if(OB&&!OB->Sdoxlo){
                    Uids[realn]=MID;
                    //Usn[realn]=USN[i];
                    UIDS[realn]=MID;
                    Parms[realn]=OB->RealY;
                    realn++;
                };
            };
        };
    }else{
        for(int i=0;i<NUIDS;i++){
            word MID=UIDS[i];
            if(MID!=0xFFFF){
                OneObject* OB=Group[MID];
                if(OB&&!OB->Sdoxlo){
                    Uids[realn]=MID;
                    //Usn[realn]=USN[i];
                    UIDS[realn]=MID;
                    Parms[realn]=-OB->RealY;
                    realn++;
                };
            };
        };
    };
    NUids=realn;
    return realn;
};
word SortClass::CreateByR(word* UIDS,int NUIDS,bool Direction,int x,int y){
    CheckSize(NUIDS);
    word realn=0;
    if(Direction){
        for(int i=0;i<NUIDS;i++){
            word MID=UIDS[i];
            if(MID!=0xFFFF){
                OneObject* OB=Group[MID];
                if(OB&&!OB->Sdoxlo){
                    Uids[realn]=MID;
                    //Usn[realn]=USN[i];
                    UIDS[realn]=MID;
                    Parms[realn]=Norma(OB->RealX-x,OB->RealY-y);
                    realn++;
                };
            };
        };
    }else{
        for(int i=0;i<NUIDS;i++){
            word MID=UIDS[i];
            if(MID!=0xFFFF){
                OneObject* OB=Group[MID];
                if(OB&&!OB->Sdoxlo){
                    Uids[realn]=MID;
                    //Usn[realn]=USN[i];
                    UIDS[realn]=MID;
                    Parms[realn]=-Norma(OB->RealX-x,OB->RealY-y);
                    realn++;
                };
            };
        };
    };
    NUids=realn;
    return realn;
};
word SortClass::CreateByLine(word* UIDS,int NUIDS,int dx,int dy){
	CheckSize(NUIDS);
    word realn=0;
    for(int i=0;i<NUIDS;i++){
        word MID=UIDS[i];
        if(MID!=0xFFFF){
            OneObject* OB=Group[MID];
            if(OB&&!OB->Sdoxlo){
                Uids[realn]=MID;
                //Usn[realn]=USN[i];
                UIDS[realn]=MID;
                Parms[realn]=(OB->RealX>>5)*dx+(OB->RealY>>5)*dy;
                realn++;
            };
        };
    };
    NUids=realn;
    return realn;
};
word SortClass::CreateByLineForShips(word* UIDS,int NUIDS,int dx,int dy){
	CheckSize(NUIDS);
    word realn=0;
    for(int i=0;i<NUIDS;i++){
        word MID=UIDS[i];
        if(MID!=0xFFFF){
            OneObject* OB=Group[MID];
            if(OB&&!OB->Sdoxlo){
                Uids[realn]=MID;
                //Usn[realn]=USN[i];
                UIDS[realn]=MID;
                Parms[realn]=(OB->RealX>>5)*dx+(OB->RealY>>5)*dy;
				if(OB->newMons->Usage==GaleraID)Parms[realn]-=100000;
                realn++;
            };
        };
    };
    NUids=realn;
    return realn;
};
void SortClass::CopyToDst(word* Dst,word* Sns){
    memcpy(Dst,Uids,NUids<<1);
    for(int i=0;i<NUids;i++){
        OneObject* OB=Group[Uids[i]];
        if(OB)Sns[i]=OB->Serial;
    };
};
void SortClass::Copy(word* Dst){
    memcpy(Dst,Uids,NUids<<1);
};
//-----------------------Usage of the SortClass----------------------
extern SortClass UNISORT;
word SortUnitsByR(word* urefs,word* snrefs,int nu,int x,int y){
    int n=UNISORT.CreateByR(urefs,nu,true,x,y);
    UNISORT.Sort();
    UNISORT.CopyToDst(urefs,snrefs);
    return n;
};
//--------------------Selected groups management---------------------
//1.Creating abstract group: square group without the place on map.
//  It means, that group will be sorted specially:
//  1.a. Sorting by Y
//  1.b. Sort Lx elements by X
//  1.c. Sort next Lx elements by X
//  ....
//2.Placing on the map:
//???

PositionOrder::PositionOrder(){
    NUnits=0;
    MaxUnit=0;
    Ids=NULL;
    px=NULL;
    py=NULL;
};
PositionOrder::~PositionOrder(){
    if(MaxUnit){
        free(Ids);
        free(px);
        free(py);
    };
    NUnits=0;
    MaxUnit=0;
    Ids=NULL;
    px=NULL;
    py=NULL;
};
void PositionOrder::CheckSize(int Size){
    if(Size>MaxUnit){
        if(MaxUnit>0){
            free(Ids);
            free(px);
            free(py);
        };
        Ids=znew(word,Size);
        px =znew(int,Size);
        py =znew(int,Size);
        MaxUnit=Size;
    };
    NUnits=Size;
};
void PositionOrder::Create(word* IDS,int NIDS){
    CheckSize(NIDS);
    memcpy(Ids,IDS,NIDS<<1);
};
word PositionOrder::CreateLinearPositions(int x,int y,word* IDS,int NIDS,int dx,int dy){
	Create(IDS,NIDS);
	UNISORT.CreateByLine(Ids,NUnits,dy>>4,-dx>>4);
	UNISORT.Sort();
	NIDS=UNISORT.NUids;
	CheckSize(NIDS);
	UNISORT.Copy(Ids);
	if(!(dx||dy))dx=1;
	int N=Norma(dx,dy);
	int dx1=(dx*96*16)/N;
	int dy1=(dy*96*16)/N;
	int x0=x-((dy1*NIDS)>>1);
	int y0=y+((dx1*NIDS)>>1);
	for(int i=0;i<NIDS;i++){
		px[i]=x0;
		py[i]=y0;
		x0+=dy1;
		y0-=dx1;
	};
	return N;
};
#ifdef COSSACKS2
int FORMDIST=270;
#else
int FORMDIST=360;
#endif
extern int FORMDIST;

#define MaxColumnTypes 16

void PositionOrder::CreateRotatedPositionsWithColumns(int x,int y,word* IDS,int NIDS,int dx,int dy){
	int NU[MaxColumnTypes];
	int Len[MaxColumnTypes];
	word* UNITS[MaxColumnTypes];
	memset(NU,0,sizeof NU);
	memset(Len,0,sizeof NU);
	memset(UNITS,0,sizeof UNITS);
	for(int i=0;i<MaxColumnTypes;i++){
		UNITS[i]=new word[NIDS];
	}
	for(int i=0;i<NIDS;i++){
        word ID=IDS[i];
		if(ID!=0xFFFF){
			OneObject* OB=Group[ID];
			if(OB){
				int CLM=OB->newMons->UnitsColumnIndex;
				if(CLM<MaxColumnTypes){
					UNITS[CLM][NU[CLM]++]=ID;
				}
			}
		}
	}
	PositionOrder temp[MaxColumnTypes];
	int summL=0;
	int nr=sqrt(dx*dx+dy*dy);
	if(nr){
		for(int i=0;i<MaxColumnTypes;i++){
			int n=temp[i].CreateRotatedPositions(x,y,UNITS[i],NU[i],dx,dy);
			int minP=10000000;
			int maxP=-10000000;		
			for(int j=0;j<n;j++){
				int xx=temp[i].px[j]-x;
				int yy=temp[i].py[j]-y;
				int dp=(dx*xx+dy*yy)/nr;
				if(dp<minP)minP=dp;
				if(dp>maxP)maxP=dp;
			}
			if(maxP>=minP){
				summL+=Len[i]=maxP-minP+60*16;
			}
		}    
	}
	if(summL){
		CheckSize(NIDS);
		int x0=dx*summL/nr/2;
		int y0=dy*summL/nr/2;
		int cp=0;
		for(int i=0;i<MaxColumnTypes;i++){
			x0-=dx*Len[i]/nr/2;
			y0-=dy*Len[i]/nr/2;
			int n=temp[i].NUnits;
			for(int j=0;j<n;j++){
                px[cp]=temp[i].px[j]+x0;
				py[cp]=temp[i].py[j]+y0;
				Ids[cp]=temp[i].Ids[j];
				cp++;
			}
			x0-=dx*Len[i]/nr/2;
			y0-=dy*Len[i]/nr/2;
		}
		NUnits=cp;
	}
	for(int i=0;i<MaxColumnTypes;i++){
		delete(UNITS[i]);
	}
}

word PositionOrder::CreateRotatedPositions(int x,int y,word* IDS,int NIDS,int dx,int dy){
	    //special sorting
	dx>>=4;
	dy>>=4;
	if(!(dx||dy))dx=1;
    Create(IDS,NIDS);
    //CheckSize(NIDS);
    int Lx=int(sqrt(NIDS));
    int Ly=Lx*5/3;
	Lx=Lx*3/5;
	if(NIDS<4){
		Lx=1;
		Ly=NIDS;
	};
	int dd=dx;
	dx=dy;
	dy=-dd;
    int nn=Lx*Ly;
    if(nn<NIDS){
        if(nn+Lx>=NIDS)Ly++;
        else{
            if(nn+Ly>=NIDS)Lx++;
            else{
                Ly++;
                Lx++;
            };
        };
    };
	nn=Lx*Ly;
    if(nn<NIDS){
        if(nn+Lx>=NIDS)Ly++;
        else{
            if(nn+Ly>=NIDS)Lx++;
            else{
                Ly++;
                Lx++;
            };
        };
    };
    UNISORT.CreateByLine(Ids,NUnits,dx>>4,dy>>4);
    UNISORT.Sort();
    int NU=UNISORT.NUids;
    CheckSize(NU);
    UNISORT.Copy(Ids);
    int Px=0;
    int Lx1;
    for(int iy=0;iy<Ly;iy++){
        Lx1=NU-Px;
        if(Lx1>Lx)Lx1=Lx;
        if(Lx1>0){
            UNISORT.CreateByLine(Ids+Px,Lx1,-dy>>4,dx>>4);
            UNISORT.Sort();
            UNISORT.Copy(Ids+Px);
            Px+=Lx1;
        };
    };
    //Sorting is finished now
    //Getting of the maximal size of Unit
    int maxR=0;
    word* IDE=Ids;
    word MID;
    int rr;
    OneObject* OB;
	int TP1=-1;
	int TP2=-1;
    for(int iy=0;iy<NU;iy++){
        MID=IDE[iy];
        OB=Group[MID];
        if(OB&&OB->NewMonst){
            rr=OB->newMons->Radius2;
            if(rr>maxR){
                maxR=rr;
            };
			if(TP1==-1)TP1=OB->NIndex;
			else if(TP1!=OB->NIndex)TP2=OB->NIndex;
        };
    };
    //Creating the position
	//if(maxR<516)maxR+=512;
    //else maxR+=1024;
	if(TP2!=-1&&maxR>FORMDIST*4)maxR=FORMDIST*4;
	maxR=maxR*3/4;
	maxR<<=2;
    int Maxx=(msx<<9)-768;
    int Maxy=(msy<<9)-768;
    //int BLx=Lx*maxR;
    //int BLy=Ly*maxR;
    //int SX=x-(((Lx-1)*maxR)>>1);
    //int SY=y-(((Ly-1)*maxR)>>1);
    //if(SX<768)SX=768;
    //if(SY<768)SY=768;
    //if(SX+BLx>Maxx)SX=Maxx-BLx;
    //if(SY+BLy>Maxy)SY=Maxy-BLy;
    //int UX=SX;
    //int UY=SY;
    int pos=0;
	int NR=Norma(dx,dy);
	int vx=(dx*maxR)/NR;
	int vy=(dy*maxR)/NR;
	int Dx=(-(Lx-1)*vy+(Ly-1)*vx)>>1;
	int Dy=((Lx-1)*vx+(Ly-1)*vy)>>1;
    for(int iy=0;iy<Ly;iy++){
        for(int ix=0;ix<Lx;ix++){
            if(pos<NU){
                px[pos]=x-ix*vy+iy*vx-Dx;
                py[pos]=y+ix*vx+iy*vy-Dy;
				word ID=Ids[pos];
				if(ID!=0xFFFF){
                    OneObject* OB=Group[ID];
					if(OB){
						int rp=OB->newMons->BRandomPos;
						if(rp){
							px[pos]+=((rando()%(rp+rp))-rp)<<4;
							py[pos]+=((rando()%(rp+rp))-rp)<<4;
						}
					}
				}
            };
            pos++;
        };
    };
    return NU;
};
word PositionOrder::CreateRotatedPositions2(int x,int y,word* IDS,int NIDS,int dx,int dy){
	//special sorting
	dx>>=4;
	dy>>=4;
	if(!(dx||dy))dx=1;
    Create(IDS,NIDS);
    //CheckSize(NIDS);
    int Lx=int(sqrt(NIDS));
    int Ly=Lx<<2;
	Lx>>=2;
	if(NIDS<10){
		Lx=1;
		Ly=NIDS;
	};
	int dd=dx;
	dx=dy;
	dy=-dd;
    int nn=Lx*Ly;
    if(nn<NIDS){
        if(nn+Lx>=NIDS)Ly++;
        else{
            if(nn+Ly>=NIDS)Lx++;
            else{
                Ly++;
                Lx++;
            };
        };
    };
	nn=Lx*Ly;
    if(nn<NIDS){
        if(nn+Lx>=NIDS)Ly++;
        else{
            if(nn+Ly>=NIDS)Lx++;
            else{
                Ly++;
                Lx++;
            };
        };
    };
    UNISORT.CreateByLine(Ids,NUnits,dx>>4,dy>>4);
    UNISORT.Sort();
    int NU=UNISORT.NUids;
    CheckSize(NU);
    UNISORT.Copy(Ids);
    int Px=0;
    int Lx1;
    for(int iy=0;iy<Ly;iy++){
        Lx1=NU-Px;
        if(Lx1>Lx)Lx1=Lx;
        if(Lx1>0){
            UNISORT.CreateByLine(Ids+Px,Lx1,-dy>>4,dx>>4);
            UNISORT.Sort();
            UNISORT.Copy(Ids+Px);
            Px+=Lx1;
        };
    };
    //Sorting is finished now
    //Getting of the maximal size of Unit
    int maxR=0;
    word* IDE=Ids;
    word MID;
    int rr;
    OneObject* OB;
    for(int iy=0;iy<NU;iy++){
        MID=IDE[iy];
        OB=Group[MID];
        if(OB&&OB->NewMonst){
            rr=OB->newMons->Radius2;
            if(rr>maxR){
                maxR=rr;
            };
        };
    };
    //Creating the position
	//if(maxR<516)maxR+=512;
    //else maxR+=1024;
	maxR<<=2;
    int Maxx=(msx<<9)-768;
    int Maxy=(msy<<9)-768;
    //int BLx=Lx*maxR;
    //int BLy=Ly*maxR;
    //int SX=x-(((Lx-1)*maxR)>>1);
    //int SY=y-(((Ly-1)*maxR)>>1);
    //if(SX<768)SX=768;
    //if(SY<768)SY=768;
    //if(SX+BLx>Maxx)SX=Maxx-BLx;
    //if(SY+BLy>Maxy)SY=Maxy-BLy;
    //int UX=SX;
    //int UY=SY;
    int pos=0;
	int NR=Norma(dx,dy);
	int vx=(dx*maxR)/NR;
	int vy=(dy*maxR)/NR;
	int Dx=(-(Lx-1)*vy+(Ly-1)*vx)>>1;
	int Dy=((Lx-1)*vx+(Ly-1)*vy)>>1;
    for(int iy=0;iy<Ly;iy++){
        for(int ix=0;ix<Lx;ix++){
            if(pos<NU){
                px[pos]=x-ix*vy+iy*vx-Dx;
                py[pos]=y+ix*vx+iy*vy-Dy;
            };
            pos++;
        };
    };
    return NU;
};
word PositionOrder::CreatePositions(int x,int y,word* IDS,int NIDS){
    //special sorting
    Create(IDS,NIDS);
    //CheckSize(NIDS);
    int Lx=int(sqrt(NIDS));
    int Ly=Lx;
    int nn=Lx*Ly;
    if(nn<NIDS){
        if(nn+Lx>=NIDS)Ly++;
        else{
            if(nn+Ly>=NIDS)Lx++;
            else{
                Ly++;
                Lx++;
            };
        };
    };
    UNISORT.CreateByY(Ids,NUnits,true);
    UNISORT.Sort();
    int NU=UNISORT.NUids;
    CheckSize(NU);
    UNISORT.Copy(Ids);
    int Px=0;
    int Lx1;
	int TP1=-1;
	int TP2=-1;
    for(int iy=0;iy<Ly;iy++){
        Lx1=NU-Px;
        if(Lx1>Lx)Lx1=Lx;
        if(Lx1>0){
            UNISORT.CreateByX(Ids+Px,Lx1,true);
            UNISORT.Sort();
            UNISORT.Copy(Ids+Px);
            Px+=Lx1;
        };
    };
    //Sorting is finished now
    //Getting of the maximal size of Unit
    int maxR=0;
    word* IDE=Ids;
    word MID;
    int rr;
    OneObject* OB;
    for(int iy=0;iy<NU;iy++){
        MID=IDE[iy];
        OB=Group[MID];
        if(OB&&OB->NewMonst){
            rr=OB->newMons->Radius2;
            if(rr>maxR){
                maxR=rr;
            };
			if(TP1==-1)TP1=OB->NIndex;
			else if(TP1!=OB->NIndex)TP2=OB->NIndex;
        };
    };
    //Creating the position
	//if(maxR<516)maxR+=512;
    //else maxR+=1024;
	maxR<<=2;
	maxR=768;//????????
    int Maxx=(msx<<9)-768;
    int Maxy=(msy<<9)-768;
    int BLx=Lx*maxR;
    int BLy=Ly*maxR;
    int SX=x-(((Lx-1)*maxR)>>1);
    int SY=y-(((Ly-1)*maxR)>>1);
    if(SX<768)SX=768;
    if(SY<768)SY=768;
    if(SX+BLx>Maxx)SX=Maxx-BLx;
    if(SY+BLy>Maxy)SY=Maxy-BLy;
    int UX=SX;
    int UY=SY;
    int pos=0;
    for(int iy=0;iy<Ly;iy++){
        for(int ix=0;ix<Lx;ix++){
            if(pos<NU){
                px[pos]=UX;
                py[pos]=UY;
                UX+=maxR;
            };
            pos++;
        };
        UX=SX;
        UY+=maxR;
    };
    return NU;
};

extern bool CmdDone[8192];
void RotUnit(OneObject* OB,char Dir,byte OrdType,byte);
bool GetObjInsubordination(OneObject* OB);

void PositionOrder::SendToPosition(byte Prio,byte OrdType){
    word MID;
	int cx=CenterX>>4;
	int cy=CenterY>>4;
    for(int i=0;i<NUnits;i++){
        MID=Ids[i];
        OneObject* OB=Group[MID];
        if(OB&&!CmdDone[MID]){
			if(GetObjInsubordination(OB)){
				continue;
			}
			
			// by vital
			OB->StandGround=false;
			OB->Guard=0xFFFF;
			//OB->ClearOrders();
			//OB->DestX=-1;

            //OB->NewMonsterSendTo(px[i],py[i],Prio,OrdType);
			if(LastDirection!=512){
				if(OB->BrigadeID==0xFFFF){
					addrand(cx);
					addrand(cy);
					addrand(px[i]);
					addrand(py[i]);
					switch(OrdType){
					case 0:
						if(!(OB->ActivityState==2&&OB->Attack)){
							if(Norma(OB->RealX-cx*16+px[i]+CenterX,OB->RealY-cy*16+py[i]+CenterY)>32){
								OB->NewMonsterSmartSendTo(cx,cy,(px[i]-CenterX)>>4,(py[i]-CenterY)>>4,Prio,0);
								if(LastDirection<256 && !OB->newMons->Transport)RotUnit(OB,LastDirection&255,2,Prio);
							}
						}
						break;
					case 1:
						if(!OB->newMons->Transport)RotUnit(OB,LastDirection&255,1,Prio);
						OB->NewMonsterSmartSendTo(cx,cy,(px[i]-CenterX)>>4,(py[i]-CenterY)>>4,Prio,1);
						break;
					case 2:
						OB->NewMonsterSmartSendTo(cx,cy,(px[i]-CenterX)>>4,(py[i]-CenterY)>>4,Prio,2);
						if(!OB->newMons->Transport)RotUnit(OB,LastDirection&255,2,Prio);
						break;
					};
						
				}else{
					addrand(cx);
					addrand(cy);
					addrand(px[i]);
					addrand(py[i]);
					addrand(99);					
					OB->NewMonsterSmartSendTo(cx,cy,(px[i]-CenterX)>>4,(py[i]-CenterY)>>4,Prio,OrdType);
				};
			}else{
				addrand(cx);
				addrand(cy);
				addrand(px[i]);
				addrand(py[i]);
				addrand(98);					
				OB->NewMonsterSmartSendTo(cx,cy,(px[i]-CenterX)>>4,(py[i]-CenterY)>>4,Prio,OrdType);
			};
        };
    };
};
void PositionOrder::Patrol(){
    word MID;
	int cx=CenterX>>4;
	int cy=CenterY>>4;
    for(int i=0;i<NUnits;i++){
        MID=Ids[i];
        OneObject* OB=Group[MID];
        if(OB&&!CmdDone[MID]){
			OB->Patrol(OB->RealX>>4,OB->RealY>>4,cx+((px[i]-CenterX)>>4),cy+((py[i]-CenterY)>>4),0);
        };
    };
};
extern City CITY[8];
extern PositionOrder PORD;
void SetStandState(Brigade* BR,byte State){
	if(BR->Strelki)return;
	int N=BR->NMemb;
	word* Memb=BR->Memb;
	word* MembSN=BR->MembSN;
	int addD=0;
	int addS=0;
	if(State){
		for(int i=0;i<N;i++){
			word MID=Memb[i];
			if(MID!=0xFFFF){
				addrand(MID);
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==MembSN[i]){
					if(OB->newMons->GetAnimation(anm_PMotionL)->Enabled){
						OB->GroundState=OB->ActivityState==2?1:(OB->ActivityState==1?0:State);
						addrand(OB->GroundState+1);
						if(!OB->LocalOrder){
							OB->NewState=1;
						};
					};
				};
			};
		};
	}else{
		for(int i=0;i<N;i++){
			word MID=Memb[i];
			if(MID!=0xFFFF){
				addrand(MID);
				OneObject* OB=Group[MID];
				if(OB&&OB->Serial==MembSN[i]){
					OB->GroundState=OB->ActivityState==2?1:0;
					addrand(OB->GroundState+1);
				};
			};
		};
	};
};
extern int tmtmt;
void CancelStandGroundAnyway(Brigade* BR);
bool GetBrCenter(Brigade* BR,int* x,int* y);
extern bool OneDirection;
void ExGroupSendSelectedTo(byte NI,word* SMon,word* MSN,int Nsel,int x,int y,int DF,int DR,byte Prio,byte OrdType);
void GroupSendSelectedTo(byte NI,int x,int y,byte Prio,byte OrdType){
    int Nsel=NSL[NI];
    word* SMon=Selm[NI];
	word* MSN=SerN[NI];
	word* tempIDS=znew(word,Nsel);
	word* tempSNS=znew(word,Nsel);
	int NR=0;
	addrand(Nsel);
	for(int p=0;p<Nsel;p++){
		word ID=SMon[p];
		if(ID!=0xFFFF){
			addrand(ID);
			if(!CmdDone[ID]){
				addrand(ID);
				tempIDS[NR]=SMon[p];
				tempSNS[NR]=MSN[p];
				NR++;
			}
		}
	}
	ExGroupSendSelectedTo(NI,tempIDS,tempSNS,NR,x,y,0,0,Prio,OrdType);
	free(tempIDS);
	free(tempSNS);
};
void ExGroupSendSelectedTo(byte NI,word* SMon,word* MSN,int Nsel,int x,int y,int DF,int DR,byte Prio,byte OrdType){
	//checking for pushka
	addrand(x);
	addrand(y);
	addrand(NI);
	addrand(DF);
	addrand(DR);
	addrand(Prio);
	addrand(OrdType);
	word* SSEL=znew(word,Nsel);
	bool AllPus=true;
	int xc=0;
	int yc=0;
	int Np=0;
	int Nu=0;
	byte RealNI=GSets.CGame.cgi_NatRefTBL[NI];
	byte LastNI=RealNI;
	for(int i=0;i<Nsel;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MSN[i]){
				if(!OB->InArmy){
					byte Usage=OB->newMons->Usage;
					if(Usage!=PushkaID && !OB->newMons->Artpodgotovka)AllPus=false;
					SSEL[Np]=MID;
					Np++;
					Nu++;
					xc+=OB->RealX;
					yc+=OB->RealY;
				}else{
					xc+=OB->RealX;
					yc+=OB->RealY;
					Nu++;					
				}
			}
		}
	}	
	BrigadesList BLIST;
	BLIST.CreateFromSelected(NI,x>>4,y>>4,Nsel,SMon,MSN);
	addrand(NI);
	int rdx=16;
	int rdy=16;
	if(LastDirection<256){
		rdx=int(TCos[byte(LastDirection)])<<4;
		rdy=int(TSin[byte(LastDirection)])<<4;
	}else{
		if(Nu)
		{
			xc/=Nu;
			yc/=Nu;
			rdx=x-xc;
			rdy=y-yc;
			LastDirection=1024+(BLIST.BrPosArr.GetAmount()?BLIST.AverageDir:GetDir(rdx,rdy));
			//char DRR=LastDirection;
			//if(abs(DRR-AverageDir)<16)LastDirection=AverageDir;
		};
	};
	if(DF||DR){
		float fdx=float(rdx);
		float fdy=float(rdy);

		int DD=int(sqrt(fdx*fdx+fdy*fdy));
		int DX=(rdx<<8)/DD;
		int DY=(rdy<<8)/DD;
		x+=((DF*DX)>>4)-((DR*DY)>>4);
		y+=((DR*DX)>>4)+((DF*DY)>>4);
	};	
	addrand(BLIST.BrPosArr.GetAmount());
	if(BLIST.BrPosArr.GetAmount()){
		if(BLIST.AttDir!=512)
			LastDirection=BLIST.AttDir;
		addrand(x>>4);
		addrand(y>>4);
		addrand(LastDirection);
		BLIST.Transform(x>>4,y>>4,LastDirection);
		BLIST.SendToPositions(Prio,OrdType);
	}

	if(Np){
		//PORD.CreateRotatedPositions(x,y,SSEL,Np,rdx,rdy);
		PORD.CreateRotatedPositionsWithColumns(x,y,SSEL,Np,rdx,rdy);
	}else{
		free(SSEL);
		return;
	}
	if(AllPus){
		if(Np)PORD.CreateRotatedPositions2(x,y,SSEL,Np,rdx,rdy);			
	}else{ 
		if(Np){
			//PORD.CreateRotatedPositions(x,y,SSEL,Np,rdx,rdy);
		}else{
			free(SSEL);
			return;
		}
	};
	if(Np){
		PORD.CenterX=x;
		PORD.CenterY=y;
		PORD.SendToPosition(Prio,OrdType);
	};
	free(SSEL);
};

void GroupAttackSelectedBrigadesTo(byte NI,int x,int y,byte Prio,byte OrdType){
    int Nsel=NSL[NI];
    word* SMon=Selm[NI];
	word* MSN=SerN[NI];
	//checking for pushka
	word* SSEL=znew(word,NSL[NI]);
	bool AllPus=true;
	int xc=0;
	int yc=0;
	int Np=0;
	int BRIDX[32];
	int BRX[32];
	int BRY[32];
	int NBRG=0;
	for(int i=0;i<Nsel;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MSN[i]){
				if(!OB->InArmy){
					byte Usage=OB->newMons->Usage;
					if(Usage!=SupMortID&&Usage!=PushkaID&&Usage!=MortiraID)AllPus=false;
					SSEL[Np]=MID;
					Np++;
					xc+=OB->RealX;
					yc+=OB->RealY;
					OB->StandGround=false;
				}else{
					if(OB->NNUM==NI&&OB->BrigadeID!=0xFFFF&&NBRG<32){
						Brigade* BR=CITY[NI].Brigs+OB->BrigadeID;
						if(BR->ID<8192){
							BRIDX[NBRG]=BR->ID;
							NBRG++;
							BR->ID+=8192;
						};
					};
				};
			};
		};
	};
	int BCX=0;
	int BCY=0;
	for(int i=0;i<NBRG;i++){
		Brigade* BR=CITY[NI].Brigs+BRIDX[i];
		BR->ID-=8192;
		BR->GetCenter(BRX+i,BRY+i);
		BCX+=BRX[i];
		BCY+=BRY[i];
	};
	if(NBRG){
		BCX/=NBRG;
		BCY/=NBRG;
		for(int i=0;i<NBRG;i++){
			int dx=BRX[i]-BCX;
			int dy=BRY[i]-BCY;
			int Nr=Norma(dx,dy);
			if(Nr>1000){
				dx=(dx*256)/Nr;
				dy=(dy*256)/Nr;
			};
			BRX[i]=(x>>4)+dx;
			BRY[i]=(y>>4)+dy;
			Brigade* BR=CITY[NI].Brigs+BRIDX[i];
			int Dir=BR->Direction;
			if(NBRG==1)Dir=LastDirection;
			if(!BR->Strelki){
				BR->HumanGlobalSendTo(BRX[i],BRY[i],Dir,126+16,OrdType);
				SetStandState(BR,1);
				BR->AttEnm=true;
			}else{
				BR->LastOrderTime=REALTIME;//tmtmt;
				BR->ClearBOrders();
			};
			CancelStandGroundAnyway(BR);
		};
	};
	free(SSEL);
};
void GroupPatrolSelected(byte NI,int x,int y,byte Prio){
    int Nsel=NSL[NI];
    word* SMon=Selm[NI];
	word* SNS=SerN[NI];
	//checking for pushka
	bool AllPus=true;
	int xc=0;
	int yc=0;
	int Np=0;
	for(int i=0;i<Nsel;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==SNS[i]){
				if(OB->newMons->Usage!=SupMortID&&OB->newMons->Usage!=PushkaID&&OB->newMons->Usage!=MortiraID)AllPus=false;
				Np++;
				xc+=OB->RealX;
				yc+=OB->RealY;
			};
		};
	};
	int rdx=16;
	int rdy=16;
	if(LastDirection<256){
		rdx=int(TCos[byte(LastDirection)])<<4;
		rdy=int(TSin[byte(LastDirection)])<<4;
	}else{
		if(Np){
			xc/=Np;
			yc/=Np;
			rdx=xc-x;
			rdy=yc-y;
			LastDirection=GetDir(-rdx,-rdy);
		};
	};
	if(AllPus&&Np){
		PORD.CreateLinearPositions(x,y,SMon,Nsel,rdx,rdy);
	}else{ //PORD.CreatePositions(x,y,SMon,Nsel);
		if(Np){
			PORD.CreateRotatedPositions(x,y,SMon,Nsel,rdx,rdy);
		}else return;
	};
	PORD.CenterX=x;
	PORD.CenterY=y;
    PORD.Patrol();
};
word PositionOrder::CreateOrdPos(int x,int y,byte dir,int NIDS,word* IDS,OrderDescription* ODS){
	int dx=TCos[dir];
	int dy=TSin[dir];
    Create(IDS,NIDS);
    //CheckSize(NIDS);
    UNISORT.CreateByLine(Ids,NUnits,dx>>4,dy>>4);
    UNISORT.Sort();
    int NU=UNISORT.NUids;
    CheckSize(NU);
    UNISORT.Copy(Ids);
    int Px=0;
    int Lx1;
	int Ly=ODS->NLines;
    for(int iy=0;iy<Ly;iy++){
		int Nx=ODS->LineNU[iy];
		if(Nx){
			Lx1=NU-Px;
			if(Lx1>Nx)Lx1=Nx;
			UNISORT.CreateByLine(Ids+Px,Lx1,-dy>>4,dx>>4);
			UNISORT.Sort();
            UNISORT.Copy(Ids+Px);
            Px+=Lx1;
		};
    };
    //Sorting is finished now
    //Getting of the maximal size of Unit
    int maxR=FORMDIST;
    word* IDE=Ids;
	int BScale=100;
	int fsc=0;
	for(int i=0;i<NIDS;i++){
		word MID=Ids[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				BScale=OB->newMons->FormationDistanceScale;
				if(OB->BrigadeID!=0xFFFF){
					Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
					fsc=BR->ScaleFactor;
				}
				break;
			}
		}
	}
	if(fsc)BScale=fsc*BScale/100;	
//    word MID;
//    int rr;
//    OneObject* OB;
	/*
    for(iy=0;iy<NU;iy++){
        MID=IDE[iy];
        OB=Group[MID];
        if(OB&&OB->NewMonst){
            rr=OB->newMons->Radius2;
            if(rr>maxR){
                maxR=rr;
            };
        };
    };
	*/
    //Creating the position
	maxR<<=2;
    int Maxx=(msx<<9)-768;
    int Maxy=(msy<<9)-768;
    int pos=0;
	int NR=Norma(dx,dy);
	int vx=(dx*maxR)/NR;
	int vy=(dy*maxR)/NR;
	int Dy=ODS->YShift;	

    for(int iy=0;iy<Ly;iy++){
		int Nx=ODS->LineNU[iy];
		short* shx=ODS->Lines[iy];
        for(int ix=0;ix<Nx;ix++){           
		   int sx=shx[ix];
           px[pos]=x-OScale(((sx*vy+(Dy-iy-iy)*vx)*BScale)/800);
           py[pos]=y+OScale(((sx*vx+(iy+iy-Dy)*vy)*BScale)/800);			
		   pos++;
        };
    };
	//return NIDS;
	return NU;
};
word PositionOrder::CreateSimpleOrdPos(int x,int y,byte dir,int NIDS,word* IDS,OrderDescription* ODS){
	int dx=TCos[dir];
	int dy=TSin[dir];
#ifdef COSSACKS2
	if(IDS){
		bool ALLFF=1;
		for(int i=0;i<NIDS;i++){	
			if(IDS[i]!=0xFFFF)ALLFF=0;;
		};
		if(ALLFF)return CreateOrdPos(x,y,dir,NIDS,IDS,ODS);
		Create(IDS,NIDS);
	}else{
		CheckSize(NIDS);
	};
#else //COSSACKS2
	if(IDS){
		for(int i=0;i<NIDS;i++){	
			if(IDS[i]==0xFFFF)return CreateOrdPos(x,y,dir,NIDS,IDS,ODS);
		};
		Create(IDS,NIDS);
	}else{
		CheckSize(NIDS);
	};
#endif COSSACKS2
    //Getting of the maximal size of Unit
    int maxR= FORMDIST;
    //word* IDE=Ids;
    //word MID;
	int NU=NIDS;
    //int rr;
    //OneObject* OB;
	/*
    for(int iy=0;iy<NU;iy++){
        MID=IDE[iy];
        OB=Group[MID];
        if(OB&&OB->NewMonst){
            rr=OB->newMons->Radius2;
            if(rr>maxR){
                maxR=rr;
            };
        };
    };
    //Creating the position
	maxR<<=2;
	*/
    maxR<<=2;
    int Maxx=(msx<<9)-768;
    int Maxy=(msy<<9)-768;
    int pos=0;
	int NR=Norma(dx,dy);
	int vx=(dx*maxR)/NR;
	int vy=(dy*maxR)/NR;
	int Dy=ODS->YShift;
	int Ly=ODS->NLines;
	int BScale=100;
	int fsc=0;
	for(int i=0;i<NIDS;i++){
		word MID=Ids[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB){
				BScale=OB->newMons->FormationDistanceScale;
				if(OB->BrigadeID!=0xFFFF){
					Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;
					fsc=BR->ScaleFactor;
				}
				break;
			}
		}
	}
	if(fsc)BScale=fsc*BScale/100;
    for(int iy=0;iy<Ly;iy++){
		int Nx=ODS->LineNU[iy];
		short* shx=ODS->Lines[iy];
        for(int ix=0;ix<Nx;ix++){
            if(pos<NU){
				int sx=shx[ix];
                px[pos]=x-OScale(((sx*vy+(Dy-iy-iy)*vx)*BScale)/800);
                py[pos]=y+OScale(((sx*vx+(iy+iy-Dy)*vy)*BScale)/800);
            };
            pos++;
        };
    };
	return NU;
};
extern int GetTopDistance(int xa,int ya,int xb,int yb,int LT);
extern word GetBrigLockType(Brigade* BR);
void BrigadesList::CreateFromSelected(byte NI,int dxx, int dyy,int Nsel,word* SMon,word* MSN){
	AttDir=512;
	BrPosArr.Clear();
	int ADX=0;
	int ADY=0;
	xc=0;
	yc=0;
	for(int i=0;i<Nsel;i++){
		word MID=SMon[i];
		if(MID!=0xFFFF){
			OneObject* OB=Group[MID];
			if(OB&&OB->Serial==MSN[i]){				
				if(OB->InArmy){
					if(OB->BrigadeID!=0xFFFF){
						Brigade* BR=CITY[OB->NNUM].Brigs+OB->BrigadeID;						
						if(BR->ID<8192&&BR->NMemb){
							OneBrigadePosition OBP;
							OBP.BrigadeID=BR->ID;
							OBP.NI=BR->CT->NI;
							GetBrCenter(BR,&OBP.xc,&OBP.yc);
							BrPosArr.Add(OBP);
							xc+=OBP.xc;
							yc+=OBP.yc;
							if(BR->WarType){
								int vx=TCos[byte(BR->Direction)]>>2;
								int vy=TSin[byte(BR->Direction)]>>2;
								ADX+=vx;
								ADY+=vy;
								SumLength+=ElementaryOrders[BR->WarType-1].Width/2+50;
							};
							BR->ID+=8192;
						};
					};
				};
			};
		};
	};			
	AverageDir=(GetDir(ADX,ADY)+8)&0xF0;
	if(BrPosArr.GetAmount()){
		xc/=BrPosArr.GetAmount();
		yc/=BrPosArr.GetAmount();

		int xx=(dxx>>7)+1;
		int yy=(dyy>>7)+1;
		if(xx>=0&&yy>=0&&xx<VAL_MAXCX&&yy<VAL_MAXCX)
		{
			int cell=xx+(yy<<VAL_SHFCX);
			int NMon=MCount[cell];
			if(NMon)
			{
				int ofs1=cell<<SHFCELL;
				word MID;
				for(int i=0;i<NMon;i++)
				{
					MID=GetNMSL(ofs1+i);
					if(MID!=0xFFFF)
					{
						OneObject* OB=Group[MID];
						if(OB&&(!OB->Sdoxlo)&&(!(NATIONS[NI].NMask&OB->NMask)))
						{
							if(Norma((OB->RealX>>4)-dxx,(OB->RealY>>4)-dyy)<40)
							{
								byte drr=(GetDir(dxx-xc,dyy-yc)+8)&0xF0;
								AttDir=drr;
								break;
							}
						}
					}
				}
			}
		}
	}
	for(int i=0;i<BrPosArr.GetAmount();i++){
		OneBrigadePosition* OBP=&(BrPosArr[i]);
		Brigade* BR=CITY[OBP->NI].Brigs+OBP->BrigadeID;
		BR->ID-=8192;
		OBP->xcb=OBP->xc;
		OBP->ycb=OBP->yc;
		OBP->Dirb=OBP->Dir;
	}
}

void BrigadesList::CreateFromSelected(byte NI,int dxx, int dyy)
{	
	int Nsel=NSL[NI];
	word* SMon=Selm[NI];
	word* MSN=SerN[NI];
	CreateFromSelected(NI,dxx,dyy,Nsel,SMon,MSN);
}
word GetBrigLockTypeIntf(Brigade* BR);
void BrigadesList::Transform(int x,int y,byte DestDir){
	DestDir=((DestDir+8)&0xF0);
	
	//const int NearestRadiusToPoint=2000;
	int nr=9999999;
	int OBPIndex=-1;
	for(int i=0;i<BrPosArr.GetAmount();i++)
	{
		OneBrigadePosition* OBP=&(BrPosArr[i]);
		Brigade* BR=CITY[OBP->NI].Brigs+OBP->BrigadeID;
		//OBP->TopDest2DestP=GetTopDistance(OBP->xc>>6,OBP->yc>>6,x>>6,y>>6,GetBrigLockTypeIntf(BR))<<6;
		OBP->Dest2DestP=Norma(OBP->xc-x,OBP->yc-y);
		if(OBP->Dest2DestP<nr)
		{
			//if(((OBP->Dest2DestP*100)/(OBP->TopDest2DestP+1))>60)
			{
				nr=OBP->Dest2DestP;
				OBPIndex=i;
			}
		}
 		OBP->GroupFlag=0;
	}
	/*
	if(OBPIndex!=-1)
	{
		OneBrigadePosition* OBP=&(BrPosArr[OBPIndex]);
		Brigade* BR=CITY[OBP->NI].Brigs+OBP->BrigadeID;
		if(BR->OnlyThisBrigadeToKill!=0xFFFF)
		{
			Brigade* TBR=CITY[BR->NIOnlyThisBrigadeToKill].Brigs+BR->OnlyThisBrigadeToKill;
			if(TBR&&TBR->WarType)
			{
				int ex=0;
				int ey=0;
				if(GetBrCenter(TBR,&ex,&ey))
				{
					DestDir=GetDir(ex-BrPosArr[OBPIndex].xc,ey-BrPosArr[OBPIndex].yc);
					DestDir=((DestDir+8)&0xF0);
				}
			}
		}
	}
	*/
	MarkNearest(OBPIndex);
	CreateFromMarked();

	for(int i=0;i<BrPosArr.GetAmount();i++)
	{
		OneBrigadePosition* OBP=&(BrPosArr[i]);
		if(OBP->GroupFlag==1)
		{
			Brigade* BR=CITY[OBP->NI].Brigs+OBP->BrigadeID;
			int dx=OBP->xc-xc;
			int dy=OBP->yc-yc;
			char Dir=BR->Direction-AverageDir+DestDir;
			byte ddir=DestDir-AverageDir;
			int SIN=TSin[byte(ddir)];
			int COS=TCos[byte(ddir)];
			OBP->xc=x+((dx*COS-dy*SIN)>>8);
			OBP->yc=y+((dx*SIN+dy*COS)>>8);		
			OBP->Dir=((Dir+8)&0xF0);
		}
	}
	CreateFromNonMarked(DestDir);
	ResortPos();
}
void DrawPlaneLine2(int x0,int y0,int x1,int y1,DWORD c);
void BrigadesList::ShowPositions(){
	for(int i=0;i<BrPosArr.GetAmount();i++){
		OneBrigadePosition* OBP=&(BrPosArr[i]);
		Brigade* BR=CITY[OBP->NI].Brigs+OBP->BrigadeID;
		int N=PORD.CreateSimpleOrdPos(OBP->xc<<4,OBP->yc<<4,OBP->Dir,BR->NMemb-NBPERSONAL,BR->Memb+NBPERSONAL,ElementaryOrders+BR->WarType-1);
		byte cl=clrYello;
		if(BR->WarType){
			OrderDescription* ODS=ElementaryOrders+BR->WarType-1;
			if(ODS->OrdUsage!=2){//dont show positions preview for KARE
				for(int j=0;j<N;j++){
					int xx=PORD.px[j]>>4;
					int yy=PORD.py[j]>>4;
					void DrawPlaneLine(int x0,int y0,int x1,int y1,byte c);
					int D=2<<(5-Shifter);
					DrawPlaneLine2(xx,yy-D,xx,yy+D,0xFF00FFFF);
					DrawPlaneLine2(xx-D,yy,xx+D,yy,0xFF00FFFF);
				}
			}
		}
	}
}
extern bool NOPAUSE;
void BrigadesList::SendToPositions(byte Prio,int OrdType){
	for(int i=0;i<BrPosArr.GetAmount();i++){
		OneBrigadePosition* OBP=&(BrPosArr[i]);
		Brigade* BR=CITY[OBP->NI].Brigs+OBP->BrigadeID;
		if(Prio&127){
			if(BR->LastOrderTime!=REALTIME){
				SetStandState(BR,0);
				BR->AttEnm=0;
			};
			addrand(67);
			if(!NOPAUSE)BR->LastOrderTime=REALTIME-1;
			BR->HumanGlobalSendTo(OBP->xc,OBP->yc,OBP->Dir,126+16,OrdType);
		}else{
			if(!BR->Strelki){
				BR->LastOrderTime=0;
				BR->AttEnm=true;
				addrand(68);
				if(!NOPAUSE)BR->LastOrderTime=REALTIME-1;
				BR->HumanGlobalSendTo(OBP->xc,OBP->yc,OBP->Dir,128,OrdType);
				SetStandState(BR,1);				
			}else{
				//BR->LastOrderTime=tmtmt;
				BR->ClearBOrders();
				addrand(69);
				if(!NOPAUSE)BR->LastOrderTime=REALTIME-1;
				BR->HumanGlobalSendTo(OBP->xc,OBP->yc,OBP->Dir,128,OrdType);
				BR->LastOrderTime=REALTIME;//tmtmt;
			};
		};
		bool KARE=0;
		if(BR->WarType){
			KARE=ElementaryOrders[BR->WarType-1].OrdUsage==2;
		};
		addrand(70);
		if(!KARE)CancelStandGroundAnyway(BR);	
		if((!BR->AttEnm)&&Prio&127){
			void MakeStandGroundTemp(Brigade* BR);
			BR->LastOrderTime=0;
			if(!(BR->InStandGround&&KARE))MakeStandGroundTemp(BR);
		}	
		addrand(71);
	}

}
void BrigadesList::MarkNearest(int OBPIndex)
{
	if(OBPIndex>-1)
	{
		const int NearestRadiusToBrigade=1701;

		BrPosArr[OBPIndex].GroupFlag=1;
		for(int i=0;i<BrPosArr.GetAmount();i++)
		{
			if(i!=OBPIndex&&BrPosArr[i].GroupFlag==0)
			{
				int ds=Norma(BrPosArr[OBPIndex].xcb-BrPosArr[i].xcb,BrPosArr[OBPIndex].ycb-BrPosArr[i].ycb);
				if(ds<NearestRadiusToBrigade)
				{
					MarkNearest(i);
				}
			}
		}
	}
}
void BrigadesList::CreateFromMarked()
{
	int ADX=0;
	int ADY=0;
	xc=0;
	yc=0;
	int n=0;
	for(int i=0;i<BrPosArr.GetAmount();i++)
	{
		if(BrPosArr[i].GroupFlag==1)
		{
			Brigade* BR=CITY[BrPosArr[i].NI].Brigs+BrPosArr[i].BrigadeID;
			n++;
			xc+=BrPosArr[i].xc;
			yc+=BrPosArr[i].yc;
			if(BR->WarType)
			{
				int vx=TCos[byte(BR->Direction)]>>2;
				int vy=TSin[byte(BR->Direction)]>>2;
				ADX+=vx;
				ADY+=vy;
			};
		}
	}
	if(n)
	{
		AverageDir=(GetDir(ADX,ADY)+8)&0xF0;
		xc/=n;
		yc/=n;
	}
}
void BrigadesList::CreateFromNonMarked( byte NDir)
{
	for(int i=0;i<BrPosArr.GetAmount();i++)
	{
		if(BrPosArr[i].GroupFlag==0)
		{
			int MinX=-1, MinY;
			int MinDst=999999;
			byte MinDir=NDir;
			for(int j=0;j<BrPosArr.GetAmount();j++)
			{
				if(BrPosArr[j].GroupFlag==1)
				{
					int nx,ny;
					int Ndr=-1;
					int ndd=GetNearestPointToBr(i,j,nx,ny,Ndr);
					if(ndd>-1&&ndd<MinDst)
					{
						MinDst=ndd;
						MinX=nx;
						MinY=ny;
						if(Ndr!=-1)
							MinDir=(byte)Ndr;
					}
				}
			}
			if(MinX!=-1)
			{
				BrPosArr[i].xc=MinX;
				BrPosArr[i].yc=MinY;
				BrPosArr[i].GroupFlag=1;
				BrPosArr[i].Dir=MinDir;
			}
		}
	}
}
int BrigadesList::GetNearestPointToBr(int OBPFrom, int OBPTo, int& nx, int& ny, int& NDir)
{
	int rez=-1;

	Brigade* BRFrom=CITY[BrPosArr[OBPFrom].NI].Brigs+BrPosArr[OBPFrom].BrigadeID;
	Brigade* BRTo=CITY[BrPosArr[OBPTo].NI].Brigs+BrPosArr[OBPTo].BrigadeID;
	int WFrom = ElementaryOrders[BRFrom->WarType-1].Width/2+50;
	int WTo = ElementaryOrders[BRTo->WarType-1].Width/2+50;
	
	int Sh= WFrom+WTo;
	int Xr=BrPosArr[OBPTo].xc-((TSin[BrPosArr[OBPTo].Dir]*Sh)>>8);
	int Yr=BrPosArr[OBPTo].yc+((TCos[BrPosArr[OBPTo].Dir]*Sh)>>8);
	int Xl=BrPosArr[OBPTo].xc+((TSin[BrPosArr[OBPTo].Dir]*Sh)>>8);
	int Yl=BrPosArr[OBPTo].yc-((TCos[BrPosArr[OBPTo].Dir]*Sh)>>8); 
 	int H = 350;
	byte dr=BrPosArr[OBPTo].Dir+128;
	int Xb=BrPosArr[OBPTo].xc+((TCos[dr]*H)>>8);
	int Yb=BrPosArr[OBPTo].yc+((TSin[dr]*H)>>8);

	int Dr=Norma(BrPosArr[OBPFrom].xcb-Xr,BrPosArr[OBPFrom].ycb-Yr);
	int Dl=Norma(BrPosArr[OBPFrom].xcb-Xl,BrPosArr[OBPFrom].ycb-Yl);
	int Db=Norma(BrPosArr[OBPFrom].xcb-Xb,BrPosArr[OBPFrom].ycb-Yb);
	
	if(Dr<Dl&&Dr<Db)
	{
		nx=Xr;
		ny=Yr;
		rez=Dr;
	}
	if(Dl<Dr&&Dl<Db)
	{
		nx=Xl;
		ny=Yl;
		rez=Dl;
	}
	if(Db<Dr&&Db<Dl)
	{
		nx=Xb;
		ny=Yb;
		rez=Db;
		NDir=BrPosArr[OBPTo].Dir;
	}
	return rez;
}
void BrigadesList::ResortPos()
{
	int n=BrPosArr.GetAmount();
	if(n>1)
	{
		bool swap;
		do 
		{			
			swap=false;
			for(int i=0;i<n&&(!swap);i++)
			{
				Brigade* Bi=CITY[BrPosArr[i].NI].Brigs+BrPosArr[i].BrigadeID;
				for(int j=0;j<n&&(!swap);j++)
				{
					if(i!=j)
					{
						Brigade* Bj=CITY[BrPosArr[j].NI].Brigs+BrPosArr[j].BrigadeID;
						if(Bi->WarType==Bj->WarType)
						{
							int d1=Norma(BrPosArr[i].xcb-BrPosArr[j].xc,BrPosArr[i].ycb-BrPosArr[j].yc);
							int d2=Norma(BrPosArr[j].xcb-BrPosArr[i].xc,BrPosArr[j].ycb-BrPosArr[i].yc);
							if((d1+d2)<(BrPosArr[i].Dest2DestP+BrPosArr[j].Dest2DestP))
							{
								int t=BrPosArr[i].BrigadeID;
								BrPosArr[i].BrigadeID=BrPosArr[j].BrigadeID;BrPosArr[j].BrigadeID=t;
								t=BrPosArr[i].xcb;
								BrPosArr[i].xcb=BrPosArr[j].xcb;BrPosArr[j].xcb=t;
								t=BrPosArr[i].ycb;
								BrPosArr[i].ycb=BrPosArr[j].ycb;BrPosArr[j].ycb=t;
								BrPosArr[i].Dest2DestP=d1;
								BrPosArr[j].Dest2DestP=d2;
								swap=true;
							}
						}
					}
				}
			}
		}
		while(swap);
	}
}


