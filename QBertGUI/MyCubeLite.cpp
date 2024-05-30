#include "stdafx.h"
#include "MyCubeLite.h"



int mathCombo(unsigned n, unsigned k)
{
	if (k == 0) return 1;

	if (k > n / 2) return mathCombo(n, n - k);

	long ret = 1;

	for (unsigned r = 1; r <= k; ++r)
	{
		ret *= n - r + 1;
		ret /= r;
	}

	return ret;
}

using namespace MyCubeDefs;


MyCubeLite::MyCubeLite()
{
	initSolve();
	reset();
}


MyCubeLite::~MyCubeLite()
{
}

MyCubeLite::MyCubeLite(const MyCubeLite & c)
{
	//corners
	for (int i = 0; i < NumCorners; ++i)
	{
		m_cs[i] = c.m_cs[i];
	}
	//edges
	for (int i = 0; i < NumEdges; ++i)
	{
		m_es[i] = c.m_es[i];
	}
}

MyCubeLite & MyCubeLite::operator=(const MyCubeLite & c)
{
	//corners
	for (int i = 0; i < NumCorners; ++i)
	{
		m_cs[i] = c.m_cs[i];
	}
	//edges
	for (int i = 0; i < NumEdges; ++i)
	{
		m_es[i] = c.m_es[i];
	}

	return *this;
}

void MyCubeLite::reset()
{
	//corners
	for (int i = 0; i < NumCorners; ++i)
	{
		m_cs[i] = CCorner(Corner(i), 0);
	}
	//edges
	for (int i = 0; i < NumEdges; ++i)
	{
		m_es[i] = CEdge(Edge(i), 0);
	}
}

void MyCubeLite::turn(RotLite r)
{
	//2 = orientation no change :^)
	//do orientation
	if (r.r == 1 || r.r == 3)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_cs[CornerTurnOrder[r.m][i]].o = (m_cs[CornerTurnOrder[r.m][i]].o + CornerOriChange[r.m][i]) % 3;
			m_es[EdgeTurnOrder[r.m][i]].o = (m_es[EdgeTurnOrder[r.m][i]].o + EdgeOriChange[r.m][i]) & 1;
		}
	}

	//rotate
	CCorner tempc[4] =
	{ m_cs[CornerTurnOrder[r.m][4 - r.r]],
		m_cs[CornerTurnOrder[r.m][(5 - r.r) & 3]],
		m_cs[CornerTurnOrder[r.m][(6 - r.r) & 3]],
		m_cs[CornerTurnOrder[r.m][3 - r.r]], };
	CEdge tempe[4] =
	{ m_es[EdgeTurnOrder[r.m][4 - r.r]],
		m_es[EdgeTurnOrder[r.m][(5 - r.r) & 3]],
		m_es[EdgeTurnOrder[r.m][(6 - r.r) & 3]],
		m_es[EdgeTurnOrder[r.m][3 - r.r]], };

	m_cs[CornerTurnOrder[r.m][0]] = tempc[0];
	m_cs[CornerTurnOrder[r.m][1]] = tempc[1];
	m_cs[CornerTurnOrder[r.m][2]] = tempc[2];
	m_cs[CornerTurnOrder[r.m][3]] = tempc[3];

	m_es[EdgeTurnOrder[r.m][0]] = tempe[0];
	m_es[EdgeTurnOrder[r.m][1]] = tempe[1];
	m_es[EdgeTurnOrder[r.m][2]] = tempe[2];
	m_es[EdgeTurnOrder[r.m][3]] = tempe[3];
}

void MyCubeLite::turn(std::vector<RotLite> rots)
{
	for (unsigned i = 0; i < rots.size(); ++i)
	{
		turn(rots[i]);
	}
}

