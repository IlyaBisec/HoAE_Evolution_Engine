#pragma once

//*****************************************************************************
// cVertex
//*****************************************************************************

class cVertex {
public:
	struct Usage {
		static const int Undefined = 0;
		static const int Position = 1;				// Type = Float, Dim = 3
		static const int PositionRhw = 2;			// Type = Float, Dim = 4
		static const int Normal = 3;				// Type = Float, Dim = 3
		static const int TexCoord = 4;				// Type = Float, Dim = 2
		static const int Tangent = 5;				// Type = Float, Dim = 3
		static const int BiTangent = 6;				// Type = Float, Dim = 3
		static const int Color = 7;					// Type = Byte, Dim = 4
		static const int SecondaryColor = 8;		// Type = Byte, Dim = 4
		static const int FogCoord = 9;				// Type = Float, Dim = 1

		static bool IsValid(const int Usage) {
			return Usage >= Undefined && Usage <= FogCoord;
		}
	};

	struct Type {
		static const int Float = 0;
		static const int Byte = 1;

		static bool IsValid(const int Type) {
			return Type >= Float && Type <= Byte;
		}

		//---------------------------------------------------------------------
		// SizeOf
		//---------------------------------------------------------------------
		static int SizeOf(const int Type) {
			cAssert(IsValid(Type));
			if(!IsValid(Type)) {
				return 0;
			}
			
			switch(Type) {
				case Float: return 4;
				case Byte: return 1;
				default: return 0;
			}
		}
	};

	struct Tag {
		static const int Vertex = 0;			// Type = Float, Dim = { 2, 3, 4 }
		static const int Normal = 1;			// Type = Float, Dim = 3
		static const int Color = 2;				// Type = { Float, Byte }, Dim = { 3, 4 }
		static const int SecondaryColor = 3;
		static const int MultiTexCoord0 = 4;	// Type = Float, Dim = { 1, 2, 3, 4 }
		static const int MultiTexCoord1 = 5;
		static const int MultiTexCoord2 = 6;
		static const int MultiTexCoord3 = 7;
		static const int MultiTexCoord4 = 8;
		static const int MultiTexCoord5 = 9;
		static const int MultiTexCoord6 = 10;
		static const int MultiTexCoord7 = 11;
		static const int FogCoord = 12;			// Type = Float, Dim = 1

		static bool IsValid(const int Tag) {
			return Tag >= Vertex && Tag <= FogCoord;
		}
	};
	
	//-------------------------------------------------------------------------
	// FieldIsValid
	//-------------------------------------------------------------------------
	static bool FieldIsValid(const int Type, const int Dim, const int Tag) {
		cAssert(cVertex::Type::IsValid(Type));
		if(!cVertex::Type::IsValid(Type)) {
			return false;
		}
		cAssert(cVertex::Tag::IsValid(Tag));
		if(!cVertex::Tag::IsValid(Tag)) {
			return false;
		}
		switch(Tag) {
			case cVertex::Tag::Vertex: return Type == cVertex::Type::Float && Dim >= 2 && Dim <= 4;
			case cVertex::Tag::Normal: return Type == cVertex::Type::Float && Dim == 3;
			case cVertex::Tag::Color:
			case cVertex::Tag::SecondaryColor:
				return (Type == cVertex::Type::Float || Type == cVertex::Type::Byte) && Dim >= 3 && Dim <= 4;
			case cVertex::Tag::MultiTexCoord0:
			case cVertex::Tag::MultiTexCoord1:
			case cVertex::Tag::MultiTexCoord2:
			case cVertex::Tag::MultiTexCoord3:
			case cVertex::Tag::MultiTexCoord4:
			case cVertex::Tag::MultiTexCoord5:
			case cVertex::Tag::MultiTexCoord6:
			case cVertex::Tag::MultiTexCoord7: return Type == cVertex::Type::Float && Dim >= 1 && Dim <= 4;
			case cVertex::Tag::FogCoord: return Type == cVertex::Type::Float && Dim == 1;
			default: return false;
		}
	} // FieldIsValid
	
	struct Format {
		cList<cVec4i> Fields; // Usage, Type, Dim, Tag
		bool IsTransformed;

		Format() : IsTransformed(false) {}

		int GetSize() const {
			int Size = 0;
			for(int f = 0; f < Fields.Count(); f++) {
				Size += Type::SizeOf(Fields[f][1]) * Fields[f][2];
			}
			return Size;
		}

		//---------------------------------------------------------------------
		// HasUsage
		//---------------------------------------------------------------------
		bool HasUsage(const int Usage) const {
			cAssert(cVertex::Usage::IsValid(Usage));
			if(!cVertex::Usage::IsValid(Usage)) {
				return false;
			}

			for(int f = 0; f < Fields.Count(); f++) {
				if(Usage == Fields[f][0]) {
					return true;
				}
			}
			return false;
		} // HasUsage
	};

