#include "stdafx.h"
_inline DWORD MixDWORD(DWORD C1,DWORD C2,int W1,int W2){
    DWORD R=0;
    if(W1<0)W1=0;
    if(W2<0)W2=0;
    if(W1>255)W1=255;
    if(W2>255)W2=255;
    int DW=(W1+W2)/2;
    for(int i=0;i<4;i++){
        //R<<=8;
        int CH1=C1&255;
        int CH2=C2&255;
        C1>>=8;
        C2>>=8;
        int CC=(CH1*W1+CH2*W2+DW)>>8;
        if(CC>255)CC=255;
        R|=(CC<<(i<<3));
    }
    return R;
}
DWORD ModDWORD2X(DWORD C1,DWORD C2){
    DWORD R=0;
    for(int i=0;i<4;i++){
        int CH1=C1&255;
        int CH2=C2&255;
        C1>>=8;
        C2>>=8;
        int CC=(CH1*CH2*2)>>8;
        if(CC>255)CC=255;
        R|=(CC<<(i<<3));
    }
    return R;
}
class fOperator;
class FlashObjectOut{
public:
    ParentFrame*    pf;
    float           PivotX;
    float           PivotY;
    FlashObjectOut(){
        pf=NULL;
        PivotX=0;
        PivotY=0;
    }
};
class FlashObject:public BaseClass{
public:
    ClassArray<fOperator>   Operators;
    ClassArray<FlashObject> SubObjects;
    virtual FlashObjectOut Create(ParentFrame* Dest,float Time,float PrevTime){FlashObjectOut fo;return fo;}
    SAVE(FlashObject);
    ENDSAVE;
};
class fSound:public FlashObject{
public:
    _str oggFileName;
    float StartPlayTime;
    SAVE(fSound){
        REG_BASE(FlashObject);
        REG_FILEPATH(oggFileName,".ogg");
        REG_MEMBER(_float,StartPlayTime);
        REG_AUTO(SubObjects);
    }ENDSAVE;
    virtual FlashObjectOut Create(ParentFrame* Dest,float Time,float PrevTime){
        FlashObjectOut fo;
        if(Time>StartPlayTime && PrevTime<=StartPlayTime){
            GameInterface::ogg_Play(oggFileName.pchar());
        }
        return fo;
    }
};
class fPicture:public FlashObject{
    _picfile FileName;
    int      x;
    int      y;
    bool     CentralAlignmentX;
    bool     CentralAlignmentY;
    float    StartTime;
    float    Duration;
    float    FadeOnTime;
    float    FadeOffTime;
    SAVE(fPicture){
        REG_BASE(FlashObject);
        REG_AUTO(FileName);
        REG_MEMBER(_int,x);
        REG_MEMBER(_int,y);
        REG_MEMBER(_bool,CentralAlignmentX);
        REG_MEMBER(_bool,CentralAlignmentY);
        REG_MEMBER(_float,StartTime);
        REG_MEMBER(_float,Duration);
        REG_MEMBER(_float,FadeOnTime);
        REG_MEMBER(_float,FadeOffTime);
        REG_AUTO(Operators);
        REG_AUTO(SubObjects);
    }ENDSAVE;
    virtual FlashObjectOut Create(ParentFrame* Dest,float Time,float PrevTime){
        FlashObjectOut fo;
        if(Time>StartTime && Time<StartTime+Duration){
            BitPicture* bp=new BitPicture;
            bp->x=x;
            bp->y=y;        
            bp->FileName=FileName;
            bp->SetFileName(FileName.pchar(),false);
            bp->EnableTransform=true;
            bp->DeepColor=true;
            float f1=FadeOnTime>0?(Time-StartTime)/FadeOnTime:1;
            float f2=FadeOffTime>0?(Duration-Time+StartTime)/FadeOffTime:1;
            f1=min(f1,f2);
            if(f1>1)f1=1;
            bp->Diffuse=0xFFFFFF+(DWORD(f1*255)<<24);
            Dest->DSS.Add(bp);
            fo.pf=bp;
            if(CentralAlignmentX){
                fo.PivotX=bp->Pic.GetLx()/2.0f;
                bp->x-=fo.PivotX;
            }
            if(CentralAlignmentY){
                fo.PivotY=bp->Pic.GetLy()/2.0f;
                bp->y-=fo.PivotY;
            }
        }
        return fo;
    }
};
class fText:public FlashObject{
public:
    RLCFont* Font;
    _str    Message;
    int     MaxLength;
    int     x;
    int     y;
    bool    CentralAlignmentX;
    bool    CentralAlignmentY;
    float   StartTime;
    float   Duration;
    float   FadeOnTime;
    float   FadeOffTime;
    SAVE(fText){
        REG_BASE(FlashObject);
        REG_MEMBER(_font,Font);
        REG_AUTO(Message);
        REG_MEMBER(_int,MaxLength);
        REG_MEMBER(_int,x);
        REG_MEMBER(_int,y);
        REG_MEMBER(_bool,CentralAlignmentX);
        REG_MEMBER(_bool,CentralAlignmentY);
        REG_MEMBER(_float,StartTime);
        REG_MEMBER(_float,Duration);
        REG_MEMBER(_float,FadeOnTime);
        REG_MEMBER(_float,FadeOffTime);
        REG_AUTO(Operators);
        REG_AUTO(SubObjects);
    }ENDSAVE;
    virtual FlashObjectOut Create(ParentFrame* Dest,float Time,float PrevTime){
        FlashObjectOut fo;
        if(Time>StartTime && Time<StartTime+Duration){
            TextButton* TB=new TextButton;
            TB->x=x;
            TB->y=y;
            TB->SetMessage(Message.pchar());
            TB->ActiveFont=Font;
            TB->PassiveFont=Font;
            TB->DisabledFont=Font;
            TB->Align=CentralAlignmentX?1:0;
            TB->DeepColor=true;
            TB->EnableTransform=true;
            float f1=FadeOnTime>0?(Time-StartTime)/FadeOnTime:1;
            float f2=FadeOffTime>0?(Duration-Time+StartTime)/FadeOffTime:1;
            f1=min(f1,f2);
            if(f1>1)f1=1;
            TB->Diffuse=0xFFFFFF+(DWORD(f1*255)<<24);
            Dest->DSS.Add(TB);
            if(CentralAlignmentX){
                fo.PivotX=TB->GetWidth()/2.0f;
                TB->x-=fo.PivotX;
            }
            if(CentralAlignmentY){
                fo.PivotY=TB->GetHeight()/2.0f;
                TB->y-=fo.PivotY;
            }
            fo.pf=TB;
        }
        return fo;
    }
};
class fOperator:public BaseClass{
public:
    SAVE(fOperator);
    ENDSAVE;
    virtual void operate(ParentFrame* op,float Time){};
};
class opScale:public fOperator{
public:
    //int PivotX;
    //int PivotY;
    float StartScale;
    float FinalScale;
    float StartTime;
    float Duration;
    SAVE(opScale){
        REG_BASE(fOperator);
        //REG_MEMBER(_int,PivotX);
        //REG_MEMBER(_int,PivotY);
        REG_MEMBER(_float,StartScale);
        REG_MEMBER(_float,FinalScale);
        REG_MEMBER(_float,StartTime);
        REG_MEMBER(_float,Duration);
    }ENDSAVE;
    virtual void operate(ParentFrame* op,float Time){
        float sc=StartScale;
        if(Time>StartTime){
            Time-=StartTime;
            if(Time<Duration){
                sc=StartScale+(FinalScale-StartScale)*Time/Duration;                
            }else sc=FinalScale;            
        }
        op->ScaleX*=sc;
        op->ScaleY*=sc;
    };
};
class opMove:public fOperator{
public:
    int Dx;
    int Dy;
    float StartTime;
    float Duration;
    SAVE(opMove){
        REG_BASE(fOperator);
        REG_MEMBER(_int,Dx);
        REG_MEMBER(_int,Dy);
        REG_MEMBER(_float,StartTime);
        REG_MEMBER(_float,Duration);
    }ENDSAVE;
    virtual void operate(ParentFrame* op,float Time){
        float dx=0;
        float dy=0;
        if(Time>StartTime){
            Time-=StartTime;
            if(Time<Duration){
                dx=Dx*Time/Duration;
                dy=Dy*Time/Duration;
            }else{
                dx=Dx;
                dy=Dy;
            }
        }
        op->ShiftX+=Dx;
        op->ShiftY+=Dy;
    };
};
class opRotate:public fOperator{
public:
    //int PivotX;
    //int PivotY;
    float StartAngle;
    float FinalAngle;
    float StartTime;
    float Duration;
    SAVE(opRotate){
        //REG_MEMBER(_int,PivotX);
        //REG_MEMBER(_int,PivotY);
        REG_MEMBER(_float,StartAngle);
        REG_MEMBER(_float,FinalAngle);
        REG_MEMBER(_float,StartTime);
        REG_MEMBER(_float,Duration);
    }ENDSAVE;
    virtual void operate(ParentFrame* op,float Time){
        float sc=StartAngle;
        if(Time>StartTime){
            Time-=StartTime;
            if(Time<Duration){
                sc=StartAngle+(FinalAngle-StartAngle)*Time/Duration;                
            }else sc=FinalAngle;            
        }
        op->Angle+=sc;
    }
};
class opColorAndAlphaRamp:public fOperator{
public:
    DWORD StartColor;
    DWORD FinalColor;
    float StartTime;
    float Duration;
    bool  ModulateCurrent2X;
    SAVE(opColorAndAlphaRamp){
        REG_MEMBER(_color,StartColor);
        REG_MEMBER(_color,FinalColor);
        REG_MEMBER(_float,StartTime);
        REG_MEMBER(_float,Duration);
        REG_MEMBER(_bool,ModulateCurrent2X);
    }ENDSAVE;
    virtual void operate(ParentFrame* op,float Time){
        DWORD C=0xFFFFFFFF;        
        if(Time>StartTime){
            Time-=StartTime;
            if(Time<Duration){
                DWORD W=Time*255/Duration;
                C=MixDWORD(StartColor,FinalColor,255-W,W);
            }else C=FinalColor;
        }
        if(ModulateCurrent2X){
            op->Diffuse=ModDWORD2X(op->Diffuse,C);
        }else{
            DWORD A=((op->Diffuse>>24)*(C>>24))>>8;
            op->Diffuse=(C&0xFFFFFF)+(A<<24);
        }
    }
};

