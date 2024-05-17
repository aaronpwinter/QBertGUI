#include "stdafx.h"
#include "FunnyCube.h"

#include <windows.h> 
#include <dos.h>
#include <iostream>

#include <vector>
#include <string>
#include <regex>

#include <algorithm> 
#include <cwctype>
#include <locale>

#include <sstream>

//https://stackoverflow.com/questions/216823/how-to-trim-a-stdstring
// trim from start (in place)
inline void ltrim(std::wstring &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](wchar_t ch) {
		return !std::iswspace(ch);
	}));
}

// trim from end (in place)
inline void rtrim(std::wstring &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](wchar_t ch) {
		return !std::iswspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::wstring &s) {
	rtrim(s);
	ltrim(s);
}

/*
std::vector<std::wstring> split(const std::wstring &s, wchar_t delim) {
	std::vector<std::wstring> elems;
	std::wstringstream ss(s);
	std::wstring item;
	while (std::getline(ss, item, delim)) {
		if (!item.empty()) {
			elems.push_back(item);
		}
	}
	return elems;
}
*/

void FunnyCube::reset()
{
	int baseWhite = Side::Up * m_size2, //Up = White
		baseGreen = Side::Front * m_size2, //Front = Green
		baseRed = Side::Right * m_size2, //Right = Red
		baseBlue = Side::Back * m_size2, //Back = Blue
		baseOrange = Side::Left * m_size2, //Left = Orange
		baseYellow = Side::Down * m_size2; //Down = Yellow

	for (int i = 0; i < m_size2; ++i)
	{
		int row = i / m_size, col = i % m_size;

		m_cube[baseWhite + i] = { Color::White, row, col };
		m_cube[baseGreen + i] = { Color::Green, row, col };
		m_cube[baseRed + i] = { Color::Red, row, col };
		m_cube[baseBlue + i] = { Color::Blue, row, col };
		m_cube[baseOrange + i] = { Color::Orange, row, col };
		m_cube[baseYellow + i] = { Color::Yellow, row, col };
	}
}

