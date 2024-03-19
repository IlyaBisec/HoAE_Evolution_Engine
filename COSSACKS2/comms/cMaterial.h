#pragma once

//*****************************************************************************
// cMaterial
//*****************************************************************************
class cMaterial {
public:
	cMaterial();
	cMaterial(const char *Name, const cColor &Ambient, const cColor &Diffuse, const cColor &Specular, const float Shininess);
	
	cStr Name;
	cColor Ambient;
	cColor Diffuse;
	cColor Specular;
	float Shininess;
	
	struct Map {
		Map();
		
		cStr FileName;
		int Unit;
		bool MipMap;
		cRender::idTexture id;
		cVec2 Offset;
		cVec2 Scale;
		
		bool Load();
	};

	Map Tex[8];
	
	void Load();
	
	static const cMaterial Brass;
	static const cMaterial Bronze;
	static const cMaterial PolishedBronze;
	static const cMaterial Chrome;
	static const cMaterial Copper;
	static const cMaterial PolishedCopper;
	static const cMaterial Gold;
	static const cMaterial PolishedGold;
	static const cMaterial Pewter;
	static const cMaterial Silver;
	static const cMaterial PolishedSilver;
	static const cMaterial Emerald;
	static const cMaterial Jade;
	static const cMaterial Obsidian;
	static const cMaterial Pearl;
	static const cMaterial Ruby;
	static const cMaterial Turquoise;
	static const cMaterial BlackPlastic;
	static const cMaterial BlackRubber;
};