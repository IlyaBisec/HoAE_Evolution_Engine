//------------------------------------------------------------------------------------------------------------------//
class PlaceInAJob : public GameExtension
{
public:

	virtual void OnGameStart();
	virtual void ProcessingGame();
	virtual void OnInitAfterMapLoading();//���������� ����� �� ����������

private:
	int LastProcessTime;
	
	SAVE(PlaceInAJob)
	ENDSAVE;
};
//------------------------------------------------------------------------------------------------------------------//
