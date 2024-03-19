
#pragma once

const int DefaultHashTSize=3517;
//KeyClass should have functions, if not defined by default:
//DWORD  operator =  (const Class& c) const;
//Class& operator =  (const Class& c) const;
//bool   operator == (const Class& c) const;
//Class should have functions, if not defined by default:
//Class& operator =  (const Class& c) const;
class hash_context{
public:
    hash_context(){
        ptr=NULL;
        ScanAll=false;
        TblPos=0;
    }
    void* ptr;
    bool ScanAll;
    int  TblPos;
};
template <class Class,class KeyClass=DWORD,int TableSize = DefaultHashTSize,int PoolSectionSize=64>
class uni_hash{	
    struct temp_s{
        KeyClass	key;
        Class		element;
        temp_s*		next;
        int			index;
    };
    typedef temp_s _parray [PoolSectionSize];
    temp_s*					table[TableSize];
    DynArray<temp_s*>	pool;
    DynArray<temp_s*>	freeVals;
    temp_s* _get_temp_s(){
        if(!freeVals.GetAmount()){
            temp_s* pa=new temp_s[PoolSectionSize];
            int sz0=pool.GetAmount()*PoolSectionSize;
            pool.Add(pa);
            for(int i=0;i<PoolSectionSize;i++){
                temp_s* ts=pa+PoolSectionSize-i-1;
                ts->index=-1-(PoolSectionSize-i-1+sz0);
                freeVals.Add(ts);
            }
        }        
        temp_s* fv=freeVals.pop_back();
        fv->index=-1-fv->index;
        assert(fv->index>=0);
        return fv;
    }	
	Class DefValue;
public:
    _inline				uni_hash	();	
    _inline	int			size		();
    _inline int			size		(KeyClass K);
    _inline Class*		get			(KeyClass K,int SubIndex=0,bool AddIfNotExists=false);
    _inline int			get_i		(KeyClass K,int SubIndex=0,bool AddIfNotExists=false);
	_inline Class*		get_last	(KeyClass K,bool AddIfNotExists=false);
	_inline int			get_i_last	(KeyClass K,bool AddIfNotExists=false);
    _inline Class*		add			(KeyClass K,Class& C);
    _inline int			add_i		(KeyClass K,Class& C);
    _inline Class*		add_uniq	(KeyClass K,Class& C);//add uniq combination (K,C) - C should define operator ==
    _inline int			add_uniq_i	(KeyClass K,Class& C);
    _inline Class*		replace		(KeyClass K,Class& C,int SubIndex=-1);
    _inline int			replace_i	(KeyClass K,Class& C,int SubIndex=-1);
    _inline Class*		add_once	(KeyClass K,Class& C);
    _inline int			add_once_i	(KeyClass K,Class& C);
    _inline bool		del			(KeyClass K,int SubIndex=-1);	
    _inline bool		del_elm		(KeyClass K,Class& C);//requires operator == for C
    _inline Class*		operator[]  (KeyClass K);
    _inline Class*		elem		(int idx);
    _inline KeyClass*	key 		(int idx);
    _inline void		reset		();
	_inline void		fast_reset	();
    _inline void		start_scan	(KeyClass K,hash_context& ct);
    _inline void		start_scan	(hash_context& ct);
    _inline Class*		next		(hash_context& ct);
    _inline int			next_i		(hash_context& ct);
    _inline KeyClass*   next_key    (hash_context& ct);
    _inline DWORD		get_free_idx();
    _inline void        del_all     ();
	_inline void		set_default (Class C){DefValue=C;}
	_inline int			simple_serialize(BYTE** buf);//this function will allocate the buffer and return size
	_inline int			simple_unserialize(BYTE* buf);//returns amount of used bytes from buffer

    //////////////////////////////////////////////////////////////////////////    

