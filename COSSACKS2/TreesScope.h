struct OneTreesGroup{
	DynArray<int> Indices;
	DynArray<SprGroup*> SGS;
};
class TreesScope{
public:
	Enumerator* TEnum;
	DynArray<OneTreesGroup*> TGroup;
	void Clear(){
		if(!TEnum)TEnum=ENUM.Get("TREESGROUPS");
		for(int i=0;i<TGroup.GetAmount();i++)delete(TGroup[i]);
		TEnum->Clear();
		TGroup.Clear();
	}
	void Add(char* Section,SprGroup* SG,int Index){
		if(!TEnum)TEnum=ENUM.Get("TREESGROUPS");
		int V=TEnum->Get(Section);
		if(V==-1){
			V=TEnum->GetAmount();
			TEnum->Add(Section);
			OneTreesGroup* TG=new OneTreesGroup;
			TGroup.Add(TG);
		}
		OneTreesGroup* OTG=TGroup[V];
		OTG->Indices.Add(Index);
		OTG->SGS.Add(SG);
	}
	OneTreesGroup* Get(char* GrpName){
		if(!TEnum)TEnum=ENUM.Get("TREESGROUPS");
		int v=TEnum->Get(GrpName);
		if(v==-1)return NULL;
		else return TGroup[v];
	}
	~TreesScope(){
		Clear();
	}
	TreesScope(){
		TEnum=0;
	}
};
extern TreesScope TSC;