bool FunnyCube::turn(RotInfo r)
{
	//Data check/editings
	if ((r.m != X && r.m != Y && r.m != Z && r.m != M && r.m != E && r.m != S) && r.layers == 0) return true;
	if (r.layers < 0) r.layers = m_size + r.layers + 1; // Negative indexing
	if (r.layers < 0 || r.layers > m_size) return false; //not turning that :(

														 //which faces to rotate
	bool rotateFace = r.wide || r.layers == 1;
	bool rotateOpposite = r.layers == m_size;

	r.cwturn = (r.cwturn % 4 + 4) % 4; // Make cw turns positive
	if (r.cwturn == 0) return true;

	switch (r.m)
	{
		//MESXYZ
	case M:
		//M = L' R x'
		turn({ L, 1, -r.cwturn, true });
		turn({ R, 1,  r.cwturn, true });
		turn({ X, 1, -r.cwturn, true });
		break;
	case E:
		//E = D' U y'
		turn({ D, 1, -r.cwturn, true });
		turn({ U, 1,  r.cwturn, true });
		turn({ Y, 1, -r.cwturn, true });
		break;
	case S:
		//S = F' B z
		turn({ F, 1, -r.cwturn, true });
		turn({ B, 1,  r.cwturn, true });
		turn({ Z, 1,  r.cwturn, true });
		break;

		//x, y, z
	case X:
		turn({ R, m_size, r.cwturn, true });
		break;
	case Y:
		turn({ U, m_size, r.cwturn, true });
		break;
	case Z:
		turn({ F, m_size, r.cwturn, true });
		break;


		//FACES
	case F:
		if (rotateFace) faceRotate(Front, r.cwturn);
		if (rotateOpposite) faceRotate(Back, -r.cwturn);

		for (int useLayer = r.wide ? 1 : r.layers; useLayer <= r.layers; ++useLayer)
		{
			rotateSides(Up, Right, Down, Left,
				true, false, true, false,
				m_size - useLayer, useLayer - 1, useLayer - 1, m_size - useLayer,
				true, true, false, false,
				r.cwturn);
		}
		break;
	case R:
		if (rotateFace) faceRotate(Right, r.cwturn);
		if (rotateOpposite) faceRotate(Left, -r.cwturn);

		for (int useLayer = r.wide ? 1 : r.layers; useLayer <= r.layers; ++useLayer)
		{
			rotateSides(Up, Back, Down, Front,
				false, false, false, false,
				m_size - useLayer, useLayer - 1, m_size - useLayer, m_size - useLayer,
				false, true, false, false,
				r.cwturn);
		}
		break;
	case U:
		if (rotateFace) faceRotate(Up, r.cwturn);
		if (rotateOpposite) faceRotate(Down, -r.cwturn);

		for (int useLayer = r.wide ? 1 : r.layers; useLayer <= r.layers; ++useLayer)
		{
			rotateSides(Back, Right, Front, Left,
				true, true, true, true,
				useLayer - 1, useLayer - 1, useLayer - 1, useLayer - 1,
				false, false, false, false,
				r.cwturn);
		}
		break;
	case L:
		if (rotateFace) faceRotate(Left, r.cwturn);
		if (rotateOpposite) faceRotate(Right, -r.cwturn);

		for (int useLayer = r.wide ? 1 : r.layers; useLayer <= r.layers; ++useLayer)
		{
			rotateSides(Up, Front, Down, Back,
				false, false, false, false,
				useLayer - 1, useLayer - 1, useLayer - 1, m_size - useLayer,
				true, true, true, false,
				r.cwturn);
		}
		break;
	case B:
		if (rotateFace) faceRotate(Back, r.cwturn);
		if (rotateOpposite) faceRotate(Front, -r.cwturn);

		for (int useLayer = r.wide ? 1 : r.layers; useLayer <= r.layers; ++useLayer)
		{
			rotateSides(Up, Left, Down, Right,
				true, false, true, false,
				useLayer - 1, useLayer - 1, m_size - useLayer, m_size - useLayer,
				false, true, true, false,
				r.cwturn);
		}
		break;
	case D:
		if (rotateFace) faceRotate(Down, r.cwturn);
		if (rotateOpposite) faceRotate(Up, -r.cwturn);

		for (int useLayer = r.wide ? 1 : r.layers; useLayer <= r.layers; ++useLayer)
		{
			rotateSides(Back, Left, Front, Right,
				true, true, true, true,
				m_size - useLayer, m_size - useLayer, m_size - useLayer, m_size - useLayer,
				true, true, true, true,
				r.cwturn);
		}
		break;

	default:
		return false;
	}

	return true;
}

void FunnyCube::turn(std::vector<RotInfo> rots)
{
	for (unsigned i = 0; i < rots.size(); ++i)
	{
		turn(rots[i]);
	}
}

std::wstring FunnyCube::asString() const
{
	std::wstring out(L" ");

	//UP
	for (int row = 0; row < m_size; ++row)
	{
		//Leading spaces
		for (int i = 0; i < m_size; ++i) out += L"  ";

		//colors
		for (int col = 0; col < m_size; ++col)
		{
			out += L" ";
			out += colToStr(get(Side::Up, row, col));
		}

		out += L"\n ";
	}

	out += L"\n";

	//Left, Front, Right, Back
	for (int row = 0; row < m_size; ++row)
	{
		//left colors
		for (int col = 0; col < m_size; ++col)
		{
			out += L" ";
			out += colToStr(get(Side::Left, row, col));
		}
		out += L" ";

		//front colors
		for (int col = 0; col < m_size; ++col)
		{
			out += L" ";
			out += colToStr(get(Side::Front, row, col));
		}
		out += L" ";

		//right colors
		for (int col = 0; col < m_size; ++col)
		{
			out += L" ";
			out += colToStr(get(Side::Right, row, col));
		}
		out += L" ";

		//back  colors
		for (int col = 0; col < m_size; ++col)
		{
			out += L" ";
			out += colToStr(get(Side::Back, row, col));
		}

		out += L"\n";
	}

	out += L"\n ";

	//Down
	for (int row = 0; row < m_size; ++row)
	{
		//Leading spaces
		for (int i = 0; i < m_size; ++i) out += L"  ";

		//colors
		for (int col = 0; col < m_size; ++col)
		{
			out += L" ";
			out += colToStr(get(Side::Down, row, col));
		}

		out += L"\n ";
	}

	return out;
}

