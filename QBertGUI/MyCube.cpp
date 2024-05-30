#include "stdafx.h"
#include "MyCube.h"

#include <queue>


int mathCombo(int n, int k)
{
	if (k == 0) return 1;

	if (k > n / 2) return mathCombo(n, n - k);

	long ret = 1;

	for (int r = 1; r <= k; ++r)
	{
		ret *= n - r + 1;
		ret /= r;
	}

	return ret;
}

using namespace MyCubeDefs;


Corner MyCube::getCornerAt(Corner c) const
{
	return Corner(getInfo(CornerCoord[c][0]));
}

Edge MyCube::getEdgeAt(Edge e) const
{
	return Edge(getInfo(EdgeCoord[e][0]));
}

MyCubeLite MyCube::getLite() const
{
	MyCubeLite ret;

	//Corners
	for (int c = 0; c < NumCorners; ++c)
	{
		ret.setCorner(Corner(c), getCornerAt(Corner(c)), getCornerOri(Corner(c)));
	}

	//Edges
	for (int e = 0; e < NumEdges; ++e)
	{
		ret.setEdge(Edge(e), getEdgeAt(Edge(e)), getEdgeOri(Edge(e)));
	}

	return ret;

}

void MyCube::reset()
{
	FunnyCube::reset();

	//Assign the corner/edge names to make it wayyyyy easier lolololol hahahahahahahha
	//Corners !!
	for (int i = 0; i < 3; ++i)
	{
		for (int c = 0; c < NumCorners; ++c)
		{
			setInfo(CornerCoord[c][i], c);
		}
	}
	//Edges !!
	for (int i = 0; i < 2; ++i)
	{
		for (int e = 0; e < NumEdges; ++e)
		{
			setInfo(EdgeCoord[e][i], e);
		}
	}

	//center
	setInfo(Up, 1, 1, Up);
	setInfo(Left, 1, 1, Left);
	setInfo(Right, 1, 1, Right);
	setInfo(Down, 1, 1, Down);
	setInfo(Back, 1, 1, Back);
	setInfo(Front, 1, 1, Front);
}

bool MyCube::assignPieceInfo()
{
	//For any cube, assign the URF, UD... to all of the facelets
	//yeah idk how to do this at all but you know :) !! If i need to !!

	//center
	setInfo(Up, 1, 1, Up);
	setInfo(Left, 1, 1, Left);
	setInfo(Right, 1, 1, Right);
	setInfo(Down, 1, 1, Down);
	setInfo(Back, 1, 1, Back);
	setInfo(Front, 1, 1, Front);

	COLORREF cUp = getColor(Up, 1, 1), cDown = getColor(Down, 1, 1), cFront = getColor(Front, 1, 1), cBack = getColor(Back, 1, 1),
		cLeft = getColor(Left, 1, 1), cRight = getColor(Right, 1, 1);

	//corners
	for (int c = 0; c < NumCorners; ++c)
	{
		int ori = getCornerOri(Corner(c));
		if (ori == -1) return false;
		COLORREF c1 = getColor(CornerCoord[c][ori]), c2 = getColor(CornerCoord[c][(ori + 1) % 3]), c3 = getColor(CornerCoord[c][(ori + 2) % 3]);

		if (c1 == cUp)
		{
			//check up dogs
			for (int ref = URF; ref < DFR; ++ref)
			{
				if (c2 == getColor(CornerCoord[ref][1].s, 1, 1))
				{
					if (c3 == getColor(CornerCoord[ref][2].s, 1, 1))
					{
						setInfo(CornerCoord[c][0], ref);
						setInfo(CornerCoord[c][1], ref);
						setInfo(CornerCoord[c][2], ref);
						break;
					}
					else return false;
				}
			}
		}
		else 
		{ //c1 == cDown
		  //check down
			for (int ref = DFR; ref < NumCorners; ++ref)
			{
				if (c2 == getColor(CornerCoord[ref][1].s, 1, 1))
				{
					if (c3 == getColor(CornerCoord[ref][2].s, 1, 1))
					{
						setInfo(CornerCoord[c][0], ref);
						setInfo(CornerCoord[c][1], ref);
						setInfo(CornerCoord[c][2], ref);
						break;
					}
					else return false;
				}
			}
		}
	}

	//edges
	for (int e = 0; e < NumEdges; ++e)
	{
		int ori = getEdgeOri(Edge(e));
		if (ori == -1) return false;
		COLORREF c1 = getColor(EdgeCoord[e][ori]), c2 = getColor(EdgeCoord[e][(ori + 1) % 2]);

		if (c1 == cUp)
		{
			//check up dogs
			for (int ref = UR; ref < DR; ++ref)
			{
				if (c2 == getColor(EdgeCoord[ref][1].s, 1, 1))
			{
					setInfo(EdgeCoord[e][0], ref);
					setInfo(EdgeCoord[e][1], ref);
				}
			}
		}
		else if (c1 == cDown)
		{ 
		  //check down
			for (int ref = DR; ref < FR; ++ref)
			{
				if (c2 == getColor(EdgeCoord[ref][1].s, 1, 1))
				{
					setInfo(EdgeCoord[e][0], ref);
					setInfo(EdgeCoord[e][1], ref);
				}
			}
		}
		else if (c1 == cFront)
		{ 
		  //check front
			for (int ref = FR; ref < BL; ++ref)
			{
				if (c2 == getColor(EdgeCoord[ref][1].s, 1, 1))
				{
					setInfo(EdgeCoord[e][0], ref);
					setInfo(EdgeCoord[e][1], ref);
				}
			}
		}
		else if (c1 == cBack)
		{
			//check front
			for (int ref = BL; ref < NumEdges; ++ref)
			{
				if (c2 == getColor(EdgeCoord[ref][1].s, 1, 1))
				{
					setInfo(EdgeCoord[e][0], ref);
					setInfo(EdgeCoord[e][1], ref);
				}
			}
		}
	}

	//check if piece count correct
	int cornerCheck = 0, cornerConfirm = 0;
	for (int c = 0; c < NumCorners; ++c)
	{
		cornerCheck += 1 << getCornerAt(Corner(c)); 
		cornerConfirm += 1 << c;
	}
	if (cornerCheck != cornerConfirm) return false;

	//check if piece count correct
	int edgeCheck = 0, edgeConfirm = 0;
	for (int e = 0; e < NumEdges; ++e)
	{
		edgeCheck += 1 << getEdgeAt(Edge(e));
		edgeConfirm += 1 << e;
	}
	if (edgeCheck != edgeConfirm) return false;

	//check corner ori
	int cornerOriCheck = 0;
	for (int c = 0; c < NumCorners; ++c)
	{
		cornerOriCheck += getCornerOri(Corner(c));
	}
	if (cornerOriCheck%3 != 0) return false;

	//check edge ori
	int edgeOriCheck = 0;
	for (int e = 0; e < NumEdges; ++e)
	{
		edgeOriCheck += getEdgeOri(Edge(e));
	}
	if (edgeOriCheck % 2 != 0) return false;


	return true;
}

