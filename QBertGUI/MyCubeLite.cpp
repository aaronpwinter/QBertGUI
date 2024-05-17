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

void MyCubeLite::test()
{
	m_cs[0] = { Corner(0), 2 };
}
