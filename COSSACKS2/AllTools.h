class AllTools:public ReferableBaseClass{
public:
	AllTools();
	virtual bool CheckIfObjectIsGlobal() { return true; }
	Delimiter D;

	SubSection Basic;

	void DefaultCursor();
	void Peace();
	void SaveMap();
	void LoadMap();

	SubSection Geometry;

	static bool KeepUV;
	void Undo();
	void Deform();
	void Smooth();
	void Texture();
	void Paint();
	void Grass();
	void Boolean();
	void Chamfer();
	void Illuminate();
	void SetRiver();
	void Refresh();

	SubSection Camera;

	void FreeCamera();
	void ToggleHero();
	void ToggleTank();
	void CollCamera();

	SubSection Layer;

	void Race();
	void Archway();
	void Layers();
	void LocalGenerator();
	void Generator();	
	void Scissors();	
	void Export();		

	SubSection Objects;

    void Trees();    
	void Stones();
	void Effects();
	void KillObjects();
	void Units();	
	void CustomizeUnit();
	void Roads();	
	void AddModel();
	void EditModels();

	SubSection Locking;

	void SetLock();
	void ClearLock();
	void DefaultLock();	
	void ShowLock();	
	void ShowTopology();
    void ViewLand();
	void ViewWater();
	void ViewLowAir();
	void ViewHiAir();

	SubSection Scripts;

	void Script();
	void Nodes();
	void Groups();
	void Debug();
	void ScrollGroups();
	void Zones();

	SubSection Properties;

	void MapOptions();
	void EngineSettings();	
	void GameSettings();
	void DialogsEditor();
	void AbilitiesEditor();
	void RoadsEditor();	
	void FacturesProperties();	
	void GrassProperties();
	void AllEditors();	

	SAVE(AllTools){
		REG_PARENT(ReferableBaseClass);

		REG_AUTO(Basic);

		REG_METHOD(DefaultCursor);
		REG_METHOD(Peace);
		REG_METHOD(SaveMap);
		REG_METHOD(LoadMap);

		REG_AUTO(Geometry);

		STATIC REG_MEMBER_EX2(_bool,KeepUV,"    KeepUV");
		REG_METHOD(Undo);
		REG_METHOD(Deform);
		REG_METHOD(Smooth);
		REG_METHOD(Texture);
		REG_METHOD(Paint);
		REG_METHOD(Boolean);
		REG_METHOD(Chamfer);
		REG_METHOD(Scissors);	
		REG_METHOD(Illuminate);
		REG_METHOD(SetRiver);
		REG_METHOD(Refresh);

		REG_AUTO_EX(Layer,Generator);

		REG_METHOD(Race);
		REG_METHOD(Archway);
		REG_METHOD(Layers);
		REG_METHOD(LocalGenerator);
		REG_METHOD(Generator);				
		REG_METHOD(Export);		

		REG_AUTO(Camera);

		REG_METHOD(FreeCamera);
		REG_METHOD(ToggleHero);
		REG_METHOD(ToggleTank);
		REG_METHOD(CollCamera);

		REG_AUTO(Objects);
		
		REG_METHOD(Trees);    
		REG_METHOD(Stones);
		REG_METHOD(Effects);
		REG_METHOD(KillObjects);
		REG_METHOD(Units);	
		REG_METHOD(CustomizeUnit);
		REG_METHOD(Roads);
		REG_METHOD(AddModel);
		REG_METHOD(EditModels);

		REG_AUTO(Locking);
		
		REG_METHOD(SetLock);
		REG_METHOD(ClearLock);
		REG_METHOD(DefaultLock);
		REG_AUTO(D);
		REG_METHOD(ShowLock);
		REG_METHOD(ShowTopology);
		REG_AUTO(D);
		REG_METHOD(ViewLand);
		REG_METHOD(ViewWater);
		REG_METHOD(ViewLowAir);
		REG_METHOD(ViewHiAir);

		REG_AUTO(Scripts);

		REG_METHOD(Script);
		REG_METHOD(Nodes);
		REG_METHOD(Groups);
        REG_METHOD(Debug);
		REG_AUTO(D);
        REG_METHOD(ScrollGroups);
        REG_METHOD(Zones);

		REG_AUTO(Properties);

		REG_METHOD(MapOptions);
		//REG_METHOD(EngineSettings);		
		REG_METHOD(GameSettings);
		REG_METHOD(DialogsEditor);
		REG_METHOD(AbilitiesEditor);
		REG_METHOD(RoadsEditor);
		REG_METHOD(FacturesProperties);	
		REG_METHOD(GrassProperties);
		REG_METHOD(AllEditors);		
		
	}ENDSAVE;
};
void ShowAllToolsToolbar();
void InitTools();