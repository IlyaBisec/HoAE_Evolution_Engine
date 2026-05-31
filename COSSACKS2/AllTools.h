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

		REG_METHOD(&AllTools::DefaultCursor);
		REG_METHOD(&AllTools::Peace);
		REG_METHOD(&AllTools::SaveMap);
		REG_METHOD(&AllTools::LoadMap);

		REG_AUTO(Geometry);

		STATIC REG_MEMBER_EX2(_bool,KeepUV,"    KeepUV");
		REG_METHOD(&AllTools::Undo);
		REG_METHOD(&AllTools::Deform);
		REG_METHOD(&AllTools::Smooth);
		REG_METHOD(&AllTools::Texture);
		REG_METHOD(&AllTools::Paint);
		REG_METHOD(&AllTools::Boolean);
		REG_METHOD(&AllTools::Chamfer);
		REG_METHOD(&AllTools::Scissors);
		REG_METHOD(&AllTools::Illuminate);
		REG_METHOD(&AllTools::SetRiver);
		REG_METHOD(&AllTools::Refresh);

		REG_AUTO_EX(Layer, &AllTools::Generator);

		REG_METHOD(&AllTools::Race);
		REG_METHOD(&AllTools::Archway);
		REG_METHOD(&AllTools::Layers);
		REG_METHOD(&AllTools::LocalGenerator);
		REG_METHOD(&AllTools::Generator);
		REG_METHOD(&AllTools::Export);

		REG_AUTO(Camera);

		REG_METHOD(&AllTools::FreeCamera);
		REG_METHOD(&AllTools::ToggleHero);
		REG_METHOD(&AllTools::ToggleTank);
		REG_METHOD(&AllTools::CollCamera);

		REG_AUTO(Objects);
		
		REG_METHOD(&AllTools::Trees);
		REG_METHOD(&AllTools::Stones);
		REG_METHOD(&AllTools::Effects);
		REG_METHOD(&AllTools::KillObjects);
		REG_METHOD(&AllTools::Units);
		REG_METHOD(&AllTools::CustomizeUnit);
		REG_METHOD(&AllTools::Roads);
		REG_METHOD(&AllTools::AddModel);
		REG_METHOD(&AllTools::EditModels);

		REG_AUTO(Locking);
		
		REG_METHOD(&AllTools::SetLock);
		REG_METHOD(&AllTools::ClearLock);
		REG_METHOD(&AllTools::DefaultLock);
		REG_AUTO(D);
		REG_METHOD(&AllTools::ShowLock);
		REG_METHOD(&AllTools::ShowTopology);
		REG_AUTO(D);
		REG_METHOD(&AllTools::ViewLand);
		REG_METHOD(&AllTools::ViewWater);
		REG_METHOD(&AllTools::ViewLowAir);
		REG_METHOD(&AllTools::ViewHiAir);

		REG_AUTO(Scripts);

		REG_METHOD(&AllTools::Script);
		REG_METHOD(&AllTools::Nodes);
		REG_METHOD(&AllTools::Groups);
        REG_METHOD(&AllTools::Debug);
		REG_AUTO(D);
        REG_METHOD(&AllTools::ScrollGroups);
        REG_METHOD(&AllTools::Zones);

		REG_AUTO(Properties);

		REG_METHOD(&AllTools::MapOptions);
		//REG_METHOD(EngineSettings);		
		REG_METHOD(&AllTools::GameSettings);
		REG_METHOD(&AllTools::DialogsEditor);
		REG_METHOD(&AllTools::AbilitiesEditor);
		REG_METHOD(&AllTools::RoadsEditor);
		REG_METHOD(&AllTools::FacturesProperties);
		REG_METHOD(&AllTools::GrassProperties);
		REG_METHOD(&AllTools::AllEditors);
		
	}ENDSAVE;
};
void ShowAllToolsToolbar();
void InitTools();