    _inline void        self_test   (){
        for(int i=0;i<TableSize;i++){
            temp_s* ts=table[i];
            int nn=512;
            while(ts && nn--){
                assert(ts->index>=0);
                ts=ts->next;
            }
            assert(nn>0);
        }
    }
};
//useful macro commands
#define scan(_Hash,_ElmPtr,_KeyPtr) {hash_context _hct##__LINE__;int _elmID;\
    _Hash.start_scan(_hct##__LINE__);\
    while((_elmID=_Hash.next_i(_hct##__LINE__))!=-1){\
        _ElmPtr=_Hash.elem(_elmID);\
        _KeyPtr=_Hash.key(_elmID);

#define scan_e(_Hash,_ElmPtr) {hash_context _hct##__LINE__;\
    _Hash.start_scan(_hct##__LINE__);\
    while(_ElmPtr=_Hash.next(_hct##__LINE__)){       

#define scan_k(_Hash,_ElmPtr) {hash_context _hct##__LINE__;\
    _Hash.start_scan(_hct##__LINE__);\
    while(_ElmPtr=_Hash.next_key(_hct##__LINE__)){       

#define scan_key(_Hash,_Key0,_ElmPtr) {hash_context _hct##__LINE__;\
    _Hash.start_scan(_Key0,_hct##__LINE__);\
    while(_ElmPtr=_Hash.next(_hct##__LINE__)){        

#define scan_end }}

//////////////////////////////////////////////////////////////////////////

_inline DWORD& _DWORD(DWORD V){
    static DWORD v;
    v=V;
    return v;
}
struct bi_DWORD{
    DWORD V1,V2;
    bi_DWORD(){};
    bi_DWORD(DWORD v1,DWORD v2){V1=v1;V2=v2;}
    operator DWORD(){
        return V1+V2*123761;
    }	
    bool   operator == (const bi_DWORD& c) const{
        return c.V1==V1 && c.V2==V2;
    }
};
struct DWORDS2{//sorted 2 DWORD-s
    DWORD V1,V2;
    void sort(){
		if(V2<V1)std::swap(V2,V1);
    }
    DWORDS2(){};
    DWORDS2(DWORD v1,DWORD v2){
        V1=v1;
        V2=v2;
        sort();
    }
    void set(DWORD v1,DWORD v2){
        V1=v1;
        V2=v2;
        sort();
    }
    operator DWORD(){
        return V1+V2*123761;
    }	
    bool   operator == (const DWORDS2& c) const{
        return c.V1==V1 && c.V2==V2;
    }
};
struct DWORDS2A{//sorted 2 DWORD-s + Attribute
	DWORD V1,V2,A;
	void sort(){
		if(V2<V1)std::swap(V2,V1);
	}
	DWORDS2A(){};
	DWORDS2A(DWORD v1,DWORD v2,DWORD a){
		V1=v1;
		V2=v2;
		A=a;
		sort();
	}
	void set(DWORD v1,DWORD v2,DWORD a){
		V1=v1;
		V2=v2;
        A=a;
		sort();
	}
	operator DWORD(){
		return V1+V2*12361+A*17217;
	}	
	bool   operator == (const DWORDS2A& c) const{
		return c.V1==V1 && c.V2==V2 && c.A==A;
	}
};
struct DWORDS3{//sorted 3 DWORD-s
    DWORD V1,V2,V3;
    void sort(){
        for(int i=0;i<2;i++){
            if(V2<V1)std::swap(V2,V1);
            if(V3<V2)std::swap(V3,V2);
        }
    }
    DWORDS3(){};
    DWORDS3(DWORD v1,DWORD v2,DWORD v3){
        V1=v1;
        V2=v2;
        V3=v3;
        sort();
    }
    void set(DWORD v1,DWORD v2,DWORD v3){
        V1=v1;
        V2=v2;
        V3=v3;
        sort();
    }
    operator DWORD(){
        return V1+V2*123761+V3*17393;
    }	
    bool   operator == (const DWORDS3& c) const{
        return c.V1==V1 && c.V2==V2 && c.V3==V3;
    }
};
struct tri_DWORD{
    DWORD V1,V2,V3;
    tri_DWORD(){};
    tri_DWORD(DWORD v1,DWORD v2,DWORD v3){V1=v1;V2=v2;V3=v3;}
    operator DWORD(){
        return V1+V2*123761+V3*1971;
    }
    bool   operator == (const tri_DWORD& c) const{
        return c.V1==V1 && c.V2==V2 && c.V3==V3;
    }
};
//implementation of [ uni_hash ]
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::uni_hash	(){
    reset();
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline Class* uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::get(KeyClass K,int SubIndex,bool AddIfNotExists){
    DWORD hash=K;
    hash%=TableSize;
    temp_s* el=table[hash];
    while(el){
        if(el->key==K){
            if(SubIndex>0)SubIndex--;
            else return &el->element;			
        }
        el=el->next;
    }
	if(AddIfNotExists){		
		return add(K,DefValue);
	}
    return NULL;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline int uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::get_i(KeyClass K,int SubIndex,bool AddIfNotExists){
    DWORD hash=K;
    hash%=TableSize;
    temp_s* el=table[hash];
    while(el){
        if(el->key==K){
            if(SubIndex>0)SubIndex--;
            else return el->index;
        }
        el=el->next;
    }
	if(AddIfNotExists){		
		return add_i(K,DefValue);
	}
    return -1;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline Class* uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::get_last(KeyClass K,bool AddIfNotExists){
	DWORD hash=K;
	hash%=TableSize;
	temp_s* el=table[hash];
	Class* C=NULL;
	while(el){
		if(el->key==K){
			C=&el->element;
		}
		el=el->next;
	}
	if(C==NULL && AddIfNotExists){		
		return add(K,DefValue);
	}
	return C;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline int uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::get_i_last(KeyClass K,bool AddIfNotExists){
	DWORD hash=K;
	hash%=TableSize;
	temp_s* el=table[hash];
	int idx=-1;
	while(el){
		if(el->key==K){
			idx=el->index;			
		}
		el=el->next;
	}
	if(idx==-1 && AddIfNotExists){		
		return add_i(K,DefValue);
	}
	return idx;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline	int uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::size(){
    return (pool.GetAmount()*PoolSectionSize)-freeVals.GetAmount();
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline int uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::size(KeyClass K){
    DWORD hash=K;
    hash%=TableSize;
    temp_s* el=table[hash];
    int n=0;
    if(el){
        do{
            if(el->key==K)n++;
            el=el->next;
        }while(el);    
    }
    return n;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline Class* uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::add(KeyClass K,Class& C){
    DWORD hash=K;
    hash%=TableSize;
    temp_s* el=table[hash];	
    temp_s* ptr=_get_temp_s();
    ptr->element=C;
    ptr->key=K;
    ptr->next=NULL;
    el=table[hash];    
    if(el){		
        assert(el!=el->next);
        while(el->next){
            assert(el!=el->next);
            el=el->next;
        }
        el->next=ptr;
    }else{
        table[hash]=ptr;
    } 
    return &ptr->element;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline int uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::add_i(KeyClass K,Class& C){
    DWORD hash=K;
    hash%=TableSize;
    temp_s* el=table[hash];	
    temp_s* ptr=_get_temp_s();
    ptr->element=C;
    ptr->key=K;
    ptr->next=NULL;
    el=table[hash];
    if(el){		
        while(el->next)el=el->next;
        el->next=ptr;
    }else{
        table[hash]=ptr;
    } 
    return ptr->index;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline Class* uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::add_uniq(KeyClass K,Class& C){
    DWORD hash=K;
    hash%=TableSize;
    temp_s* el=table[hash];
    while(el){
        if(el->key==K && el->element==C){
            el->key=K;
            el->element=C;
            return &el->element;
        }
        el=el->next;
    }
    el=table[hash];	
    temp_s* ptr=_get_temp_s();
    ptr->element=C;
    ptr->key=K;
    ptr->next=NULL;
    el=table[hash];
    if(el){		
        while(el->next)el=el->next;
        el->next=ptr;
    }else{
        table[hash]=ptr;
    } 
    return &ptr->element;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline int uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::add_uniq_i(KeyClass K,Class& C){
    DWORD hash=K;
    hash%=TableSize;
    temp_s* el=table[hash];	
    while(el){
        if(el->key==K && el->element==C){
            el->key=K;
            el->element=C;
            return el->index;
        }
        el=el->next;
    }
    el=table[hash];	
    temp_s* ptr=_get_temp_s();
    ptr->element=C;
    ptr->key=K;
    ptr->next=NULL;
    el=table[hash];
    if(el){		
        while(el->next)el=el->next;
        el->next=ptr;
    }else{
        table[hash]=ptr;
    } 
    return ptr->index;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline Class* uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::replace(KeyClass K,Class& C,int SubIndex){
    Class* CL=get(K,SubIndex);
    if(CL)*CL=C;
    return CL;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline int uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::replace_i(KeyClass K,Class& C,int SubIndex){
    int idx=get_i(K,SubIndex);
    if(idx!=-1)*(elem[idx])=C;
    return idx;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline Class* uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::add_once(KeyClass K,Class& C){
    Class* CL=get(K);
    if(CL){
        *CL=C;
        return CL;
    }else return add(K,C);
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline int uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::add_once_i(KeyClass K,Class& C){
    int idx=get_i(K);
    if(idx!=-1){
        *(elem(idx))=C;
        return idx;
    }else return add_i(K,C);
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline bool uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::del(KeyClass K,int SubIndex=-1){
    DWORD hash=K;
    hash%=TableSize;
    temp_s* el=table[hash];
    if(!el)return false;
    temp_s* prev=NULL;
    bool _del=false;
    do{
        bool donext=true;
        if(el->key==K){
            if(SubIndex>0)SubIndex--;
            else{
                _del=true;
                if(prev)prev->next=el->next;
                else{
                    table[hash]=el->next;
                }
                el->index=-1-el->index;
                freeVals.Add(el);
                if(SubIndex==0)return true;
                el=prev;
                if(!el){
                    el=table[hash];
                    donext=false;
                }
            }			
        }        
        if(donext){
            prev=el;
            if(el)el=el->next;
        }
    }while(el);
    return _del;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline bool uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::del_elm(KeyClass K,Class& C){
    DWORD hash=K;
    hash%=TableSize;
    temp_s* el=table[hash];
    if(!el)return false;
    temp_s* prev=NULL;
    bool _del=false;
    do{
        if(el->key==K && C==el->element){            
            _del=true;
            if(prev)prev->next=el->next;
            else table[hash]=el->next;
            el->index=-1-el->index;
            freeVals.Add(el);
            return true;
        }
        prev=el;
        el=el->next;
    }while(el);
    return _del;    
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline Class* uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::operator[](KeyClass K){
    return get(K,0);
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline void uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::reset(){
    for(int i=0;i<pool.GetAmount();i++)delete[]pool[i];
    pool.FastClear();
    freeVals.FastClear();
    memset(table,0,sizeof(table));
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline void uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::fast_reset(){	
	freeVals.FastClear();
	int psz=pool.GetAmount();
	for(int i=psz-1;i>=0;i--){
		temp_s* p=pool[i]+PoolSectionSize-1;
		for(int j=0;j<PoolSectionSize;j++,p--){
			if(p->index>=0)p->index=-1-p->index;                    
			freeVals.Add(p);
		}
	}	
	memset(table,0,sizeof(table));
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline void uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::start_scan(KeyClass K,hash_context& ct){
    ct.ScanAll=false;
    DWORD hash=K;
    hash%=TableSize;
    temp_s* el=table[hash];	
    while(el && el->key!=K)el=el->next;
    ct.ptr=(void*)el;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline void uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::start_scan(hash_context& ct){
    ct.ScanAll=true;
    ct.TblPos=0;
    ct.ptr=(void*)table[0];
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline Class* uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::next(hash_context& ct){
    if(ct.ScanAll){
        temp_s* ts=(temp_s*)ct.ptr;
        while(!ts){
            if(++ct.TblPos>=TableSize)return NULL;
            ts=table[ct.TblPos];
        }
        ct.ptr=(void*)ts->next;
        return &ts->element;
    }else{
        temp_s* ts=(temp_s*)ct.ptr;
        if(ts){
            temp_s* el=ts;
            if(el)el=el->next;
            while(el && el->key!=ts->key)el=el->next;
            ct.ptr=el;
            return &ts->element;
        }
        return NULL;
    }
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline KeyClass* uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::next_key(hash_context& ct){
    if(ct.ScanAll){
        temp_s* ts=(temp_s*)ct.ptr;
        while(!ts){
            if(++ct.TblPos>=TableSize)return NULL;
            ts=table[ct.TblPos];
        }
        ct.ptr=(void*)ts->next;
        return &ts->key;
    }else{
        temp_s* ts=(temp_s*)ct.ptr;
        if(ts){
            temp_s* el=ts;
            if(el)el=el->next;
            while(el && el->key!=ts->key)el=el->next;
            ct.ptr=el;
            return &ts->key;
        }
        return NULL;
    }
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline int uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::next_i(hash_context& ct){
    if(ct.ScanAll){
        temp_s* ts=(temp_s*)ct.ptr;
        while(!ts){
            if(++ct.TblPos>=TableSize)return -1;
            ts=table[ct.TblPos];
        }
        ct.ptr=(void*)ts->next;
        return ts->index;
    }else{
        temp_s* ts=(temp_s*)ct.ptr;
        if(ts){
            temp_s* el=ts;
            while(el && el->key!=ts->key)el=el->next;
            if(el)ct.ptr=(void*)el->next;
            else ct.ptr=NULL;
            return ts->index;
        }
        return -1;
    }
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline DWORD uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::get_free_idx(){
    temp_s* e=_get_temp_s();
    DWORD id=e->index;
    e->index=-1-e->index;
    freeVals.Add(e);    
    return id;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline Class* uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::elem(int idx){
    temp_s* el=pool[idx/PoolSectionSize]+(idx%PoolSectionSize);
    if(el && el->index>=0)return &el->element;
    else return NULL;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline KeyClass* uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::key(int idx){
    temp_s* el=pool[idx/PoolSectionSize]+(idx%PoolSectionSize);
    if(el->index>=0)return &el->key;
    else return NULL;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline void uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::del_all(){
    int psz=pool.GetAmount();
    for(int i=0;i<psz;i++){
        temp_s* p=pool[i];
        for(int j=0;j<PoolSectionSize;j++,p++){
            if(p->index>=0){
                DWORD k=DWORD(p->key)%TableSize;
                temp_s* tps=table[k];
                while(tps){
                    tps->index=-1-tps->index;                    
                    tps=tps->next;
                }
                table[k]=0;
            }
        }
    }
    freeVals.FastClear();
    for(int i=psz-1;i>=0;i--){
        temp_s* p=pool[i]+PoolSectionSize-1;
        for(int j=0;j<PoolSectionSize;j++,p--)
            freeVals.Add(p);
    }
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline int uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::simple_serialize(BYTE** buf){
	int n=0;
	scan((*this),Class* C,KeyClass* K){
		n++;
	}scan_end;	
	int sz=n*(sizeof(KeyClass)+sizeof(Class))+4;
	*buf=new BYTE[sz];
	int pos=4;
	memcpy(*buf,&n,4);
	scan((*this),Class* C,KeyClass* K){
		memcpy((*buf)+pos,K,sizeof(KeyClass));
		pos+=sizeof(KeyClass);
		memcpy((*buf)+pos,C,sizeof(Class));
		pos+=sizeof(Class);
	}scan_end;
	return pos;
}
template <class Class,class KeyClass,int TableSize,int PoolSectionSize>
_inline int uni_hash<Class,KeyClass,TableSize,PoolSectionSize>::simple_unserialize(BYTE* buf){
	int n=0;
	int pos=4;
    memcpy(&n,buf,4);
	reset();
	for(int i=0;i<n;i++){
		KeyClass K;
		Class C;
		memcpy(&K,buf+pos,sizeof KeyClass);
		pos+=sizeof(KeyClass);
		memcpy(&C,buf+pos,sizeof Class);
		pos+=sizeof(Class);
        add(K,C);
	}
	return n*(sizeof(KeyClass)+sizeof(Class))+4;
}