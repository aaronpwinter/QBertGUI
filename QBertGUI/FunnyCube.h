#pragma once

#include <vector>
#include <string>
#include <iostream>

class FunnyCube
{
	//public info
public:
	//--ENUMS
	enum Side	{ Front = 0, Right = 1, Up = 2, Left = 3, Back = 4, Down = 5 };
	enum Color	{ Green = Front, Red = Right, White = Up, Orange = Left, Blue = Back, Yellow = Down };
	enum Move	{ F, R, U, L, B, D, X, Y, Z, M, E, S, BadMove };

	//--STRUCTS
	struct Facelet
	{
		Color c;
		int orgRow, orgCol;
		int info = -1; //Optional info tag for each facelet
	};
	struct RotInfo
	{
		Move m;
		int layers;
		int cwturn; //Amount of times to turn clockwise. -1 for ccw
		bool wide; //If this is a wide turn (IE: turn multiple layers)

		RotInfo reversed()									{ return {m, layers, cwturn==2?cwturn:-cwturn, wide}; };
	};
	struct CubeCoord
	{
		Side s;
		int r, c;
	};

	
	//--CONSTS
	static const inline std::wstring ColorAsWStr[6] =		{ L"G", L"R", L"W", L"O", L"B", L"Y" };
	static const inline std::string ColorAsStr[6] =			{ "G", "R", "W", "O", "B", "Y" };
	static constexpr inline COLORREF ColorAsColorRef[6] =
		{
			RGB(50, 255, 50),	//Green
			RGB(255, 0, 0),		//Red
			RGB(255, 255, 255),	//White
			RGB(255, 121, 0),	//Orange
			RGB(10, 10, 255),	//Blue
			RGB(255, 255, 0),	//Yellow
		};

	static const inline std::wstring MoveAsWStr[13] =		{ L"F", L"R", L"U", L"L", L"B", L"D", L"x", L"y", L"z", L"M", L"E", L"S", L"-" };
	static const inline std::string MoveAsStr[13] =			{ "F", "R", "U", "L", "B", "D", "x", "y", "z", "M", "E", "S", "-" };

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

	int index(Side s, int row, int col) const				{	return (s * m_size2) + (row * m_size) + col; }

	Facelet get(Side s, int row, int col)	const			{ return m_cube[index(s, row, col)]; }
	Color getColor(Side s, int row, int col) const			{ return m_cube[index(s, row, col)].c; }
	int getInfo(Side s, int row, int col) const				{ return m_cube[index(s, row, col)].info; }

	void set(Side s, int row, int col, Facelet f)			{ m_cube[index(s, row, col)] = f; }
	void setColor (Side s, int row, int col, Color c)		{ m_cube[index(s, row, col)].c = c; }
	void setInfo(Side s, int row, int col, int i)			{ m_cube[index(s, row, col)].info = i; }

	//-CubeCoord access versions
	int index(CubeCoord coord) const						{ return index(coord.s, coord.r, coord.c); }

	Facelet get(CubeCoord coord) const						{ return get(coord.s, coord.r, coord.c); }
	Color getColor(CubeCoord coord) const					{ return getColor(coord.s, coord.r, coord.c); }
	int getInfo(CubeCoord coord) const						{ return getInfo(coord.s, coord.r, coord.c); }

	void set(CubeCoord coord, Facelet f)					{ set(coord.s, coord.r, coord.c, f); }
	void setColor(CubeCoord coord, Color c)					{ setColor(coord.s, coord.r, coord.c, c); }
	void setInfo(CubeCoord coord, int i)					{ setInfo(coord.s, coord.r, coord.c, i); }


	//--CHANGE
	//Reset cube to normalness
	virtual void reset();

	//Turn the cube using specified rotation info
	//	If invalid return false
	virtual bool turn(RotInfo r);
	virtual void turn(std::vector<RotInfo> rots);



	//--DATA

	//-STATIC
	//Colors
	static std::wstring colorToWStr(Color c)				{ return ColorAsWStr[c]; };
	static std::string colorToStr(Color c)					{ return ColorAsStr[c]; };
	static COLORREF colorToColorRef(Color c)				{ return ColorAsColorRef[c]; };

	//Prints a single square Å° to Windows terminal with a color
	static void prettyPrintColor(Color c);

	//Rotations
	static RotInfo strToRot(std::wstring s);
	static RotInfo wstrToRot(std::wstring s)				{ return strToRot(s); };
	static RotInfo strToRot(std::string s);

	static std::vector<FunnyCube::RotInfo> strToRots(std::wstring s);
	static std::vector<FunnyCube::RotInfo> wstrToRots(std::wstring s) { return strToRots(s); };
	static std::vector<FunnyCube::RotInfo> strToRots(std::string s);

	static std::wstring rotToWStr(RotInfo r);
	static std::string rotToStr(RotInfo r);

	static std::vector<FunnyCube::RotInfo> reverseRots(std::vector<RotInfo> rots);


	//-MEMBER FUNCTIONS
	std::wstring asWString() const;
	std::string asString() const;

	//For Windows terminal
	void prettyPrint() const;

	COLORREF getColorRef(Side s, int row, int col) const	{ return colorToColorRef(getColor(s, row, col)); };


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

