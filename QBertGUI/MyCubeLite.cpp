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
		m_cs[i] =  c.m_cs[i];
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
		m_cs[i] = CCorner( Corner(i), 0 );
	}
	//edges
	for (int i = 0; i < NumEdges; ++i)
	{
		m_es[i] = CEdge( Edge(i), 0 );
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
		{m_cs[CornerTurnOrder[r.m][4-r.r]], 
		m_cs[CornerTurnOrder[r.m][(5-r.r) & 3]],
		m_cs[CornerTurnOrder[r.m][(6-r.r) & 3]],
		m_cs[CornerTurnOrder[r.m][3-r.r]], };
	CEdge tempe[4] =
		{m_es[EdgeTurnOrder[r.m][4 - r.r]],
		m_es[EdgeTurnOrder[r.m][(5 - r.r) & 3]],
		m_es[EdgeTurnOrder[r.m][(6 - r.r) & 3]],
		m_es[EdgeTurnOrder[r.m][3 - r.r]],};

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
		std::vector<RotLite> r2 = copy.solveFromG1(ret.size(), (ret.empty() ? RotLite{R, 0} : ret.back()), max_moves);
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

	//init the static lists that should store move counts for the variables in those coord triples basically okay bye :)
	//https://www.reddit.com/r/Cubers/comments/63gitx/anyone_here_implemented_kociembas_two_phase/
	//https://www.jaapsch.net/puzzles/compcube.htm#tree

	initSolveArray(CornerOriSteps, _countof(CornerOriSteps), phase1Moves, _countof(phase1Moves), &S_getCornerOriCoord);
	initSolveArray(EdgeOriSteps, _countof(EdgeOriSteps), phase1Moves, _countof(phase1Moves), &S_getEdgeOriCoord);
	initSolveArray(UDSliceSteps, _countof(UDSliceSteps), phase1Moves, _countof(phase1Moves), &S_getUDSliceCoord);
	initSolveArray(CornerPermSteps, _countof(CornerPermSteps), phase2Moves, _countof(phase2Moves), &S_getCornerPermCoord);
	initSolveArray(EdgePermSteps, _countof(EdgePermSteps), phase2Moves, _countof(phase2Moves), &S_getEdgePermCoord);
	initSolveArray(UDSliceSortedSteps, _countof(UDSliceSortedSteps), phase2Moves, _countof(phase2Moves), &S_getUDSliceSortedCoordLite);
}

void MyCubeLite::initSolveArray(int * arr, int len, const RotLite * moves, int movesLen, int(*f)(const MyCubeLite &))
{
	//init array with lowest num turns being the max
	for (int i = 0; i < len; ++i) arr[i] = MAX_MOVES;

	//Create a queue of cube states to try
	std::queue<MyCubeLite> toTry;
	toTry.push(MyCubeLite());
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
		ret = (ret<<1) + m_es[e].o;
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

bool MyCubeLite::isG1() const
{
	return getCornerOriCoord() + getEdgeOriCoord() + getUDSliceCoord() == 0;
}

unsigned MyCubeLite::getCornerPermCoord() const
{
	int ret = 0;
	for (int c = DRB; c > URF; --c)
	{
		ret = (ret + getCornerPerm(Corner(c))) * c;
	}

	return ret;
}

unsigned MyCubeLite::getEdgePermCoord() const
{
	int ret = 0;
	for (int e = DB; e > UR; --e)
	{
		ret = (ret + getEdgePerm(Edge(e))) * e;
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
		if (e >= FR) //== FR || e == FL || e == BL || e == BR)
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
		ret = (ret + s)*(i-FR);
	}

	return ret;
}

bool MyCubeLite::solved() const
{
	return getCornerPermCoord() + getEdgePermCoord() + getUDSliceSortedCoordLite() == 0;
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

std::vector<MyCubeLite::RotLite> MyCubeLite::solveToG1(int max_depth) const
{
	std::vector<RotLite> rots;
	MyCubeLite copy(*this);

	int bound = copy.stepsFromG1();

	while (bound < max_depth)
	{
		bound = copy.searchToG1(rots, 0, bound, max_depth, {R, 0});
	}

	std::vector<RotLite> ret;
	for (int i = rots.size() - 1; i >= 0; --i)
	{
		ret.push_back(rots[i]);
	}

	return ret;
}

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

int MyCubeLite::S_stepsFromSolved(CoordTriple c)
{
	int s1 = CornerPermSteps[c.x1];
	int s2 = EdgePermSteps[c.x2];
	int s3 = UDSliceSortedSteps[c.x3];

	if (s1 > s2 && s1 > s3) return s1;
	if (s2 > s3) return s2;
	return s3;
}

std::vector<MyCubeLite::RotLite> MyCubeLite::solveFromG1(int g, RotLite r, int max_depth) const
{
	std::vector<RotLite> rots;
	MyCubeLite copy(*this);

	if (!copy.isG1()) return rots;

	int bound = g+stepsFromSolved();

	while (bound < max_depth)
	{
		bound = copy.searchFromG1(rots, g, bound, r);
	}

	std::vector<RotLite> ret;
	for (int i = rots.size() - 1; i >= 0; --i)
	{
		ret.push_back(rots[i]);
	}

	return ret;
}

int MyCubeLite::searchFromG1(std::vector<RotLite>& rots, int g, int bound, RotLite lastRot)
{
	int f = g + stepsFromSolved();

	if (f > bound) return f;
	if (solved()) return MAX_MOVES;

	int min = MAX_MOVES;

	for (int i = 0; i < _countof(phase2Moves); ++i)
	{
		RotLite r = phase2Moves[i];
		if (g == 0 || lastRot.m != r.m)
		{
			turn(r);

			int tempMin = searchFromG1(rots, g + 1, bound, r);
			if (tempMin >= MAX_MOVES)
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
