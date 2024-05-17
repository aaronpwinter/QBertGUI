#pragma once

namespace MyCubeDefs
{

	//--ENUM
	enum Corner { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB, NumCorners };
	enum Edge { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR, NumEdges };

	//--STRUCT
	struct CoordTriple
	{
		unsigned x1, x2, x3;
	};


}