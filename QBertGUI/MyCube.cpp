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

void MyCube::assignPieceInfo()
{
	//For any cube, assign the URF, UD... to all of the facelets
	//yeah idk how to do this at all but you know :) !! If i need to !!
}

std::vector<MyCube::RotInfo> MyCube::resetOrientation()
{
	std::vector<RotInfo> ret;

	//Make white go up
	if (getFacelet(Down, 1, 1).info == Up) ret.push_back({ X, 1, 2, true });
	else if (getFacelet(Back, 1, 1).info == Up) ret.push_back({ X, 1, -1, true });
	else if (getFacelet(Front, 1, 1).info == Up) ret.push_back({ X, 1, 1, true });
	else if (getFacelet(Left, 1, 1).info == Up) ret.push_back({ Z, 1, 1, true });
	else if (getFacelet(Right, 1, 1).info == Up) ret.push_back({ Z, 1, -1, true });

	size_t firstSize = ret.size();
	if (firstSize == 1) turn(ret[0]);

	//make green go front
	if (getFacelet(Back, 1, 1).info == Front) ret.push_back({ Y, 1, 2, true });
	else if (getFacelet(Left, 1, 1).info == Front) ret.push_back({ Y, 1, -1, true });
	else if (getFacelet(Right, 1, 1).info == Front) ret.push_back({ Y, 1, 1, true });

	if(firstSize < ret.size()) turn(ret[firstSize]);

	return ret;
}

std::vector<MyCube::RotInfo> MyCube::solve()
{
	MyCube c1(*this);
	std::vector<MyCube::RotInfo> ret = c1.resetOrientation();

	std::vector<MyCube::RotInfo> g1 = c1.solveToG1();
	ret.insert(ret.end(), g1.begin(), g1.end());

	MyCube c2(*this);

	for (unsigned i = 0; i < ret.size(); ++i)
	{
		c2.turn(ret[i]);
	}

	if (!c2.isG1()) return ret;

	std::vector<MyCube::RotInfo> solve2 = c2.solveFromG1();
	ret.insert(ret.end(), solve2.begin(), solve2.end());

	return ret;
}

void MyCube::initSolve()
{
	if (solveInit) return;

	solveInit = true;

	//init the static lists that should store move counts for the variables in those coord triples basically okay bye :)
	//https://www.reddit.com/r/Cubers/comments/63gitx/anyone_here_implemented_kociembas_two_phase/
	//https://www.jaapsch.net/puzzles/compcube.htm#tree

	initSolveArray(CornerOriSteps, _countof(CornerOriSteps), phase1Moves, _countof(phase1Moves), &S_getCornerOriCoord);
	initSolveArray(EdgeOriSteps, _countof(EdgeOriSteps), phase1Moves, _countof(phase1Moves), &S_getEdgeOriCoord);
	initSolveArray(UDSliceSteps, _countof(UDSliceSteps), phase1Moves, _countof(phase1Moves), &S_getUDSliceCoord);
	initSolveArray(CornerPermSteps, _countof(CornerPermSteps), phase2Moves, _countof(phase2Moves), &S_getCornerPermCoord);
	initSolveArray(EdgePermSteps, _countof(EdgePermSteps), phase2Moves, _countof(phase2Moves), &S_getEdgePermCoord);
	initSolveArray(UDSliceSortedSteps, _countof(UDSliceSortedSteps), phase2Moves, _countof(phase2Moves), &S_getUDSliceSortedCoord);
	int aaron = 0;
	

}

//This basically puts the estimated lowest number of turns to get to a certain value for the various different values
void MyCube::initSolveArray(int * arr, int len, const RotInfo * moves, int movesLen, int(*f)(MyCube&))
{
	//init array with lowest num turns being the max
	for (int i = 0; i < len; ++i) arr[i] = MAX_MOVES;

	//Create a queue of cube states to try
	std::queue<MyCube> toTry;
	toTry.push(MyCube());
	//bookkeeping, the current move count and the remaining number of cubes for this move count
	int curMoves = -1, remaining = 0, found = 1;
	arr[0] = 0; //set 0 value to 0 moves :)

	//keep going until no more cubes or no more moves
	while (!toTry.empty() && curMoves < MAX_MOVES && found < len)
	{
		//if no more cubes left in this move count, increment move count
		if (remaining <= 0)
		{
			++curMoves;
			remaining = int(toTry.size());
		}

		//try all possible moves on current cube state
		for (int i = 0; i < movesLen; ++i)
		{
			MyCube temp(toTry.front());

			temp.turn(moves[i]);
			int tempVal = f(temp);

			//if the value for this TURNED cube state is less than (honestly only happens on MAX_MOVES) the currently logged value...
			if (curMoves + 1 < arr[tempVal])
			{
				// ...then replace logged value and add cube state
				arr[tempVal] = curMoves + 1;
				toTry.push(temp);

				++found;
			}
		}

		//next cube !!
		toTry.pop();
		--remaining;
	}
}


