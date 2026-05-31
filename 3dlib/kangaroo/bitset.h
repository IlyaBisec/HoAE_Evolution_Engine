#pragma once

class UnlimitedBitset{
	DWORDS Data;
public:
	~UnlimitedBitset();
	void set(int pos,bool value);
	bool get(int pos);
	void clear();
};