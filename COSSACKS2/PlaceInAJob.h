//------------------------------------------------------------------------------------------------------------------//
class PlaceInAJob : public GameExtension
{
public:

	virtual void OnGameStart();
	virtual void ProcessingGame();
	virtual void OnInitAfterMapLoading();//Центровать экран на крестьянах

private:
	int LastProcessTime;
	
	SAVE(PlaceInAJob)
	ENDSAVE;
};
//------------------------------------------------------------------------------------------------------------------//
