#include "PixelFormats.hpp"

static inline void move_420_block(int yTL, int yTR, int yBL, int yBR, int u,
		int v, int rowPixels, unsigned char * rgb) {
	const int rvScale = 91881;
	const int guScale = -22553;
	const int gvScale = -46801;
	const int buScale = 116129;
	const int yScale = 65536;
	int r, g, b;

	g = guScale * u + gvScale * v;
	//  if (force_rgb) {
	//      r = buScale * u;
	//      b = rvScale * v;
	//  } else {
	r = rvScale * v;
	b = buScale * u;
	//  }

	yTL *= yScale;
	yTR *= yScale;
	yBL *= yScale;
	yBR *= yScale;

	/* Write out top two pixels */
	rgb[0] = LIMIT(r+yTL);
	rgb[1] = LIMIT(g+yTL);
	rgb[2] = LIMIT(b+yTL);

	rgb[3] = LIMIT(r+yTR);
	rgb[4] = LIMIT(g+yTR);
	rgb[5] = LIMIT(b+yTR);

	/* Skip down to next line to write out bottom two pixels */
	rgb += 3 * rowPixels;
	rgb[0] = LIMIT(r+yBL);
	rgb[1] = LIMIT(g+yBL);
	rgb[2] = LIMIT(b+yBL);

	rgb[3] = LIMIT(r+yBR);
	rgb[4] = LIMIT(g+yBR);
	rgb[5] = LIMIT(b+yBR);
}

static inline void move_411_block(int yTL, int yTR, int yBL, int yBR, int u,
		int v, int rowPixels, unsigned char * rgb) {
	const int rvScale = 91881;
	const int guScale = -22553;
	const int gvScale = -46801;
	const int buScale = 116129;
	const int yScale = 65536;
	int r, g, b;

	g = guScale * u + gvScale * v;
	//  if (force_rgb) {
	//      r = buScale * u;
	//      b = rvScale * v;
	//  } else {
	r = rvScale * v;
	b = buScale * u;
	//  }

	yTL *= yScale;
	yTR *= yScale;
	yBL *= yScale;
	yBR *= yScale;

	/* Write out top two first pixels */
	rgb[0] = LIMIT(r+yTL);
	rgb[1] = LIMIT(g+yTL);
	rgb[2] = LIMIT(b+yTL);

	rgb[3] = LIMIT(r+yTR);
	rgb[4] = LIMIT(g+yTR);
	rgb[5] = LIMIT(b+yTR);

	/* Write out top two last pixels */
	rgb += 6;
	rgb[0] = LIMIT(r+yBL);
	rgb[1] = LIMIT(g+yBL);
	rgb[2] = LIMIT(b+yBL);

	rgb[3] = LIMIT(r+yBR);
	rgb[4] = LIMIT(g+yBR);
	rgb[5] = LIMIT(b+yBR);
}

// Consider a YUV420P image of 8x2 pixels.
//
// A plane of Y values    A B C D E F G H
//                        I J K L M N O P
//
// A plane of U values    1   2   3   4
// A plane of V values    1   2   3   4 ....
//
// The U1/V1 samples correspond to the ABIJ pixels.
//     U2/V2 samples correspond to the CDKL pixels.
//
/* Converts from planar YUV420P to RGB24. */
unsigned char *yuv420p_to_rgb24(int width, int height, unsigned char *pIn0,
		unsigned char *pOut0) {

	const int numpix = width * height;
	const int bytes = 24 >> 3;
	if (pOut0 == NULL) {
		pOut0 = (unsigned char *) malloc(numpix * 3);
	}
	int i, j, y00, y01, y10, y11, u, v;
	unsigned char *pY = pIn0;
	unsigned char *pU = pY + numpix;
	unsigned char *pV = pU + numpix / 4;
	unsigned char *pOut = pOut0;

	for (j = 0; j <= height - 2; j += 2) {
		for (i = 0; i <= width - 2; i += 2) {
			y00 = *pY;
			y01 = *(pY + 1);
			y10 = *(pY + width);
			y11 = *(pY + width + 1);
			u = (*pU++) - 128;
			v = (*pV++) - 128;

			move_420_block(y00, y01, y10, y11, u, v, width, pOut);

			pY += 2;
			pOut += 2 * bytes;

		}
		pY += width;
		pOut += width * bytes;
	}
	return pOut0;
}

