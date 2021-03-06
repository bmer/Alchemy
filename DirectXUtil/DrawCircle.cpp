//	CGDraw.cpp
//
//	CGDraw Class
//	Copyright (c) 2015 by Kronosaur Productions, LLC. All Rights Reserved.

#include "PreComp.h"

void CGDraw::Circle (CG8bitImage &Dest, int xCenter, int yCenter, int iRadius, BYTE Value)

//	Circle
//
//	Draws a filled circle

	{
	//	Deal with edge-conditions

	if (iRadius <= 0)
		{
		Dest.SetPixel(xCenter, yCenter, Value);
		return;
		}

	//	Initialize some stuff

	int x = 0;
	int y = iRadius;
	int d = 1 - iRadius;
	int deltaE = 3;
	int deltaSE = -2 * iRadius + 5;

	Dest.FillLine(xCenter - iRadius, yCenter, 1 + 2 * iRadius, Value);

	//	Loop

	while (y > x)
		{
		if (d < 0)
			{
			d += deltaE;
			deltaE += 2;
			deltaSE += 2;
			}
		else
			{
			d += deltaSE;
			deltaE += 2;
			deltaSE += 4;

			Dest.FillLine(xCenter - x, yCenter - y, 1 + 2 * x, Value);
			Dest.FillLine(xCenter - x, yCenter + y, 1 + 2 * x, Value);

			y--;
			}

		x++;

		if (y >= x)
			{
			Dest.FillLine(xCenter - y, yCenter - x, 1 + 2 * y, Value);
			Dest.FillLine(xCenter - y, yCenter + x, 1 + 2 * y, Value);
			}
		}
	}

void CGDraw::Circle (CG32bitImage &Dest, int xCenter, int yCenter, int iRadius, CG32bitPixel rgbColor)

//	Circle
//
//	Draws a filled circle

	{
	BYTE byOpacity = rgbColor.GetAlpha();

	//	Deal with edge-conditions

	if (iRadius <= 0)
		{
		Dest.SetPixelTrans(xCenter, yCenter, rgbColor, byOpacity);
		return;
		}

	//	Initialize some stuff

	int x = 0;
	int y = iRadius;
	int d = 1 - iRadius;
	int deltaE = 3;
	int deltaSE = -2 * iRadius + 5;

	Dest.FillLine(xCenter - iRadius, yCenter, 1 + 2 * iRadius, rgbColor);

	//	Loop

	while (y > x)
		{
		if (d < 0)
			{
			d += deltaE;
			deltaE += 2;
			deltaSE += 2;
			}
		else
			{
			d += deltaSE;
			deltaE += 2;
			deltaSE += 4;

			Dest.FillLine(xCenter - x, yCenter - y, 1 + 2 * x, rgbColor);
			Dest.FillLine(xCenter - x, yCenter + y, 1 + 2 * x, rgbColor);

			y--;
			}

		x++;

		if (y >= x)
			{
			Dest.FillLine(xCenter - y, yCenter - x, 1 + 2 * y, rgbColor);
			Dest.FillLine(xCenter - y, yCenter + x, 1 + 2 * y, rgbColor);
			}
		}
	}

void CGDraw::Circle (CG32bitImage &Dest, int x, int y, int iRadius, const TArray<CG32bitPixel> &ColorRamp)

//	Circle
//
//	Draws a circle with a color ramp

	{
	CRadialCirclePainter Painter(iRadius, ColorRamp);
	Painter.Draw(Dest, x, y);
	}

void CGDraw::CircleImage (CG32bitImage &Dest, int x, int y, int iRadius, BYTE byOpacity, const CG32bitImage &Image, int xSrc, int ySrc, int cxSrc, int cySrc)

//	CircleImage
//
//	Draws a circle using the given image.

	{
	CImageCirclePainter Painter(iRadius, byOpacity, Image, xSrc, ySrc, cxSrc, cySrc);
	Painter.Draw(Dest, x, y);
	}

void CGDraw::CircleGradient (CG8bitImage &Dest, int x, int y, int iRadius, BYTE CenterValue, BYTE EdgeValue)