MyCube::Corner MyCube::getCornerAt(Corner c)
{
	return Corner(getFacelet(CornerCoord[c][0]).info);
}

MyCube::Edge MyCube::getEdgeAt(Edge e)
{
	return Edge(getFacelet(EdgeCoord[e][0]).info);
}

int MyCube::getCornerOri(Corner c)
{
	Color color1 = get(CornerCoord[c][0]), color2 = get(CornerCoord[c][1]), color3 = get(CornerCoord[c][2]);
	if (color1 == White || color1 == Yellow) return 0;
	if (color2 == White || color2 == Yellow) return 1;
	if (color3 == White || color3 == Yellow) return 2;

	//bad
	return -1;
}

int MyCube::getEdgeOri(Edge e)
{
	Color color1 = get(EdgeCoord[e][0]), color2 = get(EdgeCoord[e][1]);
	if (color1 == White || color1 == Yellow) return 0;
	if (color2 == White || color2 == Yellow) return 1;
	//Now using green/blue
	if (color1 == Green || color1 == Blue) return 0;
	if (color2 == Green || color2 == Blue) return 1;

	return -1;
}

int MyCube::getCornerPerm(Corner c)
{
	int ret = 0;
	Corner cur = getCornerAt(c);
	for (int i = 0; i < c; ++i)
	{
		//if corner at i > corner at c
		if(getCornerAt(Corner(i)) > cur)
			++ret;
	}
	return ret;
}

int MyCube::getEdgePerm(Edge e)
{
	int ret = 0;
	Edge cur = getEdgeAt(e);
	for (int i = 0; i < e; ++i)
	{
		//if corner at i > corner at c
		if (getEdgeAt(Edge(i)) > cur)
			++ret;
	}
	return ret;
}

int MyCube::getCornerOriCoord()
{
	/*
	https://kociemba.org/math/coordlevel.htm#cornoridef
	function CubieCube.CornOriCoord:Word;
	var co: Corner; s: Word;
	begin
	s:=0;
	for co:= URF to Pred(DRB) do s:= 3*s + PCorn^[co].o;
	Result:= s;
	end;
	*/
	int ret = 0;
	for (int c = 0; c < DRB; ++c)
	{
		ret = 3 * ret + getCornerOri(Corner(c));
	}

	return ret;
}

int MyCube::getEdgeOriCoord()
{
	/*
	https://kociemba.org/math/coordlevel.htm#edgeoridef
	function CubieCube.EdgeOriCoord:Word;
	var ed: Edge; s: Word;
	begin
	s:=0;
	for ed:= UR to Pred(BR) do s:= 2*s + PEdge^[ed].o;
	Result:= s;
	end;
	*/
	int ret = 0;
	for (int e = 0; e < BR; ++e)
	{
		ret = 2 * ret + getEdgeOri(Edge(e));
	}

	return ret;
}

int MyCube::getUDSliceCoord()
{
	/*
	https://kociemba.org/math/twophase.htm#udslicedef
	function CubieCube.UDSliceCoord;
	var s: Word; k,n: Integer; occupied: array[0..11] of boolean; ed: Edge;
	begin
	for n:= 0 to 11 do occupied[n]:=false;
	for ed:=UR to BR do if PEdge^[ed].e >= FR then occupied[Word(ed)]:=true;
	s:=0; k:=3; n:=11;
	while k>= 0 do
	begin
	if occupied[n] then Dec(k)
	else s:= s + C(n,k);
	Dec(n);
	end;
	Result:= s;
	end;
	*/
	int ret = 0, n = NumEdges-1, k = 3;

	bool udSliceIn[NumEdges];
	for (int e = 0; e < NumEdges; ++e)
	{
		udSliceIn[e] = getEdgeAt(Edge(e)) >= FR;
	}

	while (k >= 0 && n >= 0)
	{
		if (udSliceIn[n]) --k;
		else ret += mathCombo(n, k);

		--n;
	}

	return ret;
}

bool MyCube::isG1()
{
	CoordTriple c = getPhase1Coords();
	return (c.x1 == 0) && (c.x2 == 0) && (c.x3 == 0);
}

int MyCube::S_stepsFromG1(CoordTriple c)
{
	int s1 = CornerOriSteps[c.x1];
	int s2 = EdgeOriSteps[c.x2];
	int s3 = UDSliceSteps[c.x3];

	if (s1 > s2 && s1 > s3) return s1;
	if (s2 > s3) return s2;
	return s3;
}

std::vector<MyCube::RotInfo> MyCube::solveToG1()
{
	std::vector<RotInfo> rots;

	int bound = stepsFromG1();

	while (bound < MAX_MOVES)
	{
		bound = searchToG1(rots, 0, bound);
	}

	return rots;
}