std::vector<MyCubeLite::RotLite> MyCubeLite::solve(int max_moves) const
{
	std::vector<RotLite> ret = solveToG1(max_moves);

	MyCubeLite copy(*this);
	copy.turn(ret);


	if (copy.isG1())
	{
		std::vector<RotLite> r2 = solveFromG1(copy.getPhase2Coords(), ret.size(), (ret.empty() ? RotLite{ R, 0 } : ret.back()), max_moves);
		copy.turn(r2);
		if (copy.solved())
		{
			ret.insert(ret.end(), r2.begin(), r2.end());
		}
	}

	return ret;
}

unsigned MyCubeLite::getCornerPerm(Corner c) const
{
	int ret = 0;
	Corner cur = m_cs[c].c;
	for (int i = 0; i < c; ++i)
	{
		//if corner at i > corner at c
		if (m_cs[i].c > cur)
			++ret;
	}
	return ret;
}

unsigned MyCubeLite::getEdgePerm(Edge e) const
{
	int ret = 0;
	Edge cur = m_es[e].e;
	for (int i = 0; i < e; ++i)
	{
		//if edge at i > edge at e
		if (m_es[i].e > cur)
			++ret;
	}
	return ret;
}

void MyCubeLite::initSolve()
{
	if (solveInit) return;

	solveInit = true;

	//Move Tables
	initMoveArray(CornerOriMove, _countof(CornerOriMove), phase1Moves, _countof(phase1Moves), &S_getCornerOriCoord);
	initMoveArray(EdgeOriMove, _countof(EdgeOriMove), phase1Moves, _countof(phase1Moves), &S_getEdgeOriCoord);
	initMoveArray(UDSliceMove, _countof(UDSliceMove), phase1Moves, _countof(phase1Moves), &S_getUDSliceCoord);

	initMoveArray(P1CornerPermMove, _countof(P1CornerPermMove), phase1Moves, _countof(phase1Moves), &S_getCornerPermCoord);
	initMoveArray(P1UDSliceSortedMove, _countof(P1UDSliceSortedMove), phase1Moves, _countof(phase1Moves), &S_getUDSliceSortedCoord);
	initMoveArray(P1RLSliceSortedMove, _countof(P1RLSliceSortedMove), phase1Moves, _countof(phase1Moves), &S_getRLSliceSortedCoord);
	initMoveArray(P1FBSliceSortedMove, _countof(P1FBSliceSortedMove), phase1Moves, _countof(phase1Moves), &S_getFBSliceSortedCoord);

	initMoveArray(CornerPermMove, _countof(CornerPermMove), phase2Moves, _countof(phase2Moves), &S_getCornerPermCoord);
	initMoveArray(EdgePermMove, _countof(EdgePermMove), phase2Moves, _countof(phase2Moves), &S_getEdgePermCoord);
	initMoveArray(UDSliceSortedLiteMove, _countof(UDSliceSortedLiteMove), phase2Moves, _countof(phase2Moves), &S_getUDSliceSortedCoordLite);

	initEdge8Perm();

	//Prune Tables
	//init the static lists that should store move counts for the variables in those coord triples basically okay bye :)
	//https://www.reddit.com/r/Cubers/comments/63gitx/anyone_here_implemented_kociembas_two_phase/
	//https://www.jaapsch.net/puzzles/compcube.htm#tree

	initPruneArray(CornerOriSteps, _countof(CornerOriSteps), phase1Moves, _countof(phase1Moves), &S_getCornerOriCoord);
	initPruneArray(EdgeOriSteps, _countof(EdgeOriSteps), phase1Moves, _countof(phase1Moves), &S_getEdgeOriCoord);
	initPruneArray(UDSliceSteps, _countof(UDSliceSteps), phase1Moves, _countof(phase1Moves), &S_getUDSliceCoord);
	initPruneArray(CornerPermSteps, _countof(CornerPermSteps), phase2Moves, _countof(phase2Moves), &S_getCornerPermCoord);
	initPruneArray(EdgePermSteps, _countof(EdgePermSteps), phase2Moves, _countof(phase2Moves), &S_getEdgePermCoord);
	initPruneArray(UDSliceSortedLiteSteps, _countof(UDSliceSortedLiteSteps), phase2Moves, _countof(phase2Moves), &S_getUDSliceSortedCoordLite);
}

