class FigureType;
class ChessFigure:public BaseClass{
    //pointer to the list of units in group
    pUnitsList* pList;
    //figure type
    int         fType;
    //index of current cell of the group
    int         Position;
    //index of cell, where units are moving
    int         DestPosition;
    //figure is locked due
    bool        LockedDueToAtack;
    //remaining units amount after simulation
    int         RemainingUnitsAfterSimulation;    
    //figure color - black(0) or white(1), may be more colors in future
    int         Color;

    ////

    FigureType* GetFigureType();
};
class FigureType:public BaseClass{
public:
    //units type in group
    pUnitType uType;
    //standart amount of units in group
    int StdUnitsAmount;
    //number of motion steps per game step
    int DistanceOfMovingPerStep;
    //this type of units can move only forward
    bool MovesOnlyForward;
    //radius of shooting, if possible, othervice 0
    int ShootingRadius;
};
class OneChessCell:public BaseClass{
public:
    //position
    int x,y;
    //index in "Figures" array
    int OwnerID;
    //cells that are near to the current
    DynArray<int> NearCells;

    ////

    ChessFigure* GetFigure();
};
class ChessBattleField:public BaseClass{
public:
    //list of cells
    ClassArray<OneChessCell> ChessCells;    
    //list of possible figures
    ClassArray<FigureType>   FigureTypes;
    //this direction should be used to detemine where are back or forward directions
    iVector2D                GeneralDirection;
    //relative attack force is square NxN array (N=number of figure types).
    //for example A - attacker type, D - defender type, 
    //nA(maxA) - amount(max amount) of attacking units, nD(maxD) - amount(max amount) of defenders, then
    //float AF=RelativeAttackForce[A*N+D];
    //nD-=nA*AF*maxD/maxA; - lost during step
    //so AF if percent of dead units during the step if attackers formation is full
    DynArray<float>          RelativeAttackForce;    
    //list of groups, that take part in battle
    ClassArray<pUnitsList>   Groups;

    ////

    void AddFigureType(pUnitType uType,int StdAmount,int DistanceOfMovingPerStep,int ShootingRadius,bool MovesOnlyForward);
    void CreateChessCellsFromTopologicalZones(iZone& Zone,iVector2D Direction);
    void SetRelativeForce(int AttackerType,int DefenderType,float Force);
};
class ChessBattle:public BaseClass{
public:
    //these static fields should be used for classes ChessFigure, FigureType to
    //know current context
    static ChessBattle* CurrentBattle;
    static ChessBattleField* CurrentBattleField;

    //reference to battle field
    ChessBattleField*        BFieldPtr;
    //figures list
    ClassArray<ChessFigure>  Figures;
    //side that should now make step
    int                      SideToMakeStep;

    ////

    //every of next funtions should at first call SetContext() to fill CurrentBattle and CurrentBattleField
    //by appropriate values so that other classes will be able to know current context using
    //ChessBattle::CurrentBattle and ChessBattle::CurrentBattleField statements
    void SetContext();
    //it is only for initialising the field CurrentBattleField
    void SetBattleField(ChessBattleField* BF);
    //create new units at the specified cell and add them to the battle system
    void CreateFigure(int CellIndex,int FigureType,int Color);
    //next two functions should be used to get possible next steps
    void GetPossibleMovingWays(int FigureIndex,DynArray<int>& DestPositions);
    void GetPossibleRangedAttackWays(int FigureIndex,DynArray<int>& DestPositions);
    //it is uset to create bacups of current battle, so we will be able move figures like "in mind"
    void CopyBattle(ChessBattle* DestBattle);

    //perform step. During step we can attack or simply move to some cell. 
    //Also we can only simulate battle "in mind" using Simulate=true
    //this function should return false if we are not able to make step
    bool MakeStep(int Figure,int DestCell,bool AttackOnly,bool Simulate);
};
class ChessBattleInterface:public BaseClass{
public:
    //draws arrows over selected formation, also draws chess fields
    void DrawInterface();
    //processes moving/attack during the step, it will be called every step
    void Process();
    //ends current step - kills units that should die due to simulation
    void EndStep();
};