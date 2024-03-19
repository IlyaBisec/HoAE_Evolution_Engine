#include "comms.h"

//-----------------------------------------------------------------------------
// cMaterial::Map.ctor
//-----------------------------------------------------------------------------
cMaterial::Map::Map() {
	Unit = 0;
	MipMap = true;
	id = cRender::idNone;
	Offset.SetZero();
	Scale.SetOne();
} // cMaterial::Map.ctor

//-----------------------------------------------------------------------------
// cMaterial::Map::Load
//-----------------------------------------------------------------------------
bool cMaterial::Map::Load() {
	id = cRender::idNone;
	if(FileName.Length() == 0) {
		return true;
	}
	
	id = cRender::GetInstance()->AddTexture(FileName, MipMap);
	return id != cRender::idNone;
} // cMaterial::Map::Load

// cMaterial.ctor
cMaterial::cMaterial() {
}

//---------------------------------------------------------------------------------------------------------------------------------
// cMaterial.ctor
//---------------------------------------------------------------------------------------------------------------------------------
cMaterial::cMaterial(const char *Name, const cColor &Ambient, const cColor &Diffuse, const cColor &Specular, const float Shininess)
: Name(Name), Ambient(Ambient), Diffuse(Diffuse), Specular(Specular), Shininess(Shininess) {
	for(int i = 0; i < 8; i++) {
		Tex[i].Unit = i;
	}
} // cMaterial.ctor

//-----------------------------------------------------------------------------
// cMaterial::Load
//-----------------------------------------------------------------------------
void cMaterial::Load() {
	for(int i = 0; i < 8; i++) {
		Tex[i].Load();
	}
} // cMaterial::Load

const cMaterial cMaterial::Brass("Brass",
								 cColor(0.329412f, 0.223529f, 0.027451f, 1.0f),
								 cColor(0.780392f, 0.568627f, 0.113725f, 1.0f),
								 cColor(0.992157f, 0.941176f, 0.807843f, 1.0f), 27.8974f);

const cMaterial cMaterial::Bronze("Bronze",
								  cColor(0.2125f, 0.1275f, 0.054f, 1.0f),
								  cColor(0.714f, 0.4284f, 0.18144f, 1.0f),
								  cColor(0.393548f, 0.271906f, 0.166721f, 1.0f), 25.6f);

const cMaterial cMaterial::PolishedBronze("Polished Bronze",
										  cColor(0.25f, 0.148f, 0.06475f, 1.0f),
										  cColor(0.4f, 0.2368f, 0.1036f, 1.0f),
										  cColor(0.774597f, 0.458561f, 0.200621f, 1.0f), 76.8f);

const cMaterial cMaterial::Chrome("Chrome",
								  cColor(0.25f, 0.25f, 0.25f, 1.0f),
								  cColor(0.4f, 0.4f, 0.4f, 1.0f),
								  cColor(0.774597f, 0.774597f, 0.774597f, 1.0f), 76.8f);

const cMaterial cMaterial::Copper("Copper",
								  cColor(0.19125f, 0.0735f, 0.0225f, 1.0f),
								  cColor(0.7038f, 0.27048f, 0.0828f, 1.0f),
								  cColor(0.256777f, 0.137622f, 0.086014f, 1.0f), 12.8f);

const cMaterial cMaterial::PolishedCopper("Polished Copper",
										  cColor(0.2295f, 0.08825f, 0.0275f, 1.0f),
										  cColor(0.5508f, 0.2118f, 0.066f, 1.0f),
										  cColor(0.580594f, 0.223257f, 0.0695701f, 1.0f), 51.2f);

const cMaterial cMaterial::Gold("Gold",
								cColor(0.24725f, 0.1995f, 0.0745f, 1.0f),
								cColor(0.75164f, 0.60648f, 0.22648f, 1.0f),
								cColor(0.628281f, 0.555802f, 0.366065f, 1.0f), 51.2f);

const cMaterial cMaterial::PolishedGold("Polished Gold",
										cColor(0.24725f, 0.2245f, 0.0645f, 1.0f),
										cColor(0.34615f, 0.3143f, 0.0903f, 1.0f),
										cColor(0.797357f, 0.723991f, 0.208006f, 1.0f), 83.2f);

const cMaterial cMaterial::Pewter("Pewter",
								  cColor(0.105882f, 0.058824f, 0.113725f, 1.0f),
								  cColor(0.427451f, 0.470588f, 0.541176f, 1.0f),
								  cColor(0.333333f, 0.333333f, 0.521569f, 1.0f), 9.84615f);

const cMaterial cMaterial::Silver("Silver",
								  cColor(0.19225f, 0.19225f, 0.19225f, 1.0f),
								  cColor(0.50754f, 0.50754f, 0.50754f, 1.0f),
								  cColor(0.508273f, 0.508273f, 0.508273f, 1.0f), 51.2f);

const cMaterial cMaterial::PolishedSilver("Polished Silver",
										  cColor(0.23125f, 0.23125f, 0.23125f, 1.0f),
										  cColor(0.2775f, 0.2775f, 0.2775f, 1.0f),
										  cColor(0.773911f, 0.773911f, 0.773911f, 1.0f), 89.6f);

const cMaterial cMaterial::Emerald("Emerald",
								   cColor(0.0215f, 0.1745f, 0.0215f, 0.55f),
								   cColor(0.07568f, 0.61424f, 0.07568f, 0.55f),
								   cColor(0.633f, 0.727811f, 0.633f, 0.55f), 76.8f);

const cMaterial cMaterial::Jade("Jade",
								cColor(0.135f, 0.2225f, 0.1575f, 0.95f),
								cColor(0.54f, 0.89f, 0.63f, 0.95f),
								cColor(0.316228f, 0.316228f, 0.316228f, 0.95f), 12.8f);

const cMaterial cMaterial::Obsidian("Obsidian",
									cColor(0.05375f, 0.05f, 0.06625f, 0.82f),
									cColor(0.18275f, 0.17f, 0.22525f, 0.82f),
									cColor(0.332741f, 0.328634f, 0.346435f, 0.82f), 38.4f);

const cMaterial cMaterial::Pearl("Pearl",
								 cColor(0.25f, 0.20725f, 0.20725f, 0.922f),
								 cColor(1.0f, 0.829f, 0.829f, 0.922f),
								 cColor(0.296648f, 0.296648f, 0.296648f, 0.922f), 11.264f);

const cMaterial cMaterial::Ruby("Ruby",
								cColor(0.1745f, 0.01175f, 0.01175f, 0.55f),
								cColor(0.61424f, 0.04136f, 0.04136f, 0.55f),
								cColor(0.727811f, 0.626959f, 0.626959f, 0.55f), 76.8f);

const cMaterial cMaterial::Turquoise("Turquoise",
									 cColor(0.1f, 0.18725f, 0.1745f, 0.8f),
									 cColor(0.396f, 0.74151f, 0.69102f, 0.8f),
									 cColor(0.297254f, 0.30829f, 0.306678f, 0.8f), 12.8f);

const cMaterial cMaterial::BlackPlastic("Black Plastic",
										cColor(0.0f, 0.0f, 0.0f, 1.0f),
										cColor(0.01f, 0.01f, 0.01f, 1.0f),
										cColor(0.50f, 0.50f, 0.50f, 1.0f), 32.0f);

const cMaterial cMaterial::BlackRubber("Black Rubber", 
									   cColor(0.02f, 0.02f, 0.02f, 1.0f),
									   cColor(0.01f, 0.01f, 0.01f, 1.0f),
									   cColor(0.4f, 0.4f, 0.4f, 1.0f), 10.0f);