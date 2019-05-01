#pragma once
#ifndef JULIA_H
#define JULIA_H

#include <math.h>

#define Square sqrt
using ComplexType = double;

struct Complex
{
	ComplexType real;
	ComplexType image;

	Complex() :real(0), image(0) {}
	Complex(ComplexType real, ComplexType image) :real(real), image(image) {}

	Complex operator+(const Complex& b)
	{
		Complex c;
		c.real = this->real + b.real;
		c.image = this->image + b.image;
		return c;
	}
	
	Complex operator*(const Complex& b)
	{
		Complex c;
		c.real = this->real * b.real - this->image * b.image;
		c.image = this->real * b.image + this->image * b.real;
		return c;
	}

	ComplexType Model()
	{
		return Square(this->real * this->real + this->image * this->image);
	}
};

#undef Square

//²Î¿¼²©¿Íhttps://www.cnblogs.com/easymind223/archive/2012/07/05/2578231.html
#define CV_RGB(r,g,b) (r<<16)+(g<<8)+b+0xFF000000

static UINT ColorTable[21] = {
CV_RGB(28, 28, 28),
CV_RGB(130, 130, 130),
CV_RGB(85, 26, 139),
CV_RGB(224, 102, 255),
CV_RGB(255, 187, 255),
CV_RGB(0,0,205),
CV_RGB(72, 118, 255),
CV_RGB(0, 191, 255),
CV_RGB(0, 255, 255),
CV_RGB(0, 255, 127),
CV_RGB(0, 255, 0),
CV_RGB(50, 205, 50),
CV_RGB(173, 255, 47),
CV_RGB(255, 185, 15),
CV_RGB(255, 215, 0),
CV_RGB(255, 255, 0),
CV_RGB(255, 69, 0),
CV_RGB(255, 140, 0),
CV_RGB(255, 211, 155),
CV_RGB(255, 231, 186),
CV_RGB(255, 239, 213),
};

struct Julia
{
	static ComplexType Iteration(Complex z,Complex c, int iterationCount)
	{
		if (iterationCount == 0)
		{
			return z.Model();
		}
		else
		{
			return Iteration(z * z + c, c, iterationCount - 1);
		}
	}

	static UINT MapColor(ComplexType dist)
	{
		if (dist < 1.0 / 4096)
			return ColorTable[0];
		else if (dist < 1.0 / 1024)
			return ColorTable[1];
		else if (dist < 1.0 / 256)
			return ColorTable[2];
		else if (dist < 1.0 / 64)
			return ColorTable[3];
		else if (dist < 1.0 / 16)
			return ColorTable[4];
		else if (dist < 1.0 / 4)
			return ColorTable[5];
		else if (dist < 1)
			return ColorTable[6];
		else if (dist < 4)
			return ColorTable[7];
		else if (dist < 16)
			return ColorTable[8];
		else if (dist < 64)
			return ColorTable[9];
		else if (dist < 256)
			return ColorTable[10];
		else if (dist < 1024)
			return ColorTable[11];
		else if (dist < 4096)
			return ColorTable[12];
		else if (dist < 16384)
			return ColorTable[13];
		else if (dist < 65536)
			return ColorTable[14];
		else if (dist < 262144)
			return ColorTable[15];
		else if (dist < 1048576)
			return ColorTable[16];
		else if (dist < 4194304)
			return ColorTable[17];
		else if (dist < 16777216)
			return ColorTable[18];
		else if (dist < 67108864)
			return ColorTable[19];
		else return ColorTable[20];
	}
};

#endif // !JULIA_H
