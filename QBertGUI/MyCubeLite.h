#pragma once

#include "MyCubeDefs.h"

#include <vector>
#include <queue>

using namespace MyCubeDefs;

class MyCubeLite
{
public:
	enum Move{ F, R, U, L, B, D };
	struct RotLite
	{
		Move m : 6;
		unsigned r : 2;

		RotLite reversed() { return { m, 4-r }; };
	};
	
	//Cube Edge
	struct CEdge
	{
		CEdge() : e(UR), o(0) {}
		CEdge(Edge e, unsigned o) : e(e), o(o) {}
		Edge e : 7;	//cur edge
		unsigned o : 1;	//orientation
	};
	//Cube Corner
	struct CCorner 
	{
		CCorner() : c(URF), o(0) {}
		CCorner(Corner c, unsigned o) : c(c), o(o) {}
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


	//Solve CONSTS
	static constexpr inline int MAX_MOVES = 999;
	static constexpr inline RotLite phase1Moves[18] =
	{
		{ F, 1 },{ F, 2 },{ F, 3 },
		{ R, 1 },{ R, 2 },{ R, 3 },
		{ U, 1 },{ U, 2 },{ U, 3 },
		{ L, 1 },{ L, 2 },{ L, 3 },
		{ B, 1 },{ B, 2 },{ B, 3 },
		{ D, 1 },{ D, 2 },{ D, 3 },
	};
	static constexpr inline RotLite phase2Moves[10] =
	{
		{ F, 2 },{ R, 2 },
		{ U, 1 },{ U, 2 },{ U, 3 },
		{ L, 2 },{ B, 2 },
		{ D, 1 },{ D, 2 },{ D, 3 },
	};


	//Static Variables
private:
	//Solve variables
	static inline bool solveInit = false;

	static constexpr inline int CornerOriSize = 2187, EdgeOriSize = 2048, UDSliceSize = 495;
	static constexpr inline int CornerPermSize = 40320, EdgePermSize = 40320, UDSliceSortedSize = 24;

	//Move Tables
	//Basically want to make a move table too. This maps a move (IE: R1) to a single coord and how it changes (corner ori 4 -> 6)
	//interesting
	static inline unsigned CornerOriMove[2187*_countof(phase1Moves)];
	static inline unsigned EdgeOriMove[2048*_countof(phase1Moves)];
	static inline unsigned UDSliceMove[495*_countof(phase1Moves)];

	static inline unsigned CornerPermMove[40320*_countof(phase2Moves)];
	static inline unsigned EdgePermMove[40320*_countof(phase2Moves)];
	static inline unsigned UDSliceSortedMove[24*_countof(phase2Moves)];

	//Prune Tables
	//Phase 1
	static inline byte CornerOriSteps[2187];
	static inline byte EdgeOriSteps[2048];
	static inline byte UDSliceSteps[495];
	//Phase 2
	static inline byte CornerPermSteps[40320];
	static inline byte EdgePermSteps[40320];
	static inline byte UDSliceSortedSteps[24];



public:
	MyCubeLite();
	~MyCubeLite();
	MyCubeLite(const MyCubeLite & c);
	MyCubeLite &operator=(const MyCubeLite& c);

	void test();

	//SET
	void setCorner(Corner toSet, Corner c, unsigned o);
	void setEdge(Edge toSet, Edge e, unsigned o);


	void reset();

	void turn(RotLite r);
	void turn(std::vector<RotLite> rots);

	//SOLVING
	std::vector<RotLite> solve(int max_moves = MAX_MOVES) const;


	//Init Solve (on first MyCube creation)
	static void initSolve();
	static void initMoveArray(unsigned* arr, int len, const RotLite* moves, int movesLen, unsigned(*f)(const MyCubeLite&));
	static void initPruneArray(byte* arr, int len, const RotLite* moves, int movesLen, unsigned(*f)(const MyCubeLite&));


	//Phase 1
	unsigned getCornerOriCoord() const;
	unsigned getEdgeOriCoord() const;
	unsigned getUDSliceCoord() const;
	CoordTriple getPhase1Coords() const { return { getCornerOriCoord(), getEdgeOriCoord(), getUDSliceCoord() }; }
	static CoordTriple turnPhase1Coords(CoordTriple coords, int phase1MovesIndex);

	bool isG1() const;
	static bool isG1(CoordTriple coords);

	//Phase 2
	//All Incorrect if not in G1
	unsigned getCornerPerm(Corner c) const;
	unsigned getEdgePerm(Edge e) const;

	unsigned getCornerPermCoord() const;
	unsigned getEdgePermCoord() const;
	unsigned getUDSliceSortedCoord() const;
	unsigned getUDSliceSortedCoordLite() const;
	CoordTriple getPhase2Coords() const { return { getCornerPermCoord(), getEdgePermCoord(), getUDSliceSortedCoordLite() }; }
	static CoordTriple turnPhase2Coords(CoordTriple coords, int phase2MovesIndex);

	//Incorrect if not in G1
	bool solved() const;
	static bool solved(CoordTriple coords);


	//Solve Phase 1
	static int S_stepsFromG1(CoordTriple c);
	int stepsFromG1() const;

	std::vector<RotLite> solveToG1(int max_depth = MAX_MOVES) const;
	int searchToG1(std::vector<RotLite> &rots, int g, int bound, int max_depth, RotLite lastRot);
	int searchToG1(std::vector<RotLite> &rots, CoordTriple coords, int g, int bound, int max_depth, RotLite lastRot);


	//Solve Phase 2
	//Only use when in G1 or probably just not correct
	static int S_stepsFromSolved(CoordTriple c);
	int stepsFromSolved() const;

	std::vector<RotLite> solveFromG1(int g, RotLite r, int max_depth = MAX_MOVES) const;
	int searchFromG1(std::vector<RotLite> &rots, int g, int bound, RotLite lastRot);
	int searchFromG1(std::vector<RotLite> &rots, CoordTriple coords, int g, int bound, RotLite lastRot);

public:
	//Static Equivalents
	static unsigned S_getCornerOriCoord(const MyCubeLite& c) { return c.getCornerOriCoord(); };
	static unsigned S_getEdgeOriCoord(const MyCubeLite& c) { return c.getEdgeOriCoord(); };
	static unsigned S_getUDSliceCoord(const MyCubeLite& c) { return c.getUDSliceCoord(); };

	static unsigned S_getCornerPermCoord(const MyCubeLite& c) { return c.getCornerPermCoord(); };
	static unsigned S_getEdgePermCoord(const MyCubeLite& c) { return c.getEdgePermCoord(); };
	static unsigned S_getUDSliceSortedCoord(const MyCubeLite& c) { return c.getUDSliceSortedCoord(); };
	static unsigned S_getUDSliceSortedCoordLite(const MyCubeLite& c) { return c.getUDSliceSortedCoordLite(); };

private:
	//Corners
	CCorner m_cs[NumCorners];
	//Edges
	CEdge m_es[NumEdges];
};

