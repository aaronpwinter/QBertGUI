#pragma once

#include "MyCubeDefs.h"

using namespace MyCubeDefs;

class MyCubeLite
{
public:
	enum Move{ F, R, U, L, B, D };
	struct RotLite
	{
		Move m : 6;
		unsigned r : 2;
	};
	
	//Cube Edge
	struct CEdge 
	{
		CEdge() : e(UR), o(0) { }
		CEdge(Edge e, unsigned o) : e(e), o(o) { }
		Edge e : 7;	//cur edge
		unsigned o : 1;	//orientation
	};
	//Cube Corner
	struct CCorner 
	{
		CCorner() : c(URF), o(0) { }
		CCorner(Corner c, unsigned o) : c(c), o(o) { }
		Corner c : 6; //cur corner
		unsigned o : 2; //orientation
	};


	//Turn info
	//CW order
	static constexpr inline Corner CornerTurnOrder[6][4] =
	{
		{ UFL, URF, DFR, DLF }, //F
		{ URF, UBR, DRB, DFR }, //R
		{ ULB, UBR, URF, UFL }, //U
		{ ULB, UFL, DLF, DBL }, //L
		{ UBR, ULB, DBL, DRB }, //B
		{ DLF, DFR, DRB, DBL }, //D
	};
	static constexpr inline int CornerOriChange[6][4] =
	{
		{ 1, 2, 1, 2 }, //F
		{ 1, 2, 1, 2 }, //R
		{ 0, 0, 0, 0 }, //U
		{ 1, 2, 1, 2 }, //L
		{ 1, 2, 1, 2 }, //B
		{ 0, 0, 0, 0 }, //D
	};
	static constexpr inline Edge EdgeTurnOrder[6][4] =
	{
		{ UF, FR, DF, FL }, //F
		{ UR, BR, DR, FR }, //R
		{ UB, UR, UF, UL }, //U
		{ UL, FL, DL, BL }, //L
		{ UB, BL, DB, BR }, //B
		{ DF, DR, DB, DL }, //D
	};
	static constexpr inline int EdgeOriChange[6][4] =
	{
		{ 1, 1, 1, 1 }, //F
		{ 0, 0, 0, 0 }, //R
		{ 0, 0, 0, 0 }, //U
		{ 0, 0, 0, 0 }, //L
		{ 1, 1, 1, 1 }, //B
		{ 0, 0, 0, 0 }, //D
	};


public:
	MyCubeLite();
	~MyCubeLite();
	MyCubeLite(const MyCubeLite & c);
	MyCubeLite &operator=(const MyCubeLite& c);

	void test();

	void reset();

	void turn(RotLite r);

	unsigned getCornerPerm(Corner c) const;
	unsigned getEdgePerm(Edge e) const;

	//Phase 1
	unsigned getCornerOriCoord() const;
	unsigned getEdgeOriCoord() const;
	unsigned getUDSliceCoord() const;
	CoordTriple getPhase1Coords() const { return { getCornerOriCoord(), getEdgeOriCoord(), getUDSliceCoord() }; }

	bool isG1() const;

	//Phase 2
	unsigned getCornerPermCoord() const;
	unsigned getEdgePermCoord() const;
	unsigned getUDSliceSortedCoord() const;
	//Incorrect if not in G1
	unsigned getUDSliceSortedCoordLite() const;
	CoordTriple getPhase2Coords() const { return { getCornerPermCoord(), getEdgePermCoord(), getUDSliceSortedCoordLite() }; }

	//Incorrect if not in G1
	bool solved() const;


private:
	//Corners
	CCorner m_cs[NumCorners];
	//Edges
	CEdge m_es[NumEdges];
};

