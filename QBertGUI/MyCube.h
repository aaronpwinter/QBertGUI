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

public:
	//--CLASS STUFF
	MyCube() :
		FunnyCube(3)
	{
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

	int getCornerOri(Corner c) const;
	int getEdgeOri(Edge e) const;

	MyCubeLite getLite() const;

public:
	//--SOLVING TYPE BEAT
	std::vector<RotInfo> solve(int max_moves = MAX_MOVES) const;

	bool solved() const;

private:
	//MyCubeLite type things lololol hahahahah
	static MyCubeLite::RotLite rotToRotLite(RotInfo r);
	static RotInfo rotLiteToRot(MyCubeLite::RotLite r);
	
};