//	CircleGradient
//
//	Draws a circle gradient on an 8-bit image

	{
	CRadialCirclePainter8 Painter(iRadius, CenterValue, EdgeValue);
	Painter.Draw(Dest, x, y);
	}

void CGDraw::CircleGradient (CG32bitImage &Dest, int x, int y, int iRadius, CG32bitPixel rgbColor)

//	CircleGradient
//
//	Draws a circle that is solid at the center and fully transparent at the 
//	edges.

	{
	int i;

	if (iRadius <= 0)
		return;

	//	Generate an appropriate color ramp (pre-multiplied)

	TArray<CG32bitPixel> ColorRamp;
	ColorRamp.InsertEmpty(iRadius);
	for (i = 0; i < iRadius; i++)
		{
		BYTE byAlpha = (BYTE)(255 - (i * 255 / iRadius));
		ColorRamp[i] = CG32bitPixel(CG32bitPixel::Blend(0, rgbColor, byAlpha), byAlpha);
		}

	//	Draw the circle

	CRadialCirclePainter Painter(iRadius, ColorRamp, true);
	Painter.Draw(Dest, x, y);
	}

void CGDraw::RingGlowing (CG32bitImage &Dest, int x, int y, int iRadius, int iWidth, CG32bitPixel rgbColor)

//	RingGlowing
//
//	Draw a glowing ring

	{
	CGlowingRingPainter Painter(Dest, iRadius, iWidth, rgbColor);
	Painter.Draw(x, y);
	}

void CGDraw::RingGlowing (CG32bitImage &Dest, int x, int y, int iRadius, int iWidth, const TArray<CG32bitPixel> &ColorRamp, BYTE byOpacity)

//	RingGlowing
//
//	Draw a glowing ring.

	{
	CGlowingRingPainter Painter(Dest, iRadius, iWidth, ColorRamp, byOpacity);
	Painter.Draw(x, y);
	}

//	CImageCirclePainter --------------------------------------------------------

CImageCirclePainter::CImageCirclePainter (int iRadius, BYTE byOpacity, const CG32bitImage &Src, int xSrc, int ySrc, int cxSrc, int cySrc) : TCirclePainter32((cxSrc >= 0 ? cxSrc : Src.GetWidth()), iRadius),
		m_byOpacity(byOpacity),
		m_Src(Src),
		m_xSrc(xSrc),
		m_ySrc(ySrc),
		m_cxSrc(cxSrc >= 0 ? cxSrc : Src.GetWidth()),
		m_cySrc(cySrc >= 0 ? cySrc : Src.GetHeight())

//	CImageCirclePainter constructor

	{
	}

//	CRadialCirclePainter8 ------------------------------------------------------

CRadialCirclePainter8::CRadialCirclePainter8 (int iRadius, BYTE CenterValue, BYTE EdgeValue) : TRadialPainter8(iRadius)

//	CRadialCirclePainter8 constructor

	{
	int i;

	if (iRadius <= 0)
		return;

	int iStart = (int)(DWORD)CenterValue;
	int iRange = (int)(DWORD)EdgeValue - (int)(DWORD)CenterValue;

	m_Ramp.InsertEmpty(iRadius);
	for (i = 0; i < m_Ramp.GetCount(); i++)
		m_Ramp[i] = (BYTE)(iStart + (iRange * i / iRadius));
	}

//	CRadialCirclePainter -------------------------------------------------------

CRadialCirclePainter::CRadialCirclePainter (int iRadius, const TArray<CG32bitPixel> &ColorRamp, bool bPreMult) : TRadialPainter32(iRadius)

//	CCirclePainter constructor

	{
	int i;

	//	If the ramp is not pre-multiplied, then we need to do that now

	if (!bPreMult)
		{
		//	Pre-multiply the color ramp

		m_ColorRamp.InsertEmpty(ColorRamp.GetCount());
		m_pColorRamp = &m_ColorRamp;

		for (i = 0; i < m_ColorRamp.GetCount(); i++)
			m_ColorRamp[i] = CG32bitPixel::PreMult(ColorRamp[i]);
		}

	//	Otherwise we just take the ramp we've been given

	else
		m_pColorRamp = &ColorRamp;
	}