std::vector<MyCube::RotInfo> MyCube::resetOrientation()
{
	std::vector<RotInfo> ret;

	//Make white go up
	if (getInfo(Down, 1, 1) == Up) ret.push_back({ X, 1, 2, true });
	else if (getInfo(Back, 1, 1) == Up) ret.push_back({ X, 1, -1, true });
	else if (getInfo(Front, 1, 1) == Up) ret.push_back({ X, 1, 1, true });
	else if (getInfo(Left, 1, 1) == Up) ret.push_back({ Z, 1, 1, true });
	else if (getInfo(Right, 1, 1) == Up) ret.push_back({ Z, 1, -1, true });

	size_t firstSize = ret.size();
	if (firstSize == 1) turn(ret[0]);

	//make green go front
	if (getInfo(Back, 1, 1) == Front) ret.push_back({ Y, 1, 2, true });
	else if (getInfo(Left, 1, 1) == Front) ret.push_back({ Y, 1, -1, true });
	else if (getInfo(Right, 1, 1) == Front) ret.push_back({ Y, 1, 1, true });

	if(firstSize < ret.size()) turn(ret[firstSize]);

	return ret;
}

std::vector<MyCube::RotInfo> MyCube::solve(int max_moves) const
{
	MyCube c1(*this);
	std::vector<MyCube::RotInfo> ret; //= c1.resetOrientation();

	if (c1.assignPieceInfo())
	{
		std::vector<MyCubeLite::RotLite> solve = c1.getLite().solve(max_moves);


		for (int i = 0; i < solve.size(); ++i)
		{
			ret.push_back(rotLiteToRot(solve[i]));
		}

	}

	return ret;
}

bool MyCube::solved() const
{
	return getLite().solved();
}


int MyCube::getCornerOri(Corner c) const
{
	COLORREF colorUp = getColor(Up, 1, 1), colorDown = getColor(Down, 1, 1);
	COLORREF color1 = getColor(CornerCoord[c][0]), color2 = getColor(CornerCoord[c][1]), color3 = getColor(CornerCoord[c][2]);
	if (color1 == colorUp || color1 == colorDown) return 0;
	if (color2 == colorUp || color2 == colorDown) return 1;
	if (color3 == colorUp || color3 == colorDown) return 2;

	//bad
	return -1;
}

int MyCube::getEdgeOri(Edge e) const
{
	COLORREF colorUp = getColor(Up, 1, 1), colorDown = getColor(Down, 1, 1);
	COLORREF colorFront = getColor(Front, 1, 1), colorBack = getColor(Back, 1, 1);

	COLORREF color1 = getColor(EdgeCoord[e][0]), color2 = getColor(EdgeCoord[e][1]);
	if (color1 == colorUp || color1 == colorDown) return 0;
	if (color2 == colorUp || color2 == colorDown) return 1;
	//Now using green/blue
	if (color1 == colorFront || color1 == colorBack) return 0;
	if (color2 == colorFront || color2 == colorBack) return 1;

	return -1;
}



MyCubeLite::RotLite MyCube::rotToRotLite(RotInfo r)
{
	MyCubeLite::RotLite ret;

	switch (r.m) //F, R, U, L, B, D
	{
	case F:
		ret.m = MyCubeLite::F;
		break;
	case R:
		ret.m = MyCubeLite::R;
		break;
	case U:
		ret.m = MyCubeLite::U;
		break;
	case L:
		ret.m = MyCubeLite::L;
		break;
	case B:
		ret.m = MyCubeLite::B;
		break;
	case D:
		ret.m = MyCubeLite::D;
		break;
	}
	ret.r = (r.cwturn % 4 + 4) % 4;

	return ret;
}

MyCube::RotInfo MyCube::rotLiteToRot(MyCubeLite::RotLite r)
{
	RotInfo ret{ R, 1, 0, true };

	switch (r.m) //F, R, U, L, B, D
	{
	case MyCubeLite::F:
		ret.m = F;
		break;
	case MyCubeLite::R:
		ret.m = R;
		break;
	case MyCubeLite::U:
		ret.m = U;
		break;
	case MyCubeLite::L:
		ret.m = L;
		break;
	case MyCubeLite::B:
		ret.m = B;
		break;
	case MyCubeLite::D:
		ret.m = D;
		break;
	}

	switch (r.r)
	{
	case 1:
	case 2:
		ret.cwturn = r.r;
		break;
	case 3:
		ret.cwturn = -1;
		break;
	}

	return ret;
}