void FunnyCube::prettyPrint() const
{

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	std::cout << " ";

	//UP
	for (int row = 0; row < m_size; ++row)
	{
		//Leading spaces
		for (int i = 0; i < m_size + 1; ++i) std::cout << "  ";

		//colors
		for (int col = 0; col < m_size; ++col)
		{
			prettyPrintColor(get(Side::Up, row, col));
		}

		std::cout << std::endl << " ";
	}

	std::cout << std::endl << " ";

	//Left, Front, Right, Back
	for (int row = 0; row < m_size; ++row)
	{
		//left colors
		for (int col = 0; col < m_size; ++col)
		{
			prettyPrintColor(get(Side::Left, row, col));
		}
		std::cout << "  ";

		//front colors
		for (int col = 0; col < m_size; ++col)
		{
			prettyPrintColor(get(Side::Front, row, col));
		}
		std::cout << "  ";

		//right colors
		for (int col = 0; col < m_size; ++col)
		{
			prettyPrintColor(get(Side::Right, row, col));
		}
		std::cout << "  ";

		//back  colors
		for (int col = 0; col < m_size; ++col)
		{
			prettyPrintColor(get(Side::Back, row, col));
		}

		std::cout << std::endl << " ";
	}

	std::cout << std::endl;

	//Down
	for (int row = 0; row < m_size; ++row)
	{
		std::cout << " ";

		//Leading spaces
		for (int i = 0; i < m_size + 1; ++i) std::cout << "  ";

		//colors
		for (int col = 0; col < m_size; ++col)
		{
			prettyPrintColor(get(Side::Down, row, col));
		}

		std::cout << std::endl;
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

std::wstring FunnyCube::colToStr(Color c) 
{
	switch (c)
	{
	case Color::White:
		return L"W";
	case Color::Green:
		return L"G";
	case Color::Red:
		return L"R";
	case Color::Blue:
		return L"B";
	case Color::Orange:
		return L"O";
	case Color::Yellow:
		return L"Y";
	}

	return L"";
}

COLORREF FunnyCube::colToColorRef(Color c)
{
	switch (c)
	{
	case Color::White:
		return RGB(255, 255, 255);
	case Color::Green:
		return RGB(50, 255, 50);
	case Color::Red:
		return RGB(255, 0, 0);
	case Color::Blue:
		return RGB(10, 10, 255); //   
	case Color::Orange:
		return RGB(255, 121, 0);
	case Color::Yellow:
		return RGB(255, 255, 0);
	}

	return RGB(0,0,0);
}

void FunnyCube::prettyPrintColor(Color c)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (c)
	{
	case Color::White:
		//SetConsoleTextAttribute(hConsole, COMMON_LVB_UNDERSCORE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case Color::Green:
		//SetConsoleTextAttribute(hConsole, COMMON_LVB_UNDERSCORE | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case Color::Red:
		//SetConsoleTextAttribute(hConsole, COMMON_LVB_UNDERSCORE | BACKGROUND_RED | BACKGROUND_INTENSITY);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		break;
	case Color::Blue:
		//SetConsoleTextAttribute(hConsole, COMMON_LVB_UNDERSCORE | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		break;
	case Color::Orange:
		//SetConsoleTextAttribute(hConsole, COMMON_LVB_UNDERSCORE | BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_INTENSITY);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case Color::Yellow:
		//SetConsoleTextAttribute(hConsole, COMMON_LVB_UNDERSCORE | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_INTENSITY);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	}


	std::cout << "¡";



	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

FunnyCube::RotInfo FunnyCube::strToRot(std::wstring s)
{
	trim(s);
	if (s[0] == L'(') s = s.substr(1);
	if (s[s.length()-1] == L')') s = s.substr(0, s.length()-1);

	RotInfo ret{ BadMove, 0, 0, false };
	std::wregex rgx(L"^(\\d*)([a-zA-Z])(w?)(\\d*)('?)$");


	std::wsmatch matches;
	if (std::regex_search(s, matches, rgx)) {
		//std::cout << "Match found\n";

		std::wstring layers(matches[1].str());
		if (layers == L"") ret.layers = 1;
		else ret.layers = std::stoi(layers);


		std::wstring move(matches[2].str());
		switch (move[0])
		{
		case L'F':
			ret.m = F;
			break;
		case L'R':
			ret.m = R;
			break;
		case L'U':
			ret.m = U;
			break;
		case L'L':
			ret.m = L;
			break;
		case L'B':
			ret.m = B;
			break;
		case L'D':
			ret.m = D;
			break;
			//wides
		case L'f':
			ret.m = F;
			ret.wide = true;
			break;
		case L'r':
			ret.m = R;
			ret.wide = true;
			break;
		case L'u':
			ret.m = U;
			ret.wide = true;
			break;
		case L'l':
			ret.m = L;
			ret.wide = true;
			break;
		case L'b':
			ret.m = B;
			ret.wide = true;
			break;
		case L'd':
			ret.m = D;
			ret.wide = true;
			break;

			//mid turns
		case L'M':
		case L'm':
			ret.m = M;
			break;
		case L'E':
		case L'e':
			ret.m = E;
			break;
		case L'S':
		case L's':
			ret.m = S;
			break;

			//full rotations
		case L'X':
		case L'x':
			ret.m = X;
			break;
		case L'Y':
		case L'y':
			ret.m = Y;
			break;
		case L'Z':
		case L'z':
			ret.m = Z;
			break;

		}


		bool wide = matches[3].str() == L"w";
		if (wide || ret.wide)
		{
			ret.wide = true;
			if (layers == L"" && ret.layers == 1) ret.layers = 2;
		}

		std::wstring cw(matches[4].str());
		if (cw == L"") ret.cwturn = 1;
		else ret.cwturn = std::stoi(cw);

		bool ccw = matches[5].str() == L"'";
		if (ccw) ret.cwturn = -ret.cwturn;
	}
	else {
		//std::cout << "Match not found\n";
	}
	return ret;
}

std::wstring FunnyCube::rotToStr(RotInfo r)
{
	std::wstring ret(L"Move: ");

	switch (r.m)
	{
	case BadMove:
		return L"Bad Move :(";
	case F:
		ret += L"F";
		break;
	case R:
		ret += L"R";
		break;
	case U:
		ret += L"U";
		break;
	case L:
		ret += L"L";
		break;
	case B:
		ret += L"B";
		break;
	case D:
		ret += L"D";
		break;
	case M:
		ret += L"M";
		break;
	case E:
		ret += L"E";
		break;
	case S:
		ret += L"S";
		break;
	case X:
		ret += L"x";
		break;
	case Y:
		ret += L"y";
		break;
	case Z:
		ret += L"z";
		break;

	}

	ret += L", Layers = ";
	ret += std::to_wstring(r.layers);

	ret += L", Turns = ";
	ret += std::to_wstring(r.cwturn);

	ret += L", Wide: ";
	ret += r.wide ? L"True" : L"False";

	return ret;
}

std::wstring FunnyCube::rotToNotation(RotInfo r) const
{
	std::wstring ret(L"");

	if (r.cwturn == 0) return L"-";
	if (r.layers >= 3 || (!r.wide && r.layers == 2)) ret = std::to_wstring(r.layers);

	switch (r.m)
	{
	case BadMove:
		return L"-";
	case F:
		ret += L"F";
		break;
	case R:
		ret += L"R";
		break;
	case U:
		ret += L"U";
		break;
	case L:
		ret += L"L";
		break;
	case B:
		ret += L"B";
		break;
	case D:
		ret += L"D";
		break;
	case M:
		ret = L"M";
		break;
	case E:
		ret = L"E";
		break;
	case S:
		ret = L"S";
		break;
	case X:
		ret = L"x";
		break;
	case Y:
		ret = L"y";
		break;
	case Z:
		ret = L"z";
		break;
	}

	//other things to add
	switch (r.m)
	{
	case F:
	case R:
	case U:
	case L:
	case B:
	case D:
		//lower or not or Fw type beat
		if (r.wide && r.layers == 2 && m_size <= 3)
		{
			//use lower case
			ret[ret.length() - 1] = towlower(ret[ret.length()-1]);	
		}
		else if (r.wide && (r.layers >= 3 || (r.layers == 2 && m_size > 3)))
		{
			ret += L"w";
		}
		break;

	case M:
	case E:
	case S:
	case X:
	case Y:
	case Z:

		break;
	}

	if (std::abs(r.cwturn) != 1) ret += std::to_wstring(std::abs(r.cwturn));
	if (r.cwturn < 0) ret += L"'";

	return ret;
}

std::vector<FunnyCube::RotInfo> FunnyCube::strToRots(std::wstring s)
{
	std::vector<FunnyCube::RotInfo> rots;

	std::vector<std::wstring> cmds;// = split(s, L' ');

	std::wstringstream ss(s);
	std::wstring item;
	while (std::getline(ss, item, L' ')) {
		if (!item.empty()) {
			cmds.push_back(item);
		}
	}

	for (unsigned i = 0; i < cmds.size(); ++i)
	{
		rots.push_back(strToRot(cmds[i]));
	}

	return rots;
}

std::wstring FunnyCube::rotsToNotation(std::vector<RotInfo> rots) const
{
	std::wstring ret;

	for (unsigned i = 0; i < rots.size(); ++i)
	{
		ret += rotToNotation(rots[i]);
		ret += L" ";
	}

	return ret;
}

std::vector<FunnyCube::RotInfo> FunnyCube::reverseRots(std::vector<RotInfo> rots)
{
	std::vector<RotInfo> ret;
	for (unsigned i = 0; i < rots.size(); ++i)
	{
		RotInfo r = rots[rots.size() - i - 1];
		if (r.cwturn != 2) r.cwturn = -r.cwturn;
		ret.push_back(r);
	}
	return ret;
}

void FunnyCube::faceCW(Side s)
{
	/*
	5x5 row = 3, col = 2
	4x4 row = 2, col = 2

	***kk
	***kk
	xxokk
	xxqqq
	xxqqq

	rotates like this kinda
	*/

	for (int row = 0; row < (m_size + 1) / 2; ++row)
	{
		for (int col = 0; col < m_size / 2; ++col)
		{

			Facelet temp = getFacelet(s, row, col);
			setFacelet(s, row, col, getFacelet(s, m_size - col - 1, row));
			setFacelet(s, m_size - col - 1, row, getFacelet(s, m_size - row - 1, m_size - col - 1));
			setFacelet(s, m_size - row - 1, m_size - col - 1, getFacelet(s, col, m_size - row - 1));
			setFacelet(s, col, m_size - row - 1, temp);


		}
	}
}

void FunnyCube::faceCCW(Side s)
{
	for (int row = 0; row < (m_size + 1) / 2; ++row)
	{
		for (int col = 0; col < m_size / 2; ++col)
		{
			Facelet temp = getFacelet(s, row, col);
			setFacelet(s, row, col, getFacelet(s, col, m_size - row - 1));
			setFacelet(s, col, m_size - row - 1, getFacelet(s, m_size - row - 1, m_size - col - 1));
			setFacelet(s, m_size - row - 1, m_size - col - 1, getFacelet(s, m_size - col - 1, row));
			setFacelet(s, m_size - col - 1, row, temp);
		}
	}
}

void FunnyCube::faceRotate(Side s, int cwturn)
{
	cwturn = (cwturn % 4 + 4) % 4;
	switch (cwturn)
	{
	case 2:
		faceCW(s);
	case 1:
		faceCW(s);
		break;
	case 3:
		faceCCW(s);
		break;
	}
}

void FunnyCube::rotateSides(Side s0, Side s1, Side s2, Side s3,
	bool useRow0, bool useRow1, bool useRow2, bool useRow3,
	int usingRowCol0, int usingRowCol1, int usingRowCol2, int usingRowCol3,
	bool startZero0, bool startZero1, bool startZero2, bool startZero3,
	int cwturn)
{
	Side s[] = { s0, s1, s2, s3 };
	bool useRow[] = { useRow0, useRow1, useRow2, useRow3 };
	int usingRowCol[] = { usingRowCol0, usingRowCol1, usingRowCol2, usingRowCol3 };
	bool startZero[] = { startZero0, startZero1, startZero2, startZero3 };

	for (int other = 0; other < m_size; ++other)
	{
		/*
		Color temp;
		if (useRow[0]) temp = get(s[0], usingRowCol[0], startZero[0]?(other):(m_size-other-1));
		else temp = get(s[0], startZero[0] ? (other) : (m_size - other - 1), usingRowCol[0]);

		//prev (-cwturn)
		int lastInd = 0, curInd;

		for (int i = 0; i < 3; ++i)
		{
		curInd = (lastInd - cwturn) % 4;

		if (useRow[lastInd]) {
		if (useRow[curInd])
		set(s[lastInd], usingRowCol[lastInd], startZero[lastInd] ? (other) : (m_size - other - 1),
		get(s[curInd], usingRowCol[curInd], startZero[curInd] ? (other) : (m_size - other - 1)));
		else
		set(s[lastInd], usingRowCol[lastInd], startZero[lastInd] ? (other) : (m_size - other - 1),
		get(s[curInd], startZero[curInd] ? (other) : (m_size - other - 1), usingRowCol[curInd]));
		}
		else {
		if (useRow[curInd])
		set(s[lastInd], startZero[lastInd] ? (other) : (m_size - other - 1), usingRowCol[lastInd],
		get(s[curInd], usingRowCol[curInd], startZero[curInd] ? (other) : (m_size - other - 1)));
		else
		set(s[lastInd], startZero[lastInd] ? (other) : (m_size - other - 1), usingRowCol[lastInd],
		get(s[curInd], startZero[curInd] ? (other) : (m_size - other - 1), usingRowCol[curInd]));
		}

		lastInd = curInd;
		}
		*/

		Facelet toUse[4];

		//Get the four colors to use
		for (int i = 0; i < 4; ++i)
		{
			int next = (i - cwturn + 4) % 4;

			if (useRow[next]) toUse[i] = getFacelet(s[next], usingRowCol[next], startZero[next] ? (other) : (m_size - other - 1));
			else toUse[i] = getFacelet(s[next], startZero[next] ? (other) : (m_size - other - 1), usingRowCol[next]);
		}

		//Set the four colors using
		for (int i = 0; i < 4; ++i)
		{
			if (useRow[i]) setFacelet(s[i], usingRowCol[i], startZero[i] ? (other) : (m_size - other - 1), toUse[i]);
			else setFacelet(s[i], startZero[i] ? (other) : (m_size - other - 1), usingRowCol[i], toUse[i]);
		}

	}
}