//-----------------------------------------------------------------------------------------------------------------//
class Resource : public BaseClass
{
public:
	virtual byte GetType();
	virtual bool Add(Resource* R);
};
//-----------------------------------------------------------------------------------------------------------------//
class ResourcePack : public ClassArray<Resource>
{
public:
	void Add(ResourcePack* RP);
};
//-----------------------------------------------------------------------------------------------------------------//
/*
class Action : public BaseClass 
{
public:
	virtual bool GetResourcesToDo(ResourcePack* Product, ResourcePack* RequiredResources);
};
*/
//-----------------------------------------------------------------------------------------------------------------//
class ExpansionStack
{
public:
	ClassArray<ResourcePack> ResourceStack;
};
//-----------------------------------------------------------------------------------------------------------------//
class Performer : public BaseClass
{
public:
	//LinearArray<_WORD,word> UnitAgent;

	virtual bool GetResourcesToDo(ResourcePack* Product, ResourcePack* RequiredResources);
	//ClassArray<Action> CanDoActions;
	//virtual byte GetPriority();
};
//-----------------------------------------------------------------------------------------------------------------//
class Chief
{
public:
	ResourcePack NationalResources;
	ExpansionStack ActiveStack;
	ClassArray<Performer> Performers;

	ExpansionStack TempStack;
	ClassArray<Performer> TempPerformers;
};
//-----------------------------------------------------------------------------------------------------------------//
//-----------------------------------------------------------------------------------------------------------------//