	//-------------------------------------------------------------------------
	// CustomVertex
	//-------------------------------------------------------------------------
	class CustomVertex {
	public:
		cVec3 * GetPosition() { return NULL; }
		cVec4 * GetPositionRhw() { return NULL; }
		cVec3 * GetNormal() { return NULL; }
		cVec2 * GetTexCoord() { return NULL; }
		cVec3 * GetTangent() { return NULL; }
		cVec3 * GetBiTangent() { return NULL; }
		dword * GetColor() { return NULL; }
		dword * GetSecondaryColor() { return NULL; }
		float * GetFogCoord() { return NULL; }
	};
	
	//---------------------------------------------------------------------
	// PositionColored
	//---------------------------------------------------------------------
	class PositionColored : public CustomVertex {
	public:
		cVec3 Pos;
		dword Color;
		
		cVec3 * GetPosition() { return &Pos; }
		dword * GetColor() { return &Color; }
		
		static const Format GetFormat() {
			Format F;
			F.Fields.Add(cVec4i(Usage::Position, Type::Float, 3, Tag::Vertex));
			F.Fields.Add(cVec4i(Usage::Color, Type::Byte, 4, Tag::Color));
			return F;
		}
	};
	
	//---------------------------------------------------------------------
	// PositionColoredTextured
	//---------------------------------------------------------------------
	class PositionColoredTextured : public CustomVertex {
	public:
		cVec3 Pos;
		dword Color;
		cVec2 TexCoord;

		cVec3 * GetPosition() { return &Pos; }
		dword * GetColor() { return &Color; }
		cVec2 * GetTexCoord() { return &TexCoord; }
		
		static const Format GetFormat() {
			Format F;
			F.Fields.Add(cVec4i(Usage::Position, Type::Float, 3, Tag::Vertex));
			F.Fields.Add(cVec4i(Usage::Color, Type::Byte, 4, Tag::Color));
			F.Fields.Add(cVec4i(Usage::TexCoord, Type::Float, 2, Tag::MultiTexCoord0));
			return F;
		}
	};
	
	//---------------------------------------------------------------------
	// PositionNormal
	//---------------------------------------------------------------------
	class PositionNormal : public CustomVertex {
	public:
		cVec3 Pos;
		cVec3 Normal;

		cVec3 * GetPosition() { return &Pos; }
		cVec3 * GetNormal() { return &Normal; }
		
		static const Format GetFormat() {
			Format F;
			F.Fields.Add(cVec4i(Usage::Position, Type::Float, 3, Tag::Vertex));
			F.Fields.Add(cVec4i(Usage::Normal, Type::Float, 3, Tag::Normal));
			return F;
		}
	};
	
	//---------------------------------------------------------------------
	// PositionNormalColored
	//---------------------------------------------------------------------
	class PositionNormalColored : public CustomVertex {
	public:
		cVec3 Pos;
		cVec3 Normal;
		dword Color;

		cVec3 * GetPosition() { return &Pos; }
		cVec3 * GetNormal() { return &Normal; }
		dword * GetColor() { return &Color; }
		
		static const Format GetFormat() {
			Format F;
			F.Fields.Add(cVec4i(Usage::Position, Type::Float, 3, Tag::Vertex));
			F.Fields.Add(cVec4i(Usage::Normal, Type::Float, 3, Tag::Normal));
			F.Fields.Add(cVec4i(Usage::Color, Type::Byte, 4, Tag::Color));
			return F;
		}
	};
	
	//---------------------------------------------------------------------
	// PositionNormalTextured
	//---------------------------------------------------------------------
	class PositionNormalTextured : public CustomVertex {
	public:
		cVec3 Pos;
		cVec3 Normal;
		cVec2 TexCoord;

		cVec3 * GetPosition() { return &Pos; }
		cVec3 * GetNormal() { return &Normal; }
		cVec2 * GetTexCoord() { return &TexCoord; }
		
		static const Format GetFormat() {
			Format F;
			F.Fields.Add(cVec4i(Usage::Position, Type::Float, 3, Tag::Vertex));
			F.Fields.Add(cVec4i(Usage::Normal, Type::Float, 3, Tag::Normal));
			F.Fields.Add(cVec4i(Usage::TexCoord, Type::Float, 2, Tag::MultiTexCoord0));
			return F;
		}
	};
	
	//---------------------------------------------------------------------
	// PositionOnly
	//---------------------------------------------------------------------
	class PositionOnly : public CustomVertex {
	public:
		cVec3 Pos;

		cVec3 * GetPosition() { return &Pos; }
		
		static const Format GetFormat() {
			Format F;
			F.Fields.Add(cVec4i(Usage::Position, Type::Float, 3, Tag::Vertex));
			return F;
		}
	};

	//-------------------------------------------------------------------------
	// PositionRhw
	//-------------------------------------------------------------------------
	class PositionRhw : public CustomVertex {
	public:
		cVec4 Pos;

		cVec4 * GetPositionRhw() { return &Pos; }

