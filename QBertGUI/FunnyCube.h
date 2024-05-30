#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "AlgSaver.h"

class FunnyCube
{
	//public info
public:
	//--ENUMS
	enum Side { Up = 0, Left = 1, Front = 2, Right = 3, Back = 4, Down = 5 };
	enum Color { White = Up, Orange = Left, Green = Front, Red = Right, Blue = Back, Yellow = Down };
	enum Move { U, L, F, R, B, D, X, Y, Z, M, E, S, BadMove };

	//--STRUCTS
	struct Facelet
	{
		COLORREF c;
		int orgRow, orgCol;
		int info = -1; //Optional info tag for each facelet
	};
	struct RotInfo
	{
		Move m;
		int layers;
		int cwturn; //Amount of times to turn clockwise. -1 for ccw
		bool wide; //If this is a wide turn (IE: turn multiple layers)

		RotInfo reversed() { return { m, layers, cwturn == 2 ? cwturn : -cwturn, wide }; };
	};
	struct CubeCoord
	{
		Side s;
		int r, c;
	};


	//--CONSTS
	static const inline std::wstring ColorAsWStr[6] = { L"W", L"O", L"G", L"R", L"B", L"Y" };
	static const inline std::string ColorAsStr[6] = { "W", "O", "G", "R", "B", "Y" };
	static constexpr inline COLORREF ColorAsColorRef[6] =
	{
		RGB(255, 255, 255),	//White
		RGB(255, 121, 0),	//Orange
		RGB(50, 255, 50),	//Green
		RGB(255, 0, 0),		//Red
		RGB(10, 10, 255),	//Blue
		RGB(255, 255, 0),	//Yellow
	};

	static const inline std::wstring MoveAsWStr[13] = { L"U", L"L", L"F", L"R", L"B", L"D", L"x", L"y", L"z", L"M", L"E", L"S", L"-" };
	static const inline std::string MoveAsStr[13] = { "U", "L", "F", "R", "B", "D", "x", "y", "z", "M", "E", "S", "-" };

	//public methods
public:
	//--CLASS STUFF :)
	FunnyCube(int sz) :
		m_size(sz), m_size2(m_size*m_size), m_cube(6 * m_size*m_size)
	{
		reset();
	};
	FunnyCube(const FunnyCube &c) :
		m_size(c.m_size), m_size2(m_size*m_size), m_cube(c.m_cube)
	{
	};
	FunnyCube &operator=(const FunnyCube& c)
	{
		m_size = c.m_size;
		m_size2 = c.m_size2;
		m_cube = c.m_cube;
		return *this;
	};

	~FunnyCube()
	{
	};

	//--ACCESS
	int size() const { return m_size; }

	int index(Side s, int row, int col) const { return (s * m_size2) + (row * m_size) + col; }

	Facelet get(Side s, int row, int col)	const { return m_cube[index(s, row, col)]; }
	COLORREF getColor(Side s, int row, int col) const { return m_cube[index(s, row, col)].c; }
	int getInfo(Side s, int row, int col) const { return m_cube[index(s, row, col)].info; }

	void set(Side s, int row, int col, Facelet f) { m_cube[index(s, row, col)] = f; }
	void setColor(Side s, int row, int col, COLORREF c) { m_cube[index(s, row, col)].c = c; }
	void setInfo(Side s, int row, int col, int i) { m_cube[index(s, row, col)].info = i; }

	//-CubeCoord access versions
	int index(CubeCoord coord) const { return index(coord.s, coord.r, coord.c); }

	Facelet get(CubeCoord coord) const { return get(coord.s, coord.r, coord.c); }
	COLORREF getColor(CubeCoord coord) const { return getColor(coord.s, coord.r, coord.c); }
	int getInfo(CubeCoord coord) const { return getInfo(coord.s, coord.r, coord.c); }

	void set(CubeCoord coord, Facelet f) { set(coord.s, coord.r, coord.c, f); }
	void setColor(CubeCoord coord, COLORREF c) { setColor(coord.s, coord.r, coord.c, c); }
	void setInfo(CubeCoord coord, int i) { setInfo(coord.s, coord.r, coord.c, i); }


	//--CHANGE
	//Reset cube to normalness
	virtual void reset();

	//Turn the cube using specified rotation info
	//	If invalid return false
	virtual bool turn(RotInfo r);
	virtual void turn(std::vector<RotInfo> rots);

	//Color in the cube
	//Format is just a constant string of characters, from UP, LEFT, FRONT, RIGHT, BACK, DOWN
	//	rows first, col second ([Up, 0, 0][Up, 0, 1][Up, 0, 2]...)
	void colorIn(std::wstring s);

	//--DATA

	//-STATIC
	//Colors
	static std::wstring colorToWStr(COLORREF c);
	static std::string colorToStr(COLORREF c);
	static COLORREF colorToColorRef(Color c) { return ColorAsColorRef[c]; };

	static COLORREF charToColor(wchar_t s);
	static COLORREF charToColor(char s);

	//Prints a single square �� to Windows terminal with a color
	static void prettyPrintColor(COLORREF c);

	//Rotations
	static RotInfo strToRot(std::wstring s);
	static RotInfo wstrToRot(std::wstring s) { return strToRot(s); };
	static RotInfo strToRot(std::string s);

	static std::vector<FunnyCube::RotInfo> strToRots(std::wstring s, const AlgSaver *a = nullptr);
	static std::vector<FunnyCube::RotInfo> wstrToRots(std::wstring s, const AlgSaver *a = nullptr) { return strToRots(s, a); };
	static std::vector<FunnyCube::RotInfo> strToRots(std::string s);

	static std::wstring rotToWStr(RotInfo r);
	static std::string rotToStr(RotInfo r);

	static std::vector<FunnyCube::RotInfo> reverseRots(std::vector<RotInfo> rots);


	//-MEMBER FUNCTIONS
	std::wstring asWString() const;
	std::string asString() const;

	//For Windows terminal
	void prettyPrint() const;


	//Get Rotation info and stuff
	std::wstring rotToNotationW(RotInfo r) const;
	std::string rotToNotation(RotInfo r) const;

	std::wstring rotsToNotationW(std::vector<RotInfo> rots) const;
	std::string rotsToNotation(std::vector<RotInfo> rots) const;


	//Private Methods
private:
	//Rotates the given face CW once (90deg)
	void faceCW(Side s);
	//Rotates the given face CCW once (90deg)
	void faceCCW(Side s);

	void faceRotate(Side s, int cwturn);

	void rotateSides(Side s0, Side s1, Side s2, Side s3,
		bool useRow0, bool useRow1, bool useRow2, bool useRow3,
		int usingRowCol0, int usingRowCol1, int usingRowCol2, int usingRowCol3,
		bool startZero0, bool startZero1, bool startZero2, bool startZero3,
		int cwturn);

	//Variables
private:
	int m_size;
	int m_size2;

	std::vector< Facelet > m_cube; //Representation of the cube in a 3d matrix [side]["row"]["col"]
};
