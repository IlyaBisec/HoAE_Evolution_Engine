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

		REG_METHOD_EX(&AllTools::DefaultCursor, DefaultCursor);
		REG_METHOD_EX(&AllTools::Peace, Peace);
		REG_METHOD_EX(&AllTools::SaveMap, SaveMap);
		REG_METHOD_EX(&AllTools::LoadMap, LoadMap);

		REG_AUTO(Geometry);

		STATIC REG_MEMBER_EX2(_bool,KeepUV,"    KeepUV");
		REG_METHOD_EX(&AllTools::Undo, Undo);
		REG_METHOD_EX(&AllTools::Deform, Deform);
		REG_METHOD_EX(&AllTools::Smooth, Smooth);
		REG_METHOD_EX(&AllTools::Texture, Texture);
		REG_METHOD_EX(&AllTools::Paint, Paint);
		REG_METHOD_EX(&AllTools::Boolean, Boolean);
		REG_METHOD_EX(&AllTools::Chamfer, Chamfer);
		REG_METHOD_EX(&AllTools::Scissors, Scissors);
		REG_METHOD_EX(&AllTools::Illuminate, Illuminate);
		REG_METHOD_EX(&AllTools::SetRiver, SetRiver);
		REG_METHOD_EX(&AllTools::Refresh, Refresh);

		REG_AUTO(Layer);

		REG_METHOD_EX(&AllTools::Race, Race);
		REG_METHOD_EX(&AllTools::Archway, Archway);
		REG_METHOD_EX(&AllTools::Layers, Layers);
		REG_METHOD_EX(&AllTools::LocalGenerator, LocalGenerator);
		REG_METHOD_EX(&AllTools::Generator, Generator);
		REG_METHOD_EX(&AllTools::Export, Export);

		REG_AUTO(Camera);

		REG_METHOD_EX(&AllTools::FreeCamera, FreeCamera);
		REG_METHOD_EX(&AllTools::ToggleHero, ToggleHero);
		REG_METHOD_EX(&AllTools::ToggleTank, ToggleTank);
		REG_METHOD_EX(&AllTools::CollCamera, CollCamera);

		REG_AUTO(Objects);
		
		REG_METHOD_EX(&AllTools::Trees, Trees);
		REG_METHOD_EX(&AllTools::Stones, Stones);
		REG_METHOD_EX(&AllTools::Effects, Effects);
		REG_METHOD_EX(&AllTools::KillObjects, KillObjects);
		REG_METHOD_EX(&AllTools::Units, Units);
		REG_METHOD_EX(&AllTools::CustomizeUnit, CustomizeUnit);
		REG_METHOD_EX(&AllTools::Roads, Roads);
		REG_METHOD_EX(&AllTools::AddModel, AddModel);
		REG_METHOD_EX(&AllTools::EditModels, EditModels);

		REG_AUTO(Locking);
		
		REG_METHOD_EX(&AllTools::SetLock, SetLock);
		REG_METHOD_EX(&AllTools::ClearLock, ClearLock);
		REG_METHOD_EX(&AllTools::DefaultLock, DefaultLock);
		REG_AUTO(D);
		REG_METHOD_EX(&AllTools::ShowLock, ShowLock);
		REG_METHOD_EX(&AllTools::ShowTopology, ShowTopology);
		REG_AUTO(D);
		REG_METHOD_EX(&AllTools::ViewLand, ViewLand);
		REG_METHOD_EX(&AllTools::ViewWater, ViewWater);
		REG_METHOD_EX(&AllTools::ViewLowAir, ViewLowAir);
		REG_METHOD_EX(&AllTools::ViewHiAir, ViewHiAir);

		REG_AUTO(Scripts);

		REG_METHOD_EX(&AllTools::Script, Script);
		REG_METHOD_EX(&AllTools::Nodes, Nodes);
		REG_METHOD_EX(&AllTools::Groups, Groups);
		REG_METHOD_EX(&AllTools::Debug, Debug);
		REG_AUTO(D);
		REG_METHOD_EX(&AllTools::ScrollGroups, ScrollGroups);
		REG_METHOD_EX(&AllTools::Zones, Zones);

		REG_AUTO(Properties);

		REG_METHOD_EX(&AllTools::MapOptions, MapOptions);
		//REG_METHOD_EX(EngineSettings);		
		REG_METHOD_EX(&AllTools::GameSettings, GameSettings);
		REG_METHOD_EX(&AllTools::DialogsEditor, DialogsEditor);
		REG_METHOD_EX(&AllTools::AbilitiesEditor, AbilitiesEditor);
		REG_METHOD_EX(&AllTools::RoadsEditor,RoadsEditor);
		REG_METHOD_EX(&AllTools::FacturesProperties,FacturesProperties);
		REG_METHOD_EX(&AllTools::GrassProperties,GrassProperties);
		REG_METHOD_EX(&AllTools::AllEditors,AllEditors);
		
	}ENDSAVE;
};
void ShowAllToolsToolbar();
void InitTools();