int MyCube::searchToG1(std::vector<RotInfo>& rots, int g, int bound)
{
	int f = g + stepsFromG1();

	if (f > bound) return f;
	if (isG1()) return MAX_MOVES;

	int min = MAX_MOVES;

	for (int i = 0; i < _countof(phase1Moves); ++i)
	{
		RotInfo r = phase1Moves[i];
		if (rots.empty() || rots.back().m != r.m)
		{
			rots.push_back(r);

			MyCube temp(*this);
			temp.turn(r);

			int tempMin = temp.searchToG1(rots, g + 1, bound);
			if (tempMin >= MAX_MOVES) return tempMin;
			if (tempMin < min) min = tempMin;

			rots.pop_back();
		}
	}
	return min;
}


int MyCube::getCornerPermCoord()
{
	/*
	https://kociemba.org/math/coordlevel.htm#cornpermdef
	function CubieCube.CornPermCoord: Word;
	var i,j: Corner; x,s: Integer;
	begin
	x:= 0;
	for i:= DRB downto Succ(URF) do
	begin
	s:=0;
	for j:= Pred(i) downto URF do
	begin
	if PCorn^[j].c>PCorn^[i].c then Inc(s);
	end;
	x:= (x+s)*Ord(i);
	end;
	Result:=x;
	end;
	*/
	int ret = 0;
	for (int c = DRB; c > URF; --c)
	{
		ret = (ret + getCornerPerm(Corner(c))) * c;
	}

	return ret;
}

int MyCube::getEdgePermCoord()
{
	/*
	https://kociemba.org/math/twophase.htm#phase2edge
	function CubieCube.Phase2EdgePermCoord: Word;
	var i,j: Edge; x,s: Integer;
	begin
	x:= 0;
	for i:= DB downto Succ(UR) do
	begin
	s:=0;
	for j:= Pred(i) downto UR do
	begin
	if PEdge^[j].e>PEdge^[i].e then Inc(s);
	end;
	x:= (x+s)*Ord(i);
	end;
	Result:=x;
	end;
	*/
	int ret = 0;
	for (int e = DB; e > UR; --e)
	{
		ret = (ret + getEdgePerm(Edge(e))) * e;
	}

	return ret;
}

int MyCube::getUDSliceSortedCoord()
{
	/*
	https://kociemba.org/math/twophase.htm#phase2udslice
	function CubieCube.UDSliceSortedCoord: Word;

	var j,k,s,x: Integer; i,e: Edge; arr: array[0..3] of Edge;
	begin
	j:=0;
	for i:= UR to BR do
	begin
	e:=PEdge^[i].e;
	if (e=FR) or (e=FL) or (e=BL) or (e=BR) then begin arr[j]:= e; Inc(j); end;
	end;

	x:= 0;
	for j:= 3 downto 1 do
	begin
	s:=0;
	for k:= j-1 downto 0 do
	begin
	if arr[k]>arr[j] then Inc(s);
	end;
	x:= (x+s)*j;
	end;
	Result:= UDSliceCoord*24 + x;
	end;
	*/
	int ret = 0;

	Edge edges[4];
	int j = 0;
	for (int i = UR; i <= BR; ++i)
	{
		Edge e = getEdgeAt(Edge(i));
		if (e == FR || e == FL || e == BL || e == BR)
		{
			edges[j] = e;
			++j;
		}
	}

	for (int i = 3; i >= 1; --i)
	{
		int s = 0;
		for (int j = i - 1; j >= 0; --j)
		{
			if (edges[j] > edges[i]) ++s;
		}
		ret = (ret + s)*i;
	}

	return getUDSliceCoord()*24 + ret;
}

bool MyCube::solved()
{
	CoordTriple c = getPhase2Coords();
	return (c.x1 == 0) && (c.x2 == 0) && (c.x3 == 0);
}

int MyCube::S_stepsFromSolved(CoordTriple c)
{
	int s1 = CornerPermSteps[c.x1];
	int s2 = EdgePermSteps[c.x2];
	int s3 = UDSliceSortedSteps[c.x3];

	if (s1 > s2 && s1 > s3) return s1;
	if (s2 > s3) return s2;
	return s3;
}

std::vector<MyCube::RotInfo> MyCube::solveFromG1()
{
	std::vector<RotInfo> rots;

	if (!isG1()) return rots;

	int bound = stepsFromSolved();

	while (bound < MAX_MOVES)
	{
		bound = searchFromG1(rots, 0, bound);
	}

	return rots;
}

int MyCube::searchFromG1(std::vector<RotInfo>& rots, int g, int bound)
{
	int f = g + stepsFromSolved();

	if (f > bound) return f;
	if (solved()) return MAX_MOVES;

	int min = MAX_MOVES;

	for (int i = 0; i < _countof(phase2Moves); ++i)
	{
		RotInfo r = phase2Moves[i];
		if (rots.empty() || rots.back().m != r.m)
		{
			rots.push_back(r);

			MyCube temp(*this);
			temp.turn(r);

			int tempMin = temp.searchFromG1(rots, g + 1, bound);
			if (tempMin >= MAX_MOVES) return tempMin;
			if (tempMin < min) min = tempMin;

			rots.pop_back();
		}
	}
	return min;
}
