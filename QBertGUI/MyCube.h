#pragma once
#include "FunnyCube.h"
#include "MyCubeDefs.h"

#include "MyCubeLite.h"

using namespace MyCubeDefs;

//Just a forced 3x3 :)
class MyCube :
	public FunnyCube
{
public:
	//--CONSTS
	static constexpr inline CubeCoord CornerCoord[8][3] =
		{
			{ { Up, 2, 2 },{ Right, 0, 0 },{ Front, 0, 2 } },
			{ { Up, 2, 0 },{ Front, 0, 0 },{ Left, 0, 2 } },
			{ { Up, 0, 0 },{ Left, 0, 0 },{ Back, 0, 2 } },
			{ { Up, 0, 2 },{ Back, 0, 0 },{ Right, 0, 2 } },

			{ { Down, 0, 2 },{ Front, 2, 2 },{ Right, 2, 0 } },
			{ { Down, 0, 0 },{ Left, 2, 2 },{ Front, 2, 0 } },
			{ { Down, 2, 0 },{ Back, 2, 2 },{ Left, 2, 0 } },
			{ { Down, 2, 2 },{ Right, 2, 2 },{ Back, 2, 0 } }
		};
	static constexpr inline CubeCoord EdgeCoord[12][2] =
		{
			{ { Up, 1, 2 },{ Right, 0, 1 } },
			{ { Up, 2, 1 },{ Front, 0, 1 } },
			{ { Up, 1, 0 },{ Left, 0, 1 } },
			{ { Up, 0, 1 },{ Back, 0, 1 } },

			{ { Down, 1, 2 },{ Right, 2, 1 } },
			{ { Down, 0, 1 },{ Front, 2, 1 } },
			{ { Down, 1, 0 },{ Left, 2, 1 } },
			{ { Down, 2, 1 },{ Back, 2, 1 } },

			{ { Front, 1, 2 },{ Right, 1, 0 } },
			{ { Front, 1, 0 },{ Left, 1, 2 } },
			{ { Back, 1, 2 },{ Left, 1, 0 } },
			{ { Back, 1, 0 },{ Right, 1, 2 } },

		};

	//Solve CONSTS
	//For purposes of solving, U = White, R = Red, F = Green, D = Yellow, L = Orange, B= Blue
	static constexpr inline int MAX_MOVES = 999;
	static constexpr inline RotInfo phase1Moves[18] =
	{
		{ R, 1, 1, true },{ R, 1, 2, true },{ R, 1, -1, true },
		{ U, 1, 1, true },{ U, 1, 2, true },{ U, 1, -1, true },
		{ F, 1, 1, true },{ F, 1, 2, true },{ F, 1, -1, true },
		{ D, 1, 1, true },{ D, 1, 2, true },{ D, 1, -1, true },
		{ L, 1, 1, true },{ L, 1, 2, true },{ L, 1, -1, true },
		{ B, 1, 1, true },{ B, 1, 2, true },{ B, 1, -1, true },
	};
	static constexpr inline RotInfo phase2Moves[10] =
	{
		{ R, 1, 2, true },{ F, 1, 2, true },{ L, 1, 2, true },{ B, 1, 2, true },
		{ U, 1, 1, true },{ U, 1, 2, true },{ U, 1, -1, true },
		{ D, 1, 1, true },{ D, 1, 2, true },{ D, 1, -1, true },
	};

	//Static Variables
private:
	//Solve variables
	static inline bool solveInit = false;
	//Phase 1
	static constexpr inline int CornerOriSize = 2187, EdgeOriSize = 2048, UDSliceSize = 495;
	static inline int CornerOriSteps[2187];
	static inline int EdgeOriSteps[2048];
	static inline int UDSliceSteps[495];
	//Phase 2
	static constexpr inline int CornerPermSize = 40320, EdgePermSize = 40320, UDSliceSortedSize = 24;
	static inline int CornerPermSteps[40320];
	static inline int EdgePermSteps[40320];
	static inline int UDSliceSortedSteps[24];
public:
	//--CLASS STUFF
	MyCube() :
		FunnyCube(3)
	{
		initSolve();
		reset();
	};
	MyCube(const MyCube& c) :
		FunnyCube(c)
	{

	};
	MyCube &operator=(const MyCube& c)
	{
		return *this;
	};

	~MyCube()
	{
	};

	//--CHANGE
	void reset() override;
	//Sets info tag for pieces based on color (NOT IMPLEMENTED)
	void assignPieceInfo(); 
	//Resets orientation and also returns the moves taken to do so (IE: reversing the return will go back to pre reset)
	std::vector<MyCube::RotInfo> resetOrientation();

	//Get
	Corner getCornerAt(Corner c) const;
	Edge getEdgeAt(Edge e) const;

public:
	//--SOLVING TYPE BEAT
	std::vector<RotInfo> solve(int max_moves = MAX_MOVES) const;

	//Init Solve (on first MyCube creation)
	static void initSolve();
	static void initSolveArray(int* arr, int len, const RotInfo* moves, int movesLen, int (*f)(const MyCube&));

	int getCornerOri(Corner c) const;
	int getEdgeOri(Edge e) const;

	int getCornerPerm(Corner c) const;
	int getEdgePerm(Edge e) const;


	//Phase 1
	int getCornerOriCoord() const;
	int getEdgeOriCoord() const;
	int getUDSliceCoord() const;
	CoordTriple getPhase1Coords() const						{ return { unsigned(getCornerOriCoord()), unsigned(getEdgeOriCoord()), unsigned(getUDSliceCoord()) }; }

	bool isG1() const;

	//Phase 2
	int getCornerPermCoord() const;
	int getEdgePermCoord() const;
	int getUDSliceSortedCoord() const;
	CoordTriple getPhase2Coords() const						{ return { unsigned(getCornerPermCoord()), unsigned(getEdgePermCoord()), unsigned(getUDSliceSortedCoord()) }; }

	bool solved() const;

	//Solve Phase 1
	static int S_stepsFromG1(CoordTriple c);
	int stepsFromG1() const { return S_stepsFromG1(getPhase1Coords()); };

	std::vector<RotInfo> solveToG1(int max_depth = MAX_MOVES) const;
	int searchToG1(std::vector<RotInfo> &rots, int g, int bound, int max_depth) const;

	//Solve Phase 2
	//DANGEROUS - ONLY USE WHEN IN G1 (UDSliceSorted is > 24 when out of G1, this does not check for speed)
	static int S_stepsFromSolved(CoordTriple c);
	int stepsFromSolved() const { return S_stepsFromSolved(getPhase2Coords()); };

	std::vector<RotInfo> solveFromG1(int max_depth = MAX_MOVES) const;
	int searchFromG1(std::vector<RotInfo> &rots, int g, int bound) const;


public:
	//Static Equivalents
	static int S_getCornerOriCoord(const MyCube& c)			{ return c.getCornerOriCoord(); };
	static int S_getEdgeOriCoord(const MyCube& c)			{ return c.getEdgeOriCoord(); };
	static int S_getUDSliceCoord(const MyCube& c)			{ return c.getUDSliceCoord(); };

	static int S_getCornerPermCoord(const MyCube& c)		{ return c.getCornerPermCoord(); };
	static int S_getEdgePermCoord(const MyCube& c)			{ return c.getEdgePermCoord(); };
	static int S_getUDSliceSortedCoord(const MyCube& c)		{ return c.getUDSliceSortedCoord(); };

private:
	//MyCubeLite type things lololol hahahahah
	static MyCubeLite::RotLite rotToRotLite(RotInfo r);

//public:
//	MyCubeLite copyLite;


};