		static const Format GetFormat() {
			Format F;
			F.Fields.Add(cVec4i(Usage::PositionRhw, Type::Float, 4, Tag::Vertex));
			return F;
		}
	};
	
	//---------------------------------------------------------------------
	// PositionTextured
	//---------------------------------------------------------------------
	class PositionTextured : public CustomVertex {
	public:
		cVec3 Pos;
		cVec2 TexCoord;

		cVec3 * GetPosition() { return &Pos; }
		cVec2 * GetTexCoord() { return &TexCoord; }
		
		static const Format GetFormat() {
			Format F;
			F.Fields.Add(cVec4i(Usage::Position, Type::Float, 3, Tag::Vertex));
			F.Fields.Add(cVec4i(Usage::TexCoord, Type::Float, 2, Tag::MultiTexCoord0));
			return F;
		}
	};
	
	//---------------------------------------------------------------------
	// Transformed
	//---------------------------------------------------------------------
	class Transformed : public CustomVertex {
	public:
		cVec4 Pos;

		cVec4 * GetPositionRhw() { return &Pos; }
		
		static const Format GetFormat() {
			Format F;
			F.IsTransformed = true;
			F.Fields.Add(cVec4i(Usage::PositionRhw, Type::Float, 4, Tag::Vertex));
			return F;
		}
	};
	
	//---------------------------------------------------------------------
	// TransformedColored
	//---------------------------------------------------------------------
	class TransformedColored : public CustomVertex {
	public:
		cVec4 Pos;
		dword Color;

		cVec4 * GetPositionRhw() { return &Pos; }
		dword * GetColor() { return &Color; }
		
		static const Format GetFormat() {
			Format F;
			F.IsTransformed = true;
			F.Fields.Add(cVec4i(Usage::PositionRhw, Type::Float, 4, Tag::Vertex));
			F.Fields.Add(cVec4i(Usage::Color, Type::Byte, 4, Tag::Color));
			return F;
		}
	};
	
	//---------------------------------------------------------------------
	// TransformedColoredTextured
	//---------------------------------------------------------------------
	class TransformedColoredTextured : public CustomVertex {
	public:
		cVec4 Pos;
		dword Color;
		cVec2 TexCoord;

		cVec4 * GetPositionRhw() { return &Pos; }
		dword * GetColor() { return &Color; }
		cVec2 * GetTexCoord() { return &TexCoord; }
		
		static const Format GetFormat() {
			Format F;
			F.IsTransformed = true;
			F.Fields.Add(cVec4i(Usage::PositionRhw, Type::Float, 4, Tag::Vertex));
			F.Fields.Add(cVec4i(Usage::Color, Type::Byte, 4, Tag::Color));
			F.Fields.Add(cVec4i(Usage::TexCoord, Type::Float, 2, Tag::MultiTexCoord0));
			return F;
		}
	};
	
	//---------------------------------------------------------------------
	// TransformedTextured
	//---------------------------------------------------------------------
	class TransformedTextured : public CustomVertex {
	public:
		cVec4 Pos;
		cVec2 TexCoord;

		cVec4 * GetPositionRhw() { return &Pos; }
		cVec2 * GetTexCoord() { return &TexCoord; }
		
		static const Format GetFormat() {
			Format F;
			F.IsTransformed = true;
			F.Fields.Add(cVec4i(Usage::PositionRhw, Type::Float, 4, Tag::Vertex));
			F.Fields.Add(cVec4i(Usage::TexCoord, Type::Float, 2, Tag::MultiTexCoord0));
			return F;
		}
	};
	
	//-------------------------------------------------------------------------
	// Bump
	//-------------------------------------------------------------------------
	class Bump : public CustomVertex {
	public:
		cVec3 Pos;
		dword Color;
		cVec3 Normal;
		cVec2 TexCoord;
		cVec3 Tangent;
		cVec3 BiTangent;

		cVec3 * GetPosition() { return &Pos; }
		dword * GetColor() { return &Color; }
		cVec3 * GetNormal() { return &Normal; }
		cVec2 * GetTexCoord() { return &TexCoord; }
		cVec3 * GetTangent() { return &Tangent; }
		cVec3 * GetBiTangent() { return &BiTangent; }
		
		static const Format GetFormat() {
			Format F;
			F.Fields.Add(cVec4i(Usage::Position, Type::Float, 3, Tag::Vertex));
			F.Fields.Add(cVec4i(Usage::Color, Type::Byte, 4, Tag::Color));
			F.Fields.Add(cVec4i(Usage::Normal, Type::Float, 3, Tag::Normal));
			F.Fields.Add(cVec4i(Usage::TexCoord, Type::Float, 2, Tag::MultiTexCoord0));
			F.Fields.Add(cVec4i(Usage::Tangent, Type::Float, 3, Tag::MultiTexCoord1));
			F.Fields.Add(cVec4i(Usage::BiTangent, Type::Float, 3, Tag::MultiTexCoord2));
			return F;
		}
	};
};