//---------------------------------------------------------------------------
#include <math.h>
#include "FPathFinder.h"
//---------------------------------------------------------------------------
int FPathFinder::LayLine(FPoint* _ptFrom, FPoint* _ptTo)
{
   LinePath.Clear();

a10:
   int xn = _ptFrom->x, yn = _ptFrom->y, xk = _ptTo->x, yk = _ptTo->y;
   int pxn = xn, pyn = yn;
   int dx, dy, s, sx, sy, kl, swap, incr1, incr2;

   #define PutPixLn(xn, yn) {\
      if(!fCheckPt(pxn, pyn) && fCheckPt(xn, yn))\
      {\
         /* - отмечаем точку выхода из стены */\
         LinePath.Add(xn, yn);\
         fbBSetPt(xn, yn);\
         _ptFrom->x = xn;\
         _ptFrom->y = yn;\
         goto a10;\
      }\
      if(fCheckPt(pxn, pyn) && !fCheckPt(xn, yn))\
      {\
         /* - отмечаем пары точек путь-стена */\
         LinePath.Add(pxn, pyn);\
         LinePath.Add(xn, yn);\
      }\
      pxn = xn;\
      pyn = yn;\
   }

   // - вычисление приращений и шагов
   sx = 0;
   dx = xk - xn;
   if(dx < 0)
   {
      dx = -dx;
      sx--;
   }
   else
   if(dx > 0) sx++;
   sy = 0;
   dy = yk - yn;
   if(dy < 0)
   {
      dy = -dy;
      sy--;
   }
   else
   if(dy > 0) sy++;

   // - учет наклона
   swap = 0;
   kl = dx;
   s = dy;
   if(kl < s)
   {
      dx = s;
      dy = kl;
      kl = s;
      swap++;
   }
   incr1 = dy << 1;
   s = incr1 - dx;
   incr2 = dx << 1;

   PutPixLn(xn, yn); // - первая точка вектора
   while(--kl >= 0)
   {
      if(s >= 0)
      {
         if(swap) xn += sx;
         else yn += sy;
         s -= incr2;
      }
      if(swap) yn += sy;
      else xn += sx;
      s += incr1;
      PutPixLn(xn, yn); // - текущая точка вектора
   }
   #undef PutPixLn
   return LinePath.Len;
}
//---------------------------------------------------------------------------
bool FPathFinder::CheckLine(FPoint* _ptFrom, FPoint* _ptTo)
{
   int xn = _ptFrom->x, yn = _ptFrom->y, xk = _ptTo->x, yk = _ptTo->y;
   int dx, dy, s, sx, sy, kl, swap, incr1, incr2;

   // - вычисление приращений и шагов
   sx = 0;
   dx = xk - xn;
   if(dx < 0)
   {
      dx = -dx;
      sx--;
   }
   else
   if(dx > 0) sx++;
   sy = 0;
   dy = yk - yn;
   if(dy < 0)
   {
      dy = -dy;
      sy--;
   }
   else
   if(dy > 0) sy++;

   // - учет наклона
   swap = 0;
   kl = dx;
   s = dy;
   if(kl < s)
   {
      dx = s;
      dy = kl;
      kl = s;
      swap++;
   }
   incr1 = dy << 1;
   s = incr1 - dx;
   incr2 = dx << 1;

   if(!fCheckPt(xn, yn)) return false;

   while(--kl >= 0)
   {
      if(s >= 0)
      {
         if(swap) xn += sx;
         else yn += sy;
         s -= incr2;
      }
      if(swap) yn += sy;
      else xn += sx;
      s += incr1;
      if(!fCheckPt(xn, yn)) return false;
   }
   return true;
}
//---------------------------------------------------------------------------
int FPathFinder::MakeLine(FPoint* _ptFrom, FPoint* _ptTo, FPath* Path)
{
   int xn = _ptFrom->x, yn = _ptFrom->y, xk = _ptTo->x, yk = _ptTo->y;
   int dx, dy, s, sx, sy, kl, swap, incr1, incr2, Len = 0;

   #define PutPixLn(xn, yn) {\
      Path->Add(xn, yn);\
      Len++;\
   }

   // - вычисление приращений и шагов
   sx = 0;
   dx = xk - xn;
   if(dx < 0)
   {
      dx = -dx;
      sx--;
   }
   else
   if(dx > 0) sx++;
   sy = 0;
   dy = yk - yn;
   if(dy < 0)
   {
      dy = -dy;
      sy--;
   }
   else
   if(dy > 0) sy++;

   // - учет наклона
   swap = 0;
   kl = dx;
   s = dy;
   if(kl < s)
   {
      dx = s;
      dy = kl;
      kl = s;
      swap++;
   }
   incr1 = dy << 1;
   s = incr1 - dx;
   incr2 = dx << 1;

   PutPixLn(xn, yn); // - первая точка вектора
   while(--kl >= 0)
   {
      if(s >= 0)
      {
         if(swap) xn += sx;
         else yn += sy;
         s -= incr2;
      }
      if(swap) yn += sy;
      else xn += sx;
      s += incr1;
      PutPixLn(xn, yn); // - текущая точка вектора
   }
   #undef PutPixLn
   return Len;
}
//---------------------------------------------------------------------------
int FPathFinder::VectorsOptimize(void)
{
   int RemovedNumber = 0;

   // - начальная точка
   FPoint* p0 = WayPoints.First;
   if(p0 == NULL) return 0;
   FPoint* p1 = p0->Next;
   if(p1 == NULL) return 0;
   FPoint* p2 = p1->Next;
   if(p2 == NULL) return 0;

   while(p0 && p1 && p2)
   {
      // - есть ли путь между двумя точками?
      if(CheckLine(p0, p2))
      {
         // - удаляем p1
         p0->Next = p2;
         deletePoint(p1);
         RemovedNumber++;
      }
      p0 = p2;
      p1 = p0->Next;
      if(p1 == NULL) break;
      p2 = p1->Next;
   }
   return RemovedNumber;
}
//---------------------------------------------------------------------------
int FPathFinder::VectorsOptimize1(void)
{
   int RemovedNumber = 0;

   // - начальная точка
   FPoint* wp0 = WayPoints.First;
   if(wp0 == NULL) return 0;

   while(wp0)
   {
      // - рабочая точка
      FPoint* wp = wp0->Next;
      while(wp)
      {
         // - есть ли путь между двумя точками?
         if(CheckLine(wp0, wp))
         {
            // - удаляем все промежуточные точки
            FPoint* _wp = wp0->Next;
            while(_wp != wp)
            {
               FPoint* __wp = _wp;
               // - указываем на следующую за удаляемой точкой
               wp0->Next = _wp->Next;
               _wp = _wp->Next;
               deletePoint(__wp);
               WayPoints.Len--;
               RemovedNumber++;
            }
         }
         wp = wp->Next;
      }
      wp0 = wp0->Next;
   }
   return RemovedNumber;
}
//---------------------------------------------------------------------------