void MyCubeLite::initMoveArray(unsigned * arr, int len, const RotLite * moves, int movesLen, unsigned(*f)(const MyCubeLite &))
{
	unsigned mainLen = len / movesLen;

	//init array with lowest num turns being the max
	for (int i = 0; i < len; ++i)
	{
		arr[i] = -1;
	}


	//Create a queue of cube states to try
	std::queue<MyCubeLite> toTry;
	toTry.push(MyCubeLite());

	int found = 0;

	//keep going until no more cubes or no more moves
	while (!toTry.empty() && found < mainLen)
	{
		MyCubeLite front(toTry.front());
		unsigned coord = f(front);

		if (arr[coord*movesLen] == -1)
		{
			found++;
			//try all possible moves on current cube state
			for (int i = 0; i < movesLen; ++i)
			{
				MyCubeLite temp(front);

				temp.turn(moves[i]);
				int tempVal = f(temp);

				arr[coord*movesLen + i] = tempVal;

				//if the value for this TURNED cube state is less than (honestly only happens on MAX_MOVES) the currently logged value...
				if (arr[tempVal*movesLen] == -1)
				{
					toTry.push(temp);
				}
			}
		}

		//next cube !!
		toTry.pop();
	}
}

void MyCubeLite::initEdge8Perm()
{
	int len = _countof(GetEdge8Perm);

	//init array with lowest num turns being the max
	for (int i = 0; i < len; ++i)
	{
		GetEdge8Perm[i] = -1;
	}


	//Create a queue of cube states to try
	std::queue<MyCubeLite> toTry;
	toTry.push(MyCubeLite());

	int found = 0;

	//keep going until no more cubes or no more moves
	while (!toTry.empty() && found < len)
	{
		MyCubeLite front(toTry.front());
		//try all possible moves on current cube state
		for (int i = 0; i < _countof(phase2Moves); ++i)
		{
			MyCubeLite temp(front);
			temp.turn(phase2Moves[i]);

			unsigned rlCoord = temp.getRLSliceSortedCoord();
			unsigned fbCoord = temp.getFBSliceSortedCoord();

			if (GetEdge8Perm[rlCoord*24 + fbCoord%24] == -1)
			{
				GetEdge8Perm[rlCoord * 24 + fbCoord % 24] = temp.getEdgePermCoord();

				toTry.push(temp);
				found++;				
			}
		}

		//next cube !!
		toTry.pop();
	}
}

