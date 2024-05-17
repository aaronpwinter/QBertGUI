#pragma once
#include "FunnyCube.h"


//Just a forced 3x3 :)
class MyCube :
	public FunnyCube
{
public:
	struct CoordTriple
	{
		int x1, x2, x3;
	};
	enum Corner { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB, NumCorners };
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
	enum Edge { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR, NumEdges };
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

public:
	MyCube():
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

	void reset() override;
	void assignPieceInfo();
	std::vector<MyCube::RotInfo> resetOrientation();

public:
	//SOLVING TYPE BEAT
	std::vector<RotInfo> solve();

	static const inline int MAX_MOVES = 999;

	static inline bool solveInit = false;
	static void initSolve();

	static void initSolveArray(int* arr, int len, const RotInfo* moves, int movesLen, int (*f)(MyCube&));

	//For purposes of solving, U = White, R = Red, F = Green, D = Yellow, L = Orange, B= Blue
	Corner getCornerAt(Corner c);
	Edge getEdgeAt(Edge e);

	int getCornerOri(Corner c);
	int getEdgeOri(Edge e);

	int getCornerPerm(Corner c);
	int getEdgePerm(Edge e);

	//Phase 1
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
	}; //Idk why but if defined in phase 2 (under these statics) it go bad data :(

	static inline int CornerOriSteps[2187];
	static inline int EdgeOriSteps[2048];
	static inline int UDSliceSteps[495];

	int getCornerOriCoord();
	static int S_getCornerOriCoord(MyCube& c) { return c.getCornerOriCoord(); };
	int getEdgeOriCoord();
	static int S_getEdgeOriCoord(MyCube& c) { return c.getEdgeOriCoord(); };
	int getUDSliceCoord();
	static int S_getUDSliceCoord(MyCube& c) { return c.getUDSliceCoord(); };

	CoordTriple getPhase1Coords() { return { getCornerOriCoord(), getEdgeOriCoord(), getUDSliceCoord() }; }
	bool isG1();

	int stepsFromG1() { return S_stepsFromG1(getPhase1Coords()); };
	static int S_stepsFromG1(CoordTriple c);

	std::vector<RotInfo> solveToG1();
	int searchToG1(std::vector<RotInfo> &rots, int g, int bound);

	//Phase 2
	static inline int CornerPermSteps[40320];
	static inline int EdgePermSteps[40320];
	static inline int UDSliceSortedSteps[24];

	int getCornerPermCoord();
	static int S_getCornerPermCoord(MyCube& c) { return c.getCornerPermCoord(); };
	int getEdgePermCoord();
	static int S_getEdgePermCoord(MyCube& c) { return c.getEdgePermCoord(); };
	int getUDSliceSortedCoord();
	static int S_getUDSliceSortedCoord(MyCube& c) { return c.getUDSliceSortedCoord(); };

	CoordTriple getPhase2Coords() { return { getCornerPermCoord(), getEdgePermCoord(), getUDSliceSortedCoord() }; }
	bool solved();

	//DANGEROUS - ONLY USE WHEN IN G1 (UDSliceSorted is > 24 when out of G1, this does not check for speed)
	int stepsFromSolved() { return S_stepsFromSolved(getPhase2Coords()); };
	static int S_stepsFromSolved(CoordTriple c);

	std::vector<RotInfo> solveFromG1();
	int searchFromG1(std::vector<RotInfo> &rots, int g, int bound);

};
