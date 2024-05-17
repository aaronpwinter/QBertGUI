#pragma once

#include <vector>
#include <string>
#include <iostream>

class FunnyCube
{
	//public info
public:
	enum Side { Front = 0, Right = 1, Up = 2, Left = 3, Back = 4, Down = 5 };
	enum Color { White, Green, Red, Blue, Orange, Yellow };
	struct Facelet
	{
		Color c;
		int orgRow, orgCol;
		int info = -1;
	};
	enum Move { F, R, U, L, B, D, X, Y, Z, M, E, S, BadMove };
	struct RotInfo
	{
		Move m;
		int layers;
		int cwturn; //Amount of times to turn clockwise. -1 for ccw
		bool wide; //If this is a wide turn (IE: turn multiple layers)
	};
	struct CubeCoord
	{
		Side s;
		int r, c;
	};

	//public methods
public:
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

	//Access
	int size() const { return m_size; }

	int index(Side s, int row, int col) const
	{
		return (s * m_size2) + (row * m_size) + col;
	}
	int index(CubeCoord coord) const { return index(coord.s, coord.r, coord.c); }

	Color get(Side s, int row, int col)	const { return m_cube[index(s, row, col)].c; }
	Color get(CubeCoord coord) const { return get(coord.s, coord.r, coord.c); }
	Facelet getFacelet(Side s, int row, int col) const { return m_cube[index(s, row, col)]; }
	Facelet getFacelet(CubeCoord coord) const { return getFacelet(coord.s, coord.r, coord.c); }
	int getInfo(Side s, int row, int col) const { return m_cube[index(s, row, col)].info; }
	int getInfo(CubeCoord coord) const { return getInfo(coord.s, coord.r, coord.c); }

	void set(Side s, int row, int col, Color c)	{ m_cube[index(s, row, col)].c = c; }
	void set(CubeCoord coord, Color c) { set(coord.s, coord.r, coord.c, c); }
	void setFacelet(Side s, int row, int col, Facelet f) { m_cube[index(s, row, col)] = f; }
	void setFacelet(CubeCoord coord, Facelet f) { setFacelet(coord.s, coord.r, coord.c, f); }
	void setInfo(Side s, int row, int col, int i) { m_cube[index(s, row, col)].info = i; }
	void setInfo(CubeCoord coord, int i) { setInfo(coord.s, coord.r, coord.c, i); }


	//Change
	//Reset cube to normalness
	virtual void reset();

	//Turn the cube using specified rotation info
	//	If invalid return false
	bool turn(RotInfo r);
	void turn(std::vector<RotInfo> rots);



	//Data
	std::wstring asString() const;

	//For Windows terminal
	void prettyPrint() const;

	static const inline std::wstring ColorAsWStr[6] = { L"W", L"G", L"R", L"B", L"O", L"Y" };
	static const inline std::string ColorAsStr[6] = { "W", "G", "R", "B", "O", "Y" };
	static const inline COLORREF ColorAsColorRef[6] = 
		{ 
			RGB(255, 255, 255),
			RGB(50, 255, 50),
			RGB(255, 0, 0),
			RGB(10, 10, 255),
			RGB(255, 121, 0),
			RGB(255, 255, 0),
		};
	static std::wstring colToStr(Color c);
	static COLORREF colToColorRef(Color c);
	COLORREF getColorRef(Side s, int row, int col) const { return colToColorRef(get(s, row, col)); };

	static void prettyPrintColor(Color c);

	//Get Rotation info and stuff
	static RotInfo strToRot(std::wstring s);
	static std::wstring rotToStr(RotInfo r);
	std::wstring rotToNotation(RotInfo r) const;

	static std::vector<FunnyCube::RotInfo> strToRots(std::wstring s);
	std::wstring rotsToNotation(std::vector<RotInfo> rots) const;

	static std::vector<FunnyCube::RotInfo> reverseRots(std::vector<RotInfo> rots);

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

