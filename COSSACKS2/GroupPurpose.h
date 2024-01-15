#ifndef	__GROUP__PURPOSE__ 
#define __GROUP__PURPOSE__

extern word FULLWAY[128];
extern int NWPTS;
extern int TOPTYPE;

typedef word tpGetObjID(int);

DLLEXPORT
int FindNextZoneOnTheSafeWay(	int		Start		,
								int		Fin			,
								short*	DangerMap	,
								int*	MaxDanger	,
								int		DangSteps	
							);

DLLEXPORT
int FindNextZoneOnTheSafeWayEx(	int Start		,
								int Fin			,
								int* DangerMap	,
								int* MaxDanger	,
								int DangSteps
							  );

DLLEXPORT
int FindNextZoneOnTheSafeWayEx2(int Start		,
								int Fin			,
								int* DangerMap	,
								int* MaxDanger	,
								int DangSteps	, 
								byte TopType
							   );

DLLEXPORT
int FindNextZoneOnTheSafeWayToObject(	int Start		,
										short* DangerMap,
										word* ObjIds	,
										int* MaxDanger	,
										int DangSteps	,
										word* DestObj
									);

int FindNextZoneOnTheSafeWayToObjectWithCallback(	int Start		,
													short* DangerMap,
													tpGetObjID* fn	,
													int* MaxDanger	,
													int DangSteps	,
													word* DestObj
												);

DLLEXPORT
int FindNextZoneOnTheSafeWayToLimitedObject(	int Start		,
												short* DangerMap,
												word* ObjIds	,
												word* Types		,
												word ReqType	,
												int* MaxDanger	,
												int DangSteps	,
												word* DestObj
											);

#endif//__GROUP__PURPOSE__