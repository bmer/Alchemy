//	CG32bitPixel.cpp
//
//	CG32bitPixel Class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

CG32bitPixel::AlphaArray8 CG32bitPixel::g_Alpha8 [256];
bool CG32bitPixel::m_bAlphaInitialized = CG32bitPixel::Init();

CG32bitPixel::CG32bitPixel (WORD wPixel)

//	CG32bitPixel constructor

	{
	m_dwPixel = 0xff000000 | (DWORD)CG16bitImage::RGBFromPixel(wPixel);
	}

CG32bitPixel::CG32bitPixel (const CGRealRGB &rgbColor)

//	CG32bitPixel
//
//	CG32bitPixel constructor

	{
	BYTE byRed = (BYTE)(DWORD)(rgbColor.GetRed() * 255.0);
	BYTE byGreen = (BYTE)(DWORD)(rgbColor.GetGreen() * 255.0);
	BYTE byBlue = (BYTE)(DWORD)(rgbColor.GetBlue() * 255.0);
	BYTE byAlpha = (BYTE)(DWORD)(rgbColor.GetAlpha() * 255.0);

	m_dwPixel = ((DWORD)byAlpha << 24) | ((DWORD)byRed << 16) | ((DWORD)byGreen << 8) | ((DWORD)byBlue);
	}

CG32bitPixel CG32bitPixel::Blend (CG32bitPixel rgbDest, CG32bitPixel rgbSrc)

//	Blend
//
//	Combines rgbSrc with rgbDest, using rgbSrc as the source opacity.
//	We assume dest has no alpha component and we assume that rgbSrc does.

	{
	BYTE *pAlpha = g_Alpha8[rgbSrc.GetAlpha()];
	BYTE *pAlphaInv = g_Alpha8[rgbSrc.GetAlpha() ^ 0xff];	//	Equivalent to 255 - rgbSrc.GetAlpha()

	BYTE byRedResult = pAlphaInv[rgbDest.GetRed()] + pAlpha[rgbSrc.GetRed()];
	BYTE byGreenResult = pAlphaInv[rgbDest.GetGreen()] + pAlpha[rgbSrc.GetGreen()];
	BYTE byBlueResult = pAlphaInv[rgbDest.GetBlue()] + pAlpha[rgbSrc.GetBlue()];

	return CG32bitPixel(byRedResult, byGreenResult, byBlueResult);
	}

CG32bitPixel CG32bitPixel::Blend (CG32bitPixel rgbDest, CG32bitPixel rgbSrc, BYTE bySrcAlpha)

//	Blend
//
//	Combines rgbSrc with rgbDest, using bySrcAlpha as the source opacity.
//	We assume source and dest have no alpha component.

	{
	BYTE *pAlpha = g_Alpha8[bySrcAlpha];
	BYTE *pAlphaInv = g_Alpha8[255 - bySrcAlpha];

	BYTE byRedResult = pAlphaInv[rgbDest.GetRed()] + pAlpha[rgbSrc.GetRed()];
	BYTE byGreenResult = pAlphaInv[rgbDest.GetGreen()] + pAlpha[rgbSrc.GetGreen()];
	BYTE byBlueResult = pAlphaInv[rgbDest.GetBlue()] + pAlpha[rgbSrc.GetBlue()];

	return CG32bitPixel(byRedResult, byGreenResult, byBlueResult);
	}

CG32bitPixel CG32bitPixel::Blend (CG32bitPixel rgbFrom, CG32bitPixel rgbTo, double rFade)

//	Fade
//
//	Fade from one value to the other. rFade goes from 0 to 1.

	{
	if (rFade <= 0.0)
		return rgbFrom;
	else if (rFade >= 1.0)
		return rgbTo;
	else
		{
		int iRDiff = (int)(DWORD)rgbTo.GetRed() - (int)(DWORD)rgbFrom.GetRed();
		int iGDiff = (int)(DWORD)rgbTo.GetGreen() - (int)(DWORD)rgbFrom.GetGreen();
		int iBDiff = (int)(DWORD)rgbTo.GetBlue() - (int)(DWORD)rgbFrom.GetBlue();

		BYTE byRed = (BYTE)((int)(DWORD)rgbFrom.GetRed() + (iRDiff * rFade));
		BYTE byGreen = (BYTE)((int)(DWORD)rgbFrom.GetGreen() + (iGDiff * rFade));
		BYTE byBlue = (BYTE)((int)(DWORD)rgbFrom.GetBlue() + (iBDiff * rFade));

		return CG32bitPixel(byRed, byGreen, byBlue);
		}
	}

