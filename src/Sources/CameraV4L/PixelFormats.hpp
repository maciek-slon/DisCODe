#include <cstdlib>
#include <cstdio>

/* LIMIT: convert a 16.16 fixed-point value to a byte, with clipping. */
#define LIMIT(x) ( (x)>0xffffff?0xff: ((x)<=0xffff?0:((x)>>16)) )

#define SAT(c) \
        if (c & (~255)) { if (c < 0) c = 0; else c = 255; }

unsigned char *yuv420p_to_rgb24(int width, int height, unsigned char *pIn0, unsigned char *pOut0 = NULL);
unsigned char *yuv420_to_rgb24(int width, int height, unsigned char *pIn0, unsigned char *pOut0 = NULL);
unsigned char *yuv411p_to_rgb24(int width, int height, unsigned char *pIn0, unsigned char *pOut0 = NULL);
unsigned char *uyvy_to_rgb24(int width, int height, unsigned char *src, unsigned char *dst = NULL);
unsigned char *yuyv_to_rgb24(int width, int height, unsigned char *src, unsigned char *dst = NULL);

//unsigned char *YUV420_to_RGB24(unsigned char *b1,int width,int height,unsigned char *b2=NULL);
