#include "stdafx.h"

//---------------------------------------------------------------------------
bool ucl_decompress(const unsigned char* src, unsigned int src_len,
                    unsigned char *dst, unsigned int *dst_len)
{
	unsigned int bb = 0;
   unsigned int ilen = 0, olen = 0, last_m_off = 1;

	#define getbit(bb) (bb<<=1, bb&0xff ? (bb>>8)&1 : ((bb=(src[ilen++]<<1)+1)>>8)&1)

   for(;;)
   {
   	unsigned int m_off, m_len;

      if(getbit(bb))
      {
      	dst[olen++] = src[ilen++];
         continue;
   	}
      m_off = 1;
      do {
      	m_off = (m_off<<1) + getbit(bb);
      } while(!getbit(bb));

      if(m_off == 2)
      {
      	m_off = last_m_off;
      }
      else
      {
      	m_off = ((m_off-3)<<8) + src[ilen++];
         if(m_off == 0xffffffff)
         	break;
         last_m_off = ++m_off;
      }
      m_len = getbit(bb);
      m_len = (m_len<<1) + getbit(bb);
      if(m_len == 0)
      {
      	m_len++;
         do {
         	m_len = (m_len<<1) + getbit(bb);
         } while (!getbit(bb));
         m_len += 2;
      }
      m_len += (m_off > 0xd00);
      {
      	const unsigned char *m_pos;
         m_pos = dst + olen - m_off;
         dst[olen++] = *m_pos++;
         do dst[olen++] = *m_pos++; while (--m_len > 0);
      }
	}
   *dst_len = olen;
	if(ilen != src_len) return false;
   return true;
}
//---------------------------------------------------------------------------

