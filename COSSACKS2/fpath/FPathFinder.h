#ifndef FPathFinderH
#define FPathFinderH

#include <assert.h>
#include "myManager.h"

struct FPoint {
   short x, y;
   FPoint* Next;
};

class FPathFinder {
	virtual bool fCheckPt(int x, int y)=0;
	virtual bool fbCheckPt(int x, int y)=0;
	virtual void fbBSetPt(int x, int y)=0;
	virtual void fbBClrPt(int x, int y)=0;
   // - работа с выделением элементов списка
   static myManager<FPoint> myPointsManager;
   #define newPoint() FPathFinder::myPointsManager.Allocate()
   #define deletePoint(p) FPathFinder::myPointsManager.Free(p)
   // - класс пути
   struct FPath;
   friend FPath;
   struct FPath {
      FPoint* First;
      FPoint* Last;
      int Len;
      inline void Add(int x, int y)
      {
         if(First == NULL)
            First = Last = newPoint();
         else
         {
            assert(Last);
            FPoint* p = newPoint();
            Last->Next = p;
            Last = p;
         }
         Last->x = x;
         Last->y = y;
         Last->Next = NULL;
         Len++;
      }
      inline void AddUnique(int x, int y)
      {
         if(First == NULL) Add(x, y);
         else
         {
            assert(Last);
            if(Last->x != x || Last->y != y) Add(x, y);
         }
      }
      inline void Null(void)
      {
         First = Last = NULL;
         Len = 0;
      }
      inline void Clear(void)
      {
         FPoint* p = First;
         while(p)
         {
            deletePoint(p);
            p = p->Next;
         }
         Null();
      }
      FPath(void)
      {
         Null();
      }
      ~FPath(void)
      {
      }
   };
   FPoint ptFrom;
   FPoint ptTo;
   FPath  PathL;
   FPath  PathR;
   FPath  LinePath;
/*
   int  LayLine(FPoint* _ptFrom, FPoint* _ptTo);
   bool CheckLine(FPoint* _ptFrom, FPoint* _ptTo);
   int  MakeLine(FPoint* _ptFrom, FPoint* _ptTo, FPath* Path);
   int  VectorsOptimize(void);
   int  VectorsOptimize1(void);   
*/
	#include "FPathFinderLines.h"
	int  GoAround(int x_p, int y_p, int x_w, int y_w);

public:
	FPath  MainPath;
	FPath  WayPoints;
	bool isOptimize;
	FPoint* GetPath(int x0, int y0, int x1, int y1);
	void Bending(int val);

	FPathFinder()
	{
		isOptimize = true;
	}
};

#endif
