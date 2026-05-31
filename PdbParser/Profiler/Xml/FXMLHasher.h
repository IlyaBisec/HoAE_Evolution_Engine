#ifndef FXMLHasherH
#define FXMLHasherH

#include "FXMLCommon.h"
//---------------------------------------------------------------------------
class FXMLHasher {
   static unsigned TTT[256];
public:
   enum { 
		MAXHASH = 0xFFFF,
		UNIT_MAXWORDS = 256
	};

   struct UNIT {
      unsigned Id;
      unsigned NameLen;
      unsigned NameOffset;
      e_fxmlKind Kind;
      e_fxmlType Type;
      UNIT* Next;

      UNIT()
      {
         Kind = FXML_KIND_NONE;
         Type = FXML_TYPE_NONE;
         Next = NULL;
      }

      ~UNIT()
      {
         if(Next) delete Next;
         Next = NULL;
      }
   };

   bool isDictionaryUpdate;
   FOutStream Dictionary;
   FOutStream LinearUnitsPtrs;
   UNIT* HashTable[MAXHASH];
   int UnitsNumber;

   static inline unsigned HASH(const char* Name, unsigned NameLen)
   {
      unsigned h = 0;
      for(unsigned i = 0; i < NameLen; i++)
         h ^= FXMLHasher::TTT[Name[i]];
      assert(h < MAXHASH);
      return h;
   }

   UNIT** Find(const char* Name, unsigned NameLen, e_fxmlKind Kind, e_fxmlType Type);
   UNIT* Find(int Id);
   UNIT* Add(UNIT** ppU, const char* Name, unsigned NameLen, e_fxmlKind Kind, e_fxmlType Type);
   UNIT* Put(const char* Name, unsigned NameLen, e_fxmlKind Kind, e_fxmlType Type);

   char  GetFirstChar(UNIT* pU);
   void  PutToTextStream(UNIT* pU, FOutStream* Out);
   UNIT* GetFromTextStream(FInStream* In);

   bool LoadDictionary(char* FileName);
   bool SaveDictionary(char* FileName);
   bool Initialize(void);

   FXMLHasher()
   {
      isDictionaryUpdate = !true;
      memset(HashTable, 0, MAXHASH*sizeof(HashTable[0]));
      UnitsNumber = 0;
      srand(0x12345678);
      for(int i = 0; i < 256; i++)
         TTT[i] = rand()%MAXHASH;
   }

   ~FXMLHasher()
   {
      if(isDictionaryUpdate) SaveDictionary("!Dictionary!.!!!");

      for(int i = 0; i < MAXHASH; i++)
         if(HashTable[i]) delete HashTable[i];
   }
};
//---------------------------------------------------------------------------

#endif
