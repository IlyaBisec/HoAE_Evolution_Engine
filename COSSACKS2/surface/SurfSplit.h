#ifndef __SurfSplit__
#define __SurfSplit__

namespace Surface {

#define VERTEX VertexUV3C
#define vfVERTEX vfVertexUV3C

	class SurfSplit {
	public:
		void Free();
		~SurfSplit() { Free(); }
		void Split(const SurfRef &Ref, eSurfFx SFX = (eSurfFx)-1);
		void Draw();
		class Cache : public cList<BaseMesh*> {
		public:
			enum ModifyFlags {
				MF_OFFSET = 1 << 0,
				MF_AND = 1 << 1,
				MF_OR = 1 << 2,
				MF_SEA = 1 << 3,
				MF_TEXTURE = 1 << 4,
				MF_SUM = 1 << 5,
				MF_LAYER = 1 << 6,
			};
			struct ModifyArgs {
				DWORD Flags;
				SurfVertex Offset; // MF_OFFSET
				DWORD DiffuseAndFlag; // MF_AND
				DWORD DiffuseOrFlag; // MF_OR
				int idTexture; // MF_TEXTURE
				DWORD DiffuseSum; // MF_SUM
			};
			void Modify(const ModifyArgs &Args);
			static void Modify(BaseMesh &bm, const ModifyArgs &Args);
		} CACHE;
	private:
		void toCache(const SurfRef& ref,int fKey, eSurfFx SFX);
	};

} // Surface

#endif // __SurfSplit__