void MyCubeLite::initPruneArray(byte * arr, int len, const RotLite * moves, int movesLen, unsigned(*f)(const MyCubeLite &))
{
	//init array with lowest num turns being the max
	for (int i = 0; i < len; ++i) arr[i] = MAX_MOVES;

	//Create a queue of cube states to try
	std::queue<MyCubeLite> toTry;
	toTry.push(MyCubeLite());
	//bookkeeping, the current move count and the remaining number of cubes for this move count
	int curMoves = -1, remaining = 0, found = 1;
	arr[f(toTry.front())] = 0; //set 0 value to 0 moves :)

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
			MyCubeLite temp(toTry.front());

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

unsigned MyCubeLite::getCornerOriCoord() const
{
	unsigned ret = 0;
	for (unsigned c = 0; c < DRB; ++c)
	{
		ret = 3 * ret + m_cs[c].o;
	}
	return ret;
}

unsigned MyCubeLite::getEdgeOriCoord() const
{
	unsigned ret = 0;
	for (unsigned e = 0; e < BR; ++e)
	{
		ret = (ret << 1) + m_es[e].o;
	}
	return ret;
}

unsigned MyCubeLite::getUDSliceCoord() const
{
	int ret = 0, n = NumEdges - 1, k = 3;

	while (k >= 0 && n >= 0)
	{
		//If edge at e is FR or after, then do funny business
		if (m_es[n].e >= FR) --k;
		else ret += mathCombo(n, k);

		--n;
	}

	return ret;
}

unsigned MyCubeLite::getRLSliceCoord() const
{
	int ret = 0, n = NumEdges - 1, k = 3;

	while (k >= 0 && n >= 0)
	{
		Edge e = m_es[n].e;
		if (e == UF || e == UB || e == DB || e == DF) --k;
		else ret += mathCombo(n, k);

		--n;
	}

	return ret;
}

unsigned MyCubeLite::getFBSliceCoord() const
{
	int ret = 0, n = NumEdges - 1, k = 3;

	while (k >= 0 && n >= 0)
	{
		Edge e = m_es[n].e;
		if (e == UR || e == UL || e == DL || e == DR) --k;
		else ret += mathCombo(n, k);

		--n;
	}

	return ret;
}

unsigned MyCubeLite::getFlipUDSliceCoord() const
{
	/*
	function CubieCube.FlipUDSliceCoord: Integer;
	var k, n, coord: Integer; prod: EdgeCubie; c, d: CubieCube;
	begin
		c : = CubieCube.Create;
		d: = CubieCube.Create;
		c.InvUDSliceCoord(UDSliceCoord);
		c.InvEdgeOriCoord(EdgeOriCoord);
		Result: = -1;
		for k: = 0 to 15 do
			begin
				EdgeMult(EdgeSym[k], c.PEdge^, prod);
				EdgeMult(prod, EdgeSym[InvIdx[k]], d.PEdge^);
				n: = 2048 * d.UDSliceCoord + d.EdgeOriCoord;//raw coordinate
				coord: = FlipUDSliceRawCoordClassIndex(n);
				if coord<>-1 then
					begin
						Result : = coord shl 4 + k;
						break;
					end;
			end;
		assert(Result<>-1);
		c.Free;
		d.Free;
	end;
	*/
	int ret = -1;
	int coord;
	MyCubeLite c(*this);

	for (int i = 0; i < 16; ++i)
	{
		//MyCubeLite temp = EdgeMult(EdgeSym[i], c);
		//MyCubeLite d = EdgeMult(temp, EdgeSym[InvIdx[i]]);
		//int n = 2048 * d.getUDSliceCoord() + d.getEdgeOriCoord();
		//coord = FlipUDSliceRawCoordClassIndex(n);
		//if (coord = !- 1)
		//{
		//	ret = coord<<4 + i;
		//	break;
		//}
	}
	return ret;
}

CoordTriple MyCubeLite::turnPhase1Coords(CoordTriple coords, int phase1MovesIndex)
{
	return { CornerOriMove[coords.x1*_countof(phase1Moves) + phase1MovesIndex],
		EdgeOriMove[coords.x2*_countof(phase1Moves) + phase1MovesIndex],
		UDSliceMove[coords.x3*_countof(phase1Moves) + phase1MovesIndex] };
}

MyCubeLite::Phase1Info MyCubeLite::turnPhase1Info(Phase1Info info, int phase1MovesIndex)
{
	return { P1CornerPermMove[info.cornerPerm*_countof(phase1Moves) + phase1MovesIndex],
		P1UDSliceSortedMove[info.udSlice*_countof(phase1Moves) + phase1MovesIndex],
		P1RLSliceSortedMove[info.rlSlice*_countof(phase1Moves) + phase1MovesIndex],
		P1FBSliceSortedMove[info.fbSlice*_countof(phase1Moves) + phase1MovesIndex] };
}

bool MyCubeLite::isG1() const
{
	return getCornerOriCoord() + getEdgeOriCoord() + getUDSliceCoord() == 0;
}

bool MyCubeLite::isG1(CoordTriple coords)
{
	return coords.x1 + coords.x2 + coords.x3 == 0;
}

unsigned MyCubeLite::getCornerPermCoord() const
{
	int ret = 0;
	for (int c = DRB; c > URF; --c)
	{
		ret = (ret + getCornerPerm(Corner(c))) * c;
		//ret = (ret + m_cs[c].c)*c;
	}

	return ret;
}

unsigned MyCubeLite::getEdgePermCoord() const
{
	int ret = 0;
	for (int e = DB; e > UR; --e)
	{
		ret = (ret + getEdgePerm(Edge(e))) * e;
		//ret = (ret + m_es[e].e)*e;
	}

	return ret;
}

unsigned MyCubeLite::getUDSliceSortedCoord() const
{
	int ret = 0;

	Edge edges[4];
	int j = 0;
	for (int i = UR; i <= BR; ++i)
	{
		Edge e = m_es[i].e;
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

	return getUDSliceCoord() * 24 + ret;
}

unsigned MyCubeLite::getRLSliceSortedCoord() const
{
	int ret = 0;

	Edge edges[4];
	int j = 0;
	for (int i = UR; i <= BR; ++i)
	{
		Edge e = m_es[i].e;
		if (e == UF || e == UB || e == DB || e == DF)
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

	return getRLSliceCoord() * 24 + ret;
}

unsigned MyCubeLite::getFBSliceSortedCoord() const
{
	int ret = 0;

	Edge edges[4];
	int j = 0;
	for (int i = UR; i <= BR; ++i)
	{
		Edge e = m_es[i].e;
		if (e == UR || e == UL || e == DL || e == DR)
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

	return getFBSliceCoord() * 24 + ret;
}

unsigned MyCubeLite::getUDSliceSortedCoordLite() const
{
	int ret = 0;

	for (int i = BR; i >= FL; --i)
	{
		int s = 0;
		for (int j = i - 1; j >= FR; --j)
		{
			if (m_es[j].e > m_es[i].e) ++s;
		}
		ret = (ret + s)*(i - FR);
	}

	return ret;
}

CoordTriple MyCubeLite::turnPhase2Coords(CoordTriple coords, int phase2MovesIndex)
{
	return { CornerPermMove[coords.x1*_countof(phase2Moves) + phase2MovesIndex],
		EdgePermMove[coords.x2*_countof(phase2Moves) + phase2MovesIndex],
		UDSliceSortedLiteMove[coords.x3*_countof(phase2Moves) + phase2MovesIndex] };
}

unsigned MyCubeLite::RLFBSliceSortedToEdgePerm(unsigned rlSlice, unsigned fbSlice)
{
	return GetEdge8Perm[rlSlice * 24 + fbSlice % 24];
}

CoordTriple MyCubeLite::phase1InfoToPhase2Coords(Phase1Info p1Info)
{
	return { p1Info.cornerPerm, RLFBSliceSortedToEdgePerm(p1Info), p1Info.udSlice };
}

bool MyCubeLite::solved() const
{
	return getCornerPermCoord() + getEdgePermCoord() + getUDSliceSortedCoordLite() == 0;
}

bool MyCubeLite::solved(CoordTriple coords)
{
	return coords.x1 + coords.x2 + coords.x3 == 0;
}

int MyCubeLite::S_stepsFromG1(CoordTriple c)
{
	int s1 = CornerOriSteps[c.x1];
	int s2 = EdgeOriSteps[c.x2];
	int s3 = UDSliceSteps[c.x3];

	if (s1 > s2 && s1 > s3) return s1;
	if (s2 > s3) return s2;
	return s3;
}

int MyCubeLite::stepsFromG1() const
{
	int s1 = CornerOriSteps[getCornerOriCoord()];
	int s2 = EdgeOriSteps[getEdgeOriCoord()];
	int s3 = UDSliceSteps[getUDSliceCoord()];

	if (s1 > s2 && s1 > s3) return s1;
	if (s2 > s3) return s2;
	return s3;
}

std::vector<MyCubeLite::RotLite> MyCubeLite::solveToG1(int max_depth) const
{
	std::vector<RotLite> rots;
	int bound = stepsFromG1();

	while (bound <= max_depth)
	{
		bound = searchToG1(rots, getPhase1Coords(), getPhase1Info(), 0, bound, max_depth, { R, 0 });
	}

	std::vector<RotLite> ret;
	for (int i = rots.size() - 1; i >= 0; --i)
	{
		ret.push_back(rots[i]);
	}

	return ret;
}

/*
int MyCubeLite::searchToG1(std::vector<RotLite>& rots, int g, int bound, int max_depth, RotLite lastRot)
{
	int f = g + stepsFromG1();

	if (f > bound) return f;
	if (isG1()) return MAX_MOVES;

	int min = MAX_MOVES;

	for (int i = 0; i < _countof(phase1Moves); ++i)
	{
		RotLite r = phase1Moves[i];
		if (g == 0 || lastRot.m != r.m)
		{

			turn(r);

			int tempMin = searchToG1(rots, g + 1, bound, max_depth, r);
			if (tempMin >= MAX_MOVES)
				//{
				//	rots.push_back(r);
				//	return tempMin;
				//}

			{
				if (isG1())
				{
					MyCubeLite temp(*this);
					std::vector<RotLite> r2 = temp.solveFromG1(g, r, max_depth);
					temp.turn(r2);
					if (temp.solved())
					{
						turn(r.reversed());
						for (int i = r2.size() - 1; i >= 0; --i)
						{
							rots.push_back(r2[i]);
						}
						rots.push_back(r);
						return tempMin;
					}
				}
				else
				{
					rots.push_back(r);
					return tempMin;
				}

			}

			if (tempMin < min) min = tempMin;

			turn(r.reversed());
		}
	}
	return min;
}
*/

int MyCubeLite::searchToG1(std::vector<RotLite>& rots, CoordTriple coords, Phase1Info p1Info, int g, int bound, int max_depth, RotLite lastRot)
{
	int f = g + S_stepsFromG1(coords);

	if (f > bound) return f;
	if (isG1(coords)) return max_depth +1;

	int min = max_depth+1;

	for (int i = 0; i < _countof(phase1Moves); ++i)
	{
		RotLite r = phase1Moves[i];
		if (g == 0 || lastRot.m != r.m)
		{
			//rots.push_back(r);
			CoordTriple newCoords = turnPhase1Coords(coords, i);
			Phase1Info newInfo = turnPhase1Info(p1Info, i);

			int tempMin = searchToG1(rots, newCoords, newInfo, g + 1, bound, max_depth, r);
			if (tempMin > max_depth)
			{
				if (isG1(newCoords))
				{
					CoordTriple p2 = phase1InfoToPhase2Coords(newInfo);
					std::vector<RotLite> r2 = solveFromG1(p2, g, r, max_depth);
					if(solved(p2) || !r2.empty())
					{
						for (int j = 0; j < r2.size(); ++j)
						{
							rots.push_back(r2[r2.size()-j-1]);
						}
						rots.push_back(r);
						return tempMin;
					}
				}
				else
				{
					rots.push_back(r);
					return tempMin;
				}

			}

			if (tempMin < min) min = tempMin;

			//rots.pop_back();
		}
	}
	return min;
}

int MyCubeLite::S_stepsFromSolved(CoordTriple c)
{
	int s1 = CornerPermSteps[c.x1];
	int s2 = EdgePermSteps[c.x2];
	int s3 = UDSliceSortedLiteSteps[c.x3];

	if (s1 > s2 && s1 > s3) return s1;
	if (s2 > s3) return s2;
	return s3;
}

int MyCubeLite::stepsFromSolved() const
{
	int s1 = CornerPermSteps[getCornerPermCoord()];
	int s2 = EdgePermSteps[getEdgePermCoord()];
	int s3 = UDSliceSortedLiteSteps[getUDSliceSortedCoordLite()];

	if (s1 > s2 && s1 > s3) return s1;
	if (s2 > s3) return s2;
	return s3;
}

/*
std::vector<MyCubeLite::RotLite> MyCubeLite::solveFromG1(int g, RotLite r, int max_depth) const
{
	std::vector<RotLite> rots;
	MyCubeLite copy(*this);

	if (!copy.isG1()) return rots;

	int bound = g + stepsFromSolved();

	while (bound < max_depth)
	{
		//bound = copy.searchFromG1(rots, g, bound, r);
		bound = copy.searchFromG1(rots, copy.getPhase2Coords(), g, bound, r, max_depth);
	}

	std::vector<RotLite> ret;
	for (int i = rots.size() - 1; i >= 0; --i)
	{
		ret.push_back(rots[i]);
	}

	return ret;
}
*/

std::vector<MyCubeLite::RotLite> MyCubeLite::solveFromG1(CoordTriple coords, int g, RotLite r, int max_depth)
{
	std::vector<RotLite> rots;

	int bound = g + S_stepsFromSolved(coords);

	while (bound <= max_depth)
	{
		//bound = copy.searchFromG1(rots, g, bound, r);
		bound = searchFromG1(rots, coords, g, bound, r, max_depth);
	}

	std::vector<RotLite> ret;
	for (int i = rots.size() - 1; i >= 0; --i)
	{
		ret.push_back(rots[i]);
	}

	return ret;
}

/*
int MyCubeLite::searchFromG1(std::vector<RotLite>& rots, int g, int bound, RotLite lastRot, int max_depth)
{
	int f = g + stepsFromSolved();

	if (f > bound) return f;
	if (solved()) return max_depth+1;

	int min = max_depth+1;

	for (int i = 0; i < _countof(phase2Moves); ++i)
	{
		RotLite r = phase2Moves[i];
		if (g == 0 || lastRot.m != r.m)
		{
			turn(r);

			int tempMin = searchFromG1(rots, g + 1, bound, r,max_depth);
			if (tempMin > max_depth)
			{
				rots.push_back(r);
				return tempMin;
			}
			if (tempMin < min) min = tempMin;

			turn(r.reversed());
		}
	}
	return min;
}
*/

int MyCubeLite::searchFromG1(std::vector<RotLite>& rots, CoordTriple coords, int g, int bound, RotLite lastRot, int max_depth)
{
	int f = g + S_stepsFromSolved(coords);

	if (f > bound) return f;
	if (solved(coords)) return max_depth+ MAX_MOVES;

	int min = max_depth+MAX_MOVES;

	for (int i = 0; i < _countof(phase2Moves); ++i)
	{
		RotLite r = phase2Moves[i];
		if (g == 0 || lastRot.m != r.m)
		{
			CoordTriple newCoords = turnPhase2Coords(coords, i);

			int tempMin = searchFromG1(rots, newCoords, g + 1, bound, r,max_depth);
			if (tempMin > max_depth)
			{
				if(tempMin == max_depth + MAX_MOVES) rots.push_back(r);
				return tempMin;
			}
			if (tempMin < min) min = tempMin;
		}
	}
	return min;
}

MyCubeLite MyCubeLite::EdgeMult(const MyCubeLite& a, const MyCubeLite& b)
{
	MyCubeLite ret;

	for (int e = UR; e < NumEdges; ++e)
	{
		ret.m_es[e].e = a.m_es[b.m_es[e].e].e;
		int ori = b.m_es[e].o + a.m_es[b.m_es[e].e].o;
		if (ori == 2) ori = 0;
		ret.m_es[e].o = ori;
	}

	return ret;
}

void MyCubeLite::test()
{
	m_cs[0] = { Corner(0), 2 };
}

void MyCubeLite::setCorner(Corner toSet, Corner c, unsigned o)
{
	m_cs[toSet] = { c, o % 3 };
}

void MyCubeLite::setEdge(Edge toSet, Edge e, unsigned o)
{
	m_es[toSet] = { e, o % 2 };
}