#pragma once

void PrepareSpellLink(OneObject* OB);
bool PrepareSpell(OneObject* OB, byte OrdType, DWORD AblSerial, OneObject* Target, int Tx, int Ty, int Dist, int Radius, int CursorID);

void CastingSpellLink(OneObject* OB);
bool CastingSpell(OneObject* OB, byte OrdType);