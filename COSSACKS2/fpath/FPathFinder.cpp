//---------------------------------------------------------------------------
#include "stdheader.h"
#include <math.h>
#include "FPathFinder.h"
//---------------------------------------------------------------------------
myManager<FPoint> FPathFinder::myPointsManager;
//---------------------------------------------------------------------------
// - ����� �����������, ��������� _��_ ������������ ����� ���� � �����,
// � ���� ��������� �����, �������� ���������, ������� ������������
//---------------------------------------------------------------------------
int FPathFinder::GoAround(int x_p, int y_p, int x_w, int y_w)
{
   int x_pL = x_p, y_pL = y_p, x_wL = x_w, y_wL = y_w;
   int x_pR = x_p, y_pR = y_p, x_wR = x_w, y_wR = y_w;
   int x1L, y1L, x2L, y2L, p_lenL = 0;
   int x1R, y1R, x2R, y2R, p_lenR = 0;
   int dxL = 2, dyL = 2, dxR = 2, dyR = 2;

	// - �������� ��������� �������
   int x_p_start = x_p;
   int y_p_start = y_p;
   int x_w_start = x_w;
   int y_w_start = y_w;

   #define checkStartPos(_) (x_p##_ == x_p_start && y_p##_ == y_p_start && \
                          	  x_w##_ == x_w_start && y_w##_ == y_w_start)

   // - ���������� ��������� ������� ����� �����
   #define assignWallCell(x, y, _) {\
   	x_w##_ = x; y_w##_ = y;\
      /* - ����� ��� ������������ */\
      if(checkStartPos(_)) return -1;\
   }
   #define AROUND_VECTORS_OPTIMIZE

   #ifndef AROUND_VECTORS_OPTIMIZE
      // - ���������� ��������� ������� ����� ����
      #define assignPathCell(x, y, _) {\
   	   x_p##_ = x; y_p##_ = y;\
         /* - ����� ��� ������������ */\
         if(checkStartPos(_)) return -1;\
         /* - �������� ����� ���� */\
         Path##_.Add(x_p##_, y_p##_);\
  	   	p_len##_++;\
		   /* - ���� ������� ����� ��� �������� �����, �� �� ������ ����������� */\
         if(fbCheckPt(x_p##_, y_p##_)) break;\
      }
   #else
      // - ���������� ��������� ������� ����� ����
      #define assignPathCell(x, y, _) {\
         /* - ������ ����������� */\
         int _dx = x_p##_ - x;\
         int _dy = y_p##_ - y;\
         if(dx##_ != _dx || dy##_ != _dy)\
         {\
            dx##_ = _dx;\
            dy##_ = _dy;\
            Path##_.Add(x_p##_, y_p##_);\
     	   	p_len##_++;\
         }\
      	x_p##_ = x; y_p##_ = y;\
         /* - ����� ��� ������������ */\
         if(checkStartPos(_)) return -1;\
   		/* - ���� ������� ����� ��� �������� �����, �� �� ������ ����������� */\
         if(fbCheckPt(x_p##_, y_p##_))\
         {\
            Path##_.Add(x_p##_, y_p##_);\
       		p_len##_++;\
            break;\
         }\
      }
   #endif

   #define assignNextPoints(_) { \
      /* - ��������� �� ������� ������ ���� - �����? */\
  		if(!fCheckPt(x1##_, y1##_))\
      {\
         /* - "��������������" �� ����� */\
         assignWallCell(x1##_, y1##_, _);\
      }\
      else\
      {\
         /* - ��������� �� ������ ����� - �����? */\
         if(!fCheckPt(x2##_, y2##_))\
     	   {\
            /* - ������������ �� ��� ������ ����� ������ ����� */\
            assignWallCell(x2##_, y2##_, _);\
            /* - ��������� �� ������� ������ ���� - ���� */\
            assignPathCell(x1##_, y1##_, _);\
     	   }\
         else\
         {\
			/* - ���� ������� ����� ��� �������� �����, �� �� ������ ����������� */\
			if(fbCheckPt(x1##_, y1##_))\
				assignPathCell(x1##_, y1##_, _);\
            /* - ��������� �� ������ ����� - ����, ������� ������� */\
            assignPathCell(x2##_, y2##_, _);\
         }\
      }\
   }

   bool isLeft = true;
	while(1)
   {
		// - ����� �����, ����������� ��������� ����� ���� � �����
  		x1L = x_pL + y_wL - y_pL;
  		y1L = y_pL + x_pL - x_wL;

  		x2L = x_wL + y_wL - y_pL;
  		y2L = y_wL + x_pL - x_wL;

		// - ����� ������, ����������� ��������� ����� ���� � �����
 		x1R = x_pR - (y_wR - y_pR);
     	y1R = y_pR - (x_pR - x_wR);

  		x2R = x_wR - (y_wR - y_pR);
     	y2R = y_wR - (x_pR - x_wR);

      // - �����
      isLeft = true;
      assignNextPoints(L);

      // - ������
      isLeft = false;
      assignNextPoints(R);
	}

   if(isLeft)
   {
      // - ������� ����� �����
      assert(fbCheckPt(x_pL, y_pL));
      PathR.Clear();
      return 0;
   }
   // - ����� ������
   assert(fbCheckPt(x_pR, y_pR));
   PathL.Clear();
 	return 1;
}
//---------------------------------------------------------------------------
FPoint* FPathFinder::GetPath(int x0, int y0, int x1, int y1)
{
   ptFrom.x = x0;
   ptFrom.y = y0;
   ptTo.x = x1;
   ptTo.y = y1;

   WayPoints.Clear();

   // - �������� ������ � ��������� �����, �� ������ ���� �����
   if(!fCheckPt(x0, y0) || !fCheckPt(x1, y1)) return NULL;

   // - ������ way-point
   WayPoints.Add(x0, y0);

   // - �������� ������ ����� ����� �������
   if(LayLine(&ptFrom, &ptTo) != 0)
   {
      // - ������ ����� - ����-�����-����(����� �� �����)
      FPoint* p0 = LinePath.First;
      if(p0 == NULL) return NULL;
      FPoint* p1 = p0->Next;
      assert(p1);
      FPoint* p2 = p1->Next;
      assert(p2);

      // - ���� ����� ����������� �����
      while(p0)
      {
         assert(p1);
         assert(p2);
         // - ����� �� ��� ����� �� ���
         int x0 = p0->x;
         int y0 = p0->y;
         int x1 = p1->x;
         int y1 = p1->y;
         int x2 = p2->x;
         int y2 = p2->y;

         // - ������ ���� ������ "����-�����-����"
         if(!(fCheckPt(x0, y0) && !fCheckPt(x1, y1) && fCheckPt(x2, y2)))
            assert(fCheckPt(x0, y0) && !fCheckPt(x1, y1) && fCheckPt(x2, y2));

         // - ��������� ����� � ����
         WayPoints.AddUnique(x0, y0);

       	// - ������������� ��������� ����� ���� � �����
         if(!fCheckPt(x0, y1)) x1 = x0;
    	   else y0 = y1;

         // - ��������, ��� ����� �������
         WayPoints.AddUnique(x0, y0);
         p0->x = x0;
         p0->y = y0;

         PathL.Clear();
         PathR.Clear();
         int LenR = 0, LenL = 0;
         int Res = GoAround(x0, y0, x1, y1);
         LenL = PathL.Len;
         LenR = PathR.Len;

         // - ���� ��������?
         if(Res == -1)
         {
            // - ������� �����
            while(p0)
            {
               assert(p1);
               assert(p2);
               // - ������� �����
               fbBClrPt(p2->x, p2->y);
               // - ����� ��������� ��� �����
               p0 = p2->Next;
               if(p0 == NULL) break;
               p1 = p0->Next;
               assert(p1);
               p2 = p1->Next;
               assert(p2);
            }
            LinePath.Clear();
            MainPath.Clear();
            WayPoints.Clear();
            PathL.Clear();
            PathR.Clear();
            return NULL;
         }

         // - ����� ������ �� ������ ���� �������
         if(!(LenR || LenL))
            assert(LenR || LenL);

         // - ���� ������� ������ ����
         if(LenR != 0)
         {
            // - "���������" ����
            WayPoints.Last->Next = PathR.First;
            WayPoints.Last = PathR.Last;
            WayPoints.Len += PathR.Len;
            PathR.Null();
            PathL.Clear();
         }
         else
         {
            // - "���������" ����
            WayPoints.Last->Next = PathL.First;
            WayPoints.Last = PathL.Last;
            WayPoints.Len += PathL.Len;
            PathL.Null();
            PathR.Clear();
         }

         // - ������� ���������� ����� ��������
         int x = WayPoints.Last->x;
         int y = WayPoints.Last->y;

         // - ���� ����� ��������, ������� � �������
         while(p0)
         {
            assert(p1);
            assert(p2);

            int _x = p2->x;
            int _y = p2->y;

            // - ������� ���� �� ������� �����
            fbBClrPt(_x, _y);

            if(_x == x && _y == y) break;

            // - ����� ��������� �����
            p0 = p2->Next;
            if(p0 == NULL) break;
            p1 = p0->Next;
            assert(p1);
            p2 = p1->Next;
            assert(p2);
         }
         if(p0 == NULL) break;
         assert(p1);
         assert(p2);

         // - ����� ��������� �����
         p0 = p2->Next;
         if(p0 == NULL) break;
         p1 = p0->Next;
         assert(p1);
         p2 = p1->Next;
         assert(p2);
      }
   }
   // - ��������� ��������� ����� � ����
   WayPoints.AddUnique(ptTo.x, ptTo.y);

   // - ����� ������ �������� ������ ��� ���������� ���������

   /*for(int j = 0; j < 1024; j++)
   {
      for(int i = 0; i < 1024; i++)
      {
         if(fbCheckPt(i,j))
         {
            assert(!fbCheckPt(i,j));
         }
      }
   }*/

//   memset(_bmap, 0, sizeof(_bmap));

   if(isOptimize && WayPoints.Len > 2)
   {
//      while(VectorsOptimize());
      VectorsOptimize1();
//      Bending(3);
   }

   // - ������������� ���������� ����
   MainPath.Clear();
   FPoint* wp = WayPoints.First;
   while(wp && wp->Next)
   {
      MakeLine(wp, wp->Next, &MainPath);
      wp = wp->Next;
   }
   return MainPath.First;
}
//---------------------------------------------------------------------------
int Norma(int x, int y);
void FPathFinder::Bending(int val)
{
   FPoint* p0 = WayPoints.First;
   if(p0 == NULL) return;
   FPoint* p1 = p0->Next;
   if(p1 == NULL) return;
   FPoint* p2 = p1->Next;

   while(p0 && p1 && p2)
   {
      int ax = p0->x - p1->x;
      int ay = p0->y - p1->y;
      int da = Norma(ax, ay);
		if(da == 0) da = 1;
      ax /= da;
      ay /= da;

      int bx = p2->x - p1->x;
      int by = p2->y - p1->y;
      int db = Norma(bx, by);
		if(db == 0) db = 1;
      bx /= db;
      by /= db;

		int nx = -ax - bx;
      int ny = -ay - by;
      int dn = Norma(nx, ny);
      if(dn == 0) dn = 1;
      nx /= dn;
      ny /= dn;

		// - �������� ���������� ����� ������� �������
      FPoint pt;
      pt.x = p1->x + val*nx;
      pt.y = p1->y + val*ny;

      // - �������� ����� �� ����� ����� � ������
      FPath Path;
      MakeLine(&pt, p1, &Path);

      FPoint* p = Path.First;
      while(p)
      {
         if(fCheckPt(p->x, p->y) && CheckLine(p, p0) && CheckLine(p, p2))
            break;
         p = p->Next;
      }
      if(p)
      {
         p1->x = p->x;
         p1->y = p->y;
      }
      p0 = p1;
      p1 = p0->Next;
      p2 = p1->Next;
   }
}
//---------------------------------------------------------------------------

