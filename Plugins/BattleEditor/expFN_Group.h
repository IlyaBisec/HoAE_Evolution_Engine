#ifndef __EXP_FN_GROUP__
#define __EXP_FN_GROUP__

void	DeclareteDLL_expFN_Group();

EDITOR_FUNCTION RemoveUnitsInZoneToGroup			(__PARAMS__);
EDITOR_FUNCTION GROUP_AddSelected					(__PARAMS__);
EDITOR_FUNCTION GROUP_AddInZone						(__PARAMS__);
EDITOR_FUNCTION GROUP_AddTypeInZone					(__PARAMS__);
EDITOR_FUNCTION GROUP_SetLockType					(__PARAMS__);
EDITOR_FUNCTION GROUP_PutRandFormation				(__PARAMS__);
EDITOR_FUNCTION GROUP_GetNationAmout				(__PARAMS__);
EDITOR_FUNCTION GROUP_RemoveAllUnits				(__PARAMS__);
EDITOR_FUNCTION NATION_ClearSelection				(__PARAMS__);
EDITOR_FUNCTION	GROUP_FreeFromFSM					(__PARAMS__);

//////////////////////////////////////////////////////////////////////////
// ATTACK_FN			
//////////////////////////////////////////////////////////////////////////
EDITOR_FUNCTION GROUP_ATTACK_GROUP					(__PARAMS__);
EDITOR_FUNCTION GROUP_ATTACK_NODE					(__PARAMS__);
EDITOR_FUNCTION ADD_STORM							(__PARAMS__);
EDITOR_FUNCTION GROUP_ATTACK_00						(__PARAMS__);
EDITOR_FUNCTION GROUP_EXPLORING_MAP					(__PARAMS__);

#endif//__EXP_FN_GROUP__

//////////////////////////////////////////////////////////////////////////