
// Units ABILITY Import
bool	AddCarryAbility_lua(int Index, UnitAbility* UA);
// StarForce
__declspec(dllexport)
bool	DetectCarryAbility_lua(int Index, UnitAbility* UA);
__declspec(dllexport)
bool	DeleteCarryAbility_lua(int Index, UnitAbility* UA );
__declspec(dllexport)
bool	DeleteCarryAbility_lua(int Index, const char* Name);

class	be_CGROUP;
enum	enABL_STATE;
__declspec(dllexport)
void		SetAblState_lua( be_CGROUP* pGRP, const char* AblName, enABL_STATE state );
enABL_STATE	GetAblState_lua( be_CGROUP* pGRP, const char* AblName );

int			GetAbilityOwnerNation_lua();
int			GetAbilityOwnerObject_lua(be_CGROUP* pGR);
int			GetAbilityActivatorObject_lua(be_CGROUP* pGR);
int			g_beCheckAbilityActivatorObject(be_CGROUP* pGR);

enum	en_AblExecState;
en_AblExecState GetAblExecuteState_lua(	UnitAbility*	UA		, 
									    be_CGROUP*		Caster	, 
										be_CGROUP*		Target	, 
										int				x		, 
										int				y
									  );

en_AblExecState AblExecute_lua		(	UnitAbility*	UA		, 
										be_CGROUP*		Caster	, 
										be_CGROUP*		Target	, 
										int				x		, 
										int				y
									);

void DisableCapturability_lua(be_CGROUP* pGRP);

void CreateNewActiveWeapon_lua(	const char*				WMName,
								int						Index,
								int						sx, 
								int						sy, 
								int						DestIndex, 
								int						dx, 
								int						dy
							  );