// Consider a YUV420 image of 6x2 pixels.
//
// A B C D U1 U2
// I J K L V1 V2
//
// The U1/V1 samples correspond to the ABIJ pixels.
//     U2/V2 samples correspond to the CDKL pixels.
//
/* Converts from interlaced YUV420 to RGB24. */
/* [FD] untested... */
unsigned char *yuv420_to_rgb24(int width, int height, unsigned char *pIn0,
		unsigned char *pOut0) {
	const int numpix = width * height;
	const int bytes = 24 >> 3;
	if (pOut0 == NULL) {
		pOut0 = (unsigned char *) malloc(numpix * 3);
	}
	int i, j, y00, y01, y10, y11, u, v;
	unsigned char *pY = pIn0;
	unsigned char *pU = pY + 4;
	unsigned char *pV = pU + width;
	unsigned char *pOut = pOut0;

	for (j = 0; j <= height - 2; j += 2) {
		for (i = 0; i <= width - 4; i += 4) {
			y00 = *pY;
			y01 = *(pY + 1);
			y10 = *(pY + width);
			y11 = *(pY + width + 1);
			u = (*pU++) - 128;
			v = (*pV++) - 128;

			move_420_block(y00, y01, y10, y11, u, v, width, pOut);

			pY += 2;
			pOut += 2 * bytes;

			y00 = *pY;
			y01 = *(pY + 1);
			y10 = *(pY + width);
			y11 = *(pY + width + 1);
			u = (*pU++) - 128;
			v = (*pV++) - 128;

			move_420_block(y00, y01, y10, y11, u, v, width, pOut);

			pY += 4; // skip UV
			pOut += 2 * bytes;

		}
		pY += width;
		pOut += width * bytes;
	}
	return pOut0;
}

// Consider a YUV411P image of 8x2 pixels.
//
// A plane of Y values as before.
//
// A plane of U values    1       2
//                        3       4
//
// A plane of V values    1       2
//                        3       4
//
// The U1/V1 samples correspond to the ABCD pixels.
//     U2/V2 samples correspond to the EFGH pixels.
//
/* Converts from planar YUV411P to RGB24. */
/* [FD] untested... */
unsigned char *yuv411p_to_rgb24(int width, int height, unsigned char *pIn0,
		unsigned char *pOut0) {
	const int numpix = width * height;
	const int bytes = 24 >> 3;
	if (pOut0 == NULL) {
		pOut0 = (unsigned char *) malloc(numpix * 3);
	}
	int i, j, y00, y01, y10, y11, u, v;
	unsigned char *pY = pIn0;
	unsigned char *pU = pY + numpix;
	unsigned char *pV = pU + numpix / 4;
	unsigned char *pOut = pOut0;

	for (j = 0; j <= height; j++) {
		for (i = 0; i <= width - 4; i += 4) {
			y00 = *pY;
			y01 = *(pY + 1);
			y10 = *(pY + 2);
			y11 = *(pY + 3);
			u = (*pU++) - 128;
			v = (*pV++) - 128;

			move_411_block(y00, y01, y10, y11, u, v, width, pOut);

			pY += 4;
			pOut += 4 * bytes;

		}
	}
	return pOut0;
}

unsigned char *
yuyv_to_rgb24(int width, int height, unsigned char *src, unsigned char *dst) {

	unsigned char *s;
	unsigned char *d;
	const int numpix = width * height;
	if (dst == NULL) {
		dst = (unsigned char *) malloc(numpix * 3);
	}
	int l, c;
	int r, g, b, cr, cg, cb, y1, y2;

	l = height;
	s = src;
	d = dst;
	while (l--) {
		c = width >> 1;
		while (c--) {
			y1 = *s++;
			cb = ((*s - 128) * 454) >> 8;
			cg = (*s++ - 128) * 88;
			y2 = *s++;
			cr = ((*s - 128) * 359) >> 8;
			cg = (cg + (*s++ - 128) * 183) >> 8;

			r = y1 + cr;
			b = y1 + cb;
			g = y1 - cg;
			SAT(r);
			SAT(g);
			SAT(b);

			*d++ = b;
			*d++ = g;
			*d++ = r;

			r = y2 + cr;
			b = y2 + cb;
			g = y2 - cg;
			SAT(r);
			SAT(g);
			SAT(b);

			*d++ = b;
			*d++ = g;
			*d++ = r;
		}
	}
	return dst;
}

unsigned char *
uyvy_to_rgb24(int width, int height, unsigned char *src, unsigned char *dst) {
	unsigned char *s;
	unsigned char *d;
	const int numpix = width * height;
	if (dst == NULL) {
		dst = (unsigned char *) malloc(numpix * 3);
	}

	int l, c;
	int r, g, b, cr, cg, cb, y1, y2;

	l = height;
	s = src;
	d = dst;
	while (l--) {
		c = width >> 1;
		while (c--) {
			cb = ((*s - 128) * 454) >> 8;
			cg = (*s++ - 128) * 88;
			y1 = *s++;
			cr = ((*s - 128) * 359) >> 8;
			cg = (cg + (*s++ - 128) * 183) >> 8;
			y2 = *s++;

			r = y1 + cr;
			b = y1 + cb;
			g = y1 - cg;
			SAT(r);
			SAT(g);
			SAT(b);

			*d++ = b;
			*d++ = g;
			*d++ = r;

			r = y2 + cr;
			b = y2 + cb;
			g = y2 - cg;
			SAT(r);
			SAT(g);
			SAT(b);

			*d++ = b;
			*d++ = g;
			*d++ = r;
		}
	}
	return dst;
}