DIALOGS_API void FlipPages(void);
DIALOGS_API bool ProcessWindowMessages();

class MicroFlash:public BaseClass{
public:
    float Duration;
    float TrackPosition;
    ClassArray<FlashObject> Objects;
    void Play(){
        float Time=GetTickCount();
        float dTime=0;
        float dTimePrev=dTime;
        do{
            ProcessWindowMessages();
            ShowAt(0,0,RealLx,RealLy,dTime,dTimePrev);
            FlipPages();
            dTimePrev=dTime;
            dTime=float(GetTickCount()-Time)/1000.0f;
        }while(dTime<Duration && !KeyPressed);
        KeyPressed=false;
        LastKey=0;
    }
    void ShowObject(ParentFrame* pf,FlashObject* fob,int x,int y,float ScaleX,float ScaleY,float Time,float PrevTime){
        FlashObjectOut out=fob->Create(pf,Time,PrevTime);
        if(out.pf){
            out.pf->x+=x;
            out.pf->y+=y;
            out.pf->x1+=x;
            out.pf->y1+=y;
            out.pf->ScaleX*=ScaleX;
            out.pf->ScaleY*=ScaleY;
            for(int i=0;i<fob->Operators.GetAmount();i++){
                fob->Operators[i]->operate(out.pf,Time);
            }
            for(int i=0;i<fob->SubObjects.GetAmount();i++){
                ShowObject(out.pf,fob->SubObjects[i],0,0,1,1,Time,PrevTime);
            }
        }
    }
    void ShowAt(int x,int y,int Lx,int Ly,float Time,float PrevTime){
        DialogsSystem ds(x,y);
        ds.EnableTransform=true;
        ds.ScaleX=Lx/1024.0f;
        ds.ScaleY=Ly/768.0f;
        for(int i=0;i<Objects.GetAmount();i++){
            ShowObject(&ds,Objects[i],0,0,1,1,Time,PrevTime);
        }
        ds.ProcessDialogs();
    }
    SAVE(MicroFlash){
        REG_MEMBER(_float,Duration);
        REG_MEMBER(_float01,TrackPosition);
        REG_AUTO(Objects);
    }ENDSAVE;
};