CG32bitPixel CG32bitPixel::Blend3 (CG32bitPixel rgbNegative, CG32bitPixel rgbCenter, CG32bitPixel rgbPositive, double rFade)

//	Fade3
//
//	Fades from negative to center to positive.

	{
	if (rFade < 0.0)
		return Blend(rgbNegative, rgbCenter, rFade + 1.0);
	else if (rFade > 0.0)
		return Blend(rgbCenter, rgbPositive, rFade);
	else
		return rgbCenter;
	}

CG32bitPixel CG32bitPixel::Darken (CG32bitPixel rgbSource, BYTE byOpacity)

//	Darken
//
//	Equivalent to Blend(0, rgbSource, byOpacity)
	
	{
	BYTE *pAlpha = g_Alpha8[byOpacity];
	return CG32bitPixel(pAlpha[rgbSource.GetRed()], pAlpha[rgbSource.GetGreen()], pAlpha[rgbSource.GetBlue()]);
	}

CG32bitPixel CG32bitPixel::Desaturate (CG32bitPixel rgbColor)

//	Desaturate
//
//	Desaturate the given color

	{
	BYTE byMax = Max(Max(rgbColor.GetRed(), rgbColor.GetGreen()), rgbColor.GetBlue());
	BYTE byMin = Min(Min(rgbColor.GetRed(), rgbColor.GetGreen()), rgbColor.GetBlue());
	return CG32bitPixel::FromGrayscale((BYTE)(((DWORD)byMax + (DWORD)byMin) / 2), rgbColor.GetAlpha());
	}

CG32bitPixel CG32bitPixel::Fade (CG32bitPixel rgbFrom, CG32bitPixel rgbTo, int iPercent)

//	Fade
//
//	Fade from one value to the other by percent.

	{
	if (iPercent <= 0)
		return rgbFrom;
	else if (iPercent >= 100)
		return rgbTo;
	else
		{
		int iRDiff = (int)(DWORD)rgbTo.GetRed() - (int)(DWORD)rgbFrom.GetRed();
		int iGDiff = (int)(DWORD)rgbTo.GetGreen() - (int)(DWORD)rgbFrom.GetGreen();
		int iBDiff = (int)(DWORD)rgbTo.GetBlue() - (int)(DWORD)rgbFrom.GetBlue();

		BYTE byRed = (BYTE)((int)(DWORD)rgbFrom.GetRed() + (iRDiff * iPercent / 100));
		BYTE byGreen = (BYTE)((int)(DWORD)rgbFrom.GetGreen() + (iGDiff * iPercent / 100));
		BYTE byBlue = (BYTE)((int)(DWORD)rgbFrom.GetBlue() + (iBDiff * iPercent / 100));

		return CG32bitPixel(byRed, byGreen, byBlue);
		}
	}

bool CG32bitPixel::Init (void)

//	Init
//
//	Initializes alpha tables for blending.

	{
	DWORD i, j;

	for (i = 0; i < 256; i++)
		for (j = 0; j < 256; j++)
			g_Alpha8[j][i] = (BYTE)((DWORD)((i * (j / 255.0f)) + 0.5));

	return true;
	}

CG32bitPixel CG32bitPixel::PreMult (CG32bitPixel rgbColor, BYTE byAlpha)

//	PreMult
//
//	Returns a new color premultiplied by the alpha value.

	{
	if (byAlpha == 0x00)
		return CG32bitPixel::Null();
	else if (byAlpha == 0xff)
		return CG32bitPixel(rgbColor, 0xff);
	else
		{
		BYTE *pAlpha = CG32bitPixel::AlphaTable(byAlpha);

		BYTE byRed = rgbColor.GetRed();
		BYTE byGreen = rgbColor.GetGreen();
		BYTE byBlue = rgbColor.GetBlue();

		return CG32bitPixel(pAlpha[byRed], pAlpha[byGreen], pAlpha[byBlue], byAlpha);
		}
	}
