#include "stdafx.h"
#include "FunnyCube.h"

#include <windows.h> 
#include <dos.h>
#include <iostream>

#include <vector>
#include <string>
#include <regex>

#include <algorithm> 
#include <cctype>
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

inline void ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
inline void rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
inline void trim(std::string &s) {
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

		m_cube[baseWhite + i] = { ColorAsColorRef[White], row, col };
		m_cube[baseGreen + i] = { ColorAsColorRef[Green], row, col };
		m_cube[baseRed + i] = { ColorAsColorRef[Red], row, col };
		m_cube[baseBlue + i] = { ColorAsColorRef[Blue], row, col };
		m_cube[baseOrange + i] = { ColorAsColorRef[Orange], row, col };
		m_cube[baseYellow + i] = { ColorAsColorRef[Yellow], row, col };
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

void FunnyCube::colorIn(std::wstring s)
{
	for (int i = 0; i < s.length() && i < m_cube.size(); ++i)
	{
		m_cube[i].c = charToColor(s[i]);
	}
}

std::wstring FunnyCube::asWString() const
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
			out += colorToWStr(getColor(Side::Up, row, col));
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
			out += colorToWStr(getColor(Side::Left, row, col));
		}
		out += L" ";

		//front colors
		for (int col = 0; col < m_size; ++col)
		{
			out += L" ";
			out += colorToWStr(getColor(Side::Front, row, col));
		}
		out += L" ";

		//right colors
		for (int col = 0; col < m_size; ++col)
		{
			out += L" ";
			out += colorToWStr(getColor(Side::Right, row, col));
		}
		out += L" ";

		//back  colors
		for (int col = 0; col < m_size; ++col)
		{
			out += L" ";
			out += colorToWStr(getColor(Side::Back, row, col));
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
			out += colorToWStr(getColor(Side::Down, row, col));
		}

		out += L"\n ";
	}

	return out;
}

std::string FunnyCube::asString() const
{
	std::string out(" ");

	//UP
	for (int row = 0; row < m_size; ++row)
	{
		//Leading spaces
		for (int i = 0; i < m_size; ++i) out += "  ";

		//colors
		for (int col = 0; col < m_size; ++col)
		{
			out += " ";
			out += colorToStr(getColor(Side::Up, row, col));
		}

		out += "\n ";
	}

	out += "\n";

	//Left, Front, Right, Back
	for (int row = 0; row < m_size; ++row)
	{
		//left colors
		for (int col = 0; col < m_size; ++col)
		{
			out += " ";
			out += colorToStr(getColor(Side::Left, row, col));
		}
		out += " ";

		//front colors
		for (int col = 0; col < m_size; ++col)
		{
			out += " ";
			out += colorToStr(getColor(Side::Front, row, col));
		}
		out += " ";

		//right colors
		for (int col = 0; col < m_size; ++col)
		{
			out += " ";
			out += colorToStr(getColor(Side::Right, row, col));
		}
		out += " ";

		//back  colors
		for (int col = 0; col < m_size; ++col)
		{
			out += " ";
			out += colorToStr(getColor(Side::Back, row, col));
		}

		out += "\n";
	}

	out += "\n ";

	//Down
	for (int row = 0; row < m_size; ++row)
	{
		//Leading spaces
		for (int i = 0; i < m_size; ++i) out += "  ";

		//colors
		for (int col = 0; col < m_size; ++col)
		{
			out += " ";
			out += colorToStr(getColor(Side::Down, row, col));
		}

		out += "\n ";
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
			prettyPrintColor(getColor(Side::Up, row, col));
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
			prettyPrintColor(getColor(Side::Left, row, col));
		}
		std::cout << "  ";

		//front colors
		for (int col = 0; col < m_size; ++col)
		{
			prettyPrintColor(getColor(Side::Front, row, col));
		}
		std::cout << "  ";

		//right colors
		for (int col = 0; col < m_size; ++col)
		{
			prettyPrintColor(getColor(Side::Right, row, col));
		}
		std::cout << "  ";

		//back  colors
		for (int col = 0; col < m_size; ++col)
		{
			prettyPrintColor(getColor(Side::Back, row, col));
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
			prettyPrintColor(getColor(Side::Down, row, col));
		}

		std::cout << std::endl;
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

std::wstring FunnyCube::colorToWStr(COLORREF c)
{
	switch (c)
	{
	case ColorAsColorRef[White]:
		return ColorAsWStr[White];
	case ColorAsColorRef[Green]:
		return ColorAsWStr[Green];
	case ColorAsColorRef[Red]:
		return ColorAsWStr[Red];
	case ColorAsColorRef[Blue]:
		return ColorAsWStr[Blue];
	case ColorAsColorRef[Orange]:
		return ColorAsWStr[Orange];
	case ColorAsColorRef[Yellow]:
		return ColorAsWStr[Yellow];
	}

	return L"?";
}

std::string FunnyCube::colorToStr(COLORREF c)
{
	switch (c)
	{
	case ColorAsColorRef[White]:
		return ColorAsStr[White];
	case ColorAsColorRef[Green]:
		return ColorAsStr[Green];
	case ColorAsColorRef[Red]:
		return ColorAsStr[Red];
	case ColorAsColorRef[Blue]:
		return ColorAsStr[Blue];
	case ColorAsColorRef[Orange]:
		return ColorAsStr[Orange];
	case ColorAsColorRef[Yellow]:
		return ColorAsStr[Yellow];
	}

	return "?";
}

COLORREF FunnyCube::charToColor(wchar_t s)
{
	switch (s)
	{
	case L'W':
	case L'w':
		return ColorAsColorRef[White];
	case L'R':
	case L'r':
		return ColorAsColorRef[Red];
	case L'O':
	case L'o':
		return ColorAsColorRef[Orange];
	case L'B':
	case L'b':
		return ColorAsColorRef[Blue];
	case L'G':
	case L'g':
		return ColorAsColorRef[Green];
	case L'Y':
	case L'y':
		return ColorAsColorRef[Yellow];
	}

	return COLORREF();
}

COLORREF FunnyCube::charToColor(char s)
{
	switch (s)
	{
	case 'W':
	case 'w':
		return ColorAsColorRef[White];
	case 'R':
	case 'r':
		return ColorAsColorRef[Red];
	case 'O':
	case 'o':
		return ColorAsColorRef[Orange];
	case 'B':
	case 'b':
		return ColorAsColorRef[Blue];
	case 'G':
	case 'g':
		return ColorAsColorRef[Green];
	case 'Y':
	case 'y':
		return ColorAsColorRef[Yellow];
	}

	return COLORREF();
}

void FunnyCube::prettyPrintColor(COLORREF c)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (c)
	{
	case ColorAsColorRef[White]:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
		break;
	case ColorAsColorRef[Green]:
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		break;
	case ColorAsColorRef[Red]:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		break;
	case ColorAsColorRef[Blue]:
		SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE);
		break;
	case ColorAsColorRef[Orange]:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
		break;
	case ColorAsColorRef[Yellow]:
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
		break;
	}

	std::cout << "��";

	SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

FunnyCube::RotInfo FunnyCube::strToRot(std::wstring s)
{
	trim(s);
	if (s[0] == L'(') s = s.substr(1);
	if (s[s.length() - 1] == L')') s = s.substr(0, s.length() - 1);

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

FunnyCube::RotInfo FunnyCube::strToRot(std::string s)
{
	trim(s);
	if (s[0] == '(') s = s.substr(1);
	if (s[s.length() - 1] == ')') s = s.substr(0, s.length() - 1);

	RotInfo ret{ BadMove, 0, 0, false };
	std::regex rgx("^(\\d*)([a-zA-Z])(w?)(\\d*)('?)$");


	std::smatch matches;
	if (std::regex_search(s, matches, rgx)) {
		//std::cout << "Match found\n";

		std::string layers(matches[1].str());
		if (layers == "") ret.layers = 1;
		else ret.layers = std::stoi(layers);


		std::string move(matches[2].str());
		switch (move[0])
		{
		case 'F':
			ret.m = F;
			break;
		case 'R':
			ret.m = R;
			break;
		case 'U':
			ret.m = U;
			break;
		case 'L':
			ret.m = L;
			break;
		case 'B':
			ret.m = B;
			break;
		case 'D':
			ret.m = D;
			break;
			//wides
		case 'f':
			ret.m = F;
			ret.wide = true;
			break;
		case 'r':
			ret.m = R;
			ret.wide = true;
			break;
		case 'u':
			ret.m = U;
			ret.wide = true;
			break;
		case 'l':
			ret.m = L;
			ret.wide = true;
			break;
		case 'b':
			ret.m = B;
			ret.wide = true;
			break;
		case 'd':
			ret.m = D;
			ret.wide = true;
			break;

			//mid turns
		case 'M':
		case 'm':
			ret.m = M;
			break;
		case 'E':
		case 'e':
			ret.m = E;
			break;
		case 'S':
		case 's':
			ret.m = S;
			break;

			//full rotations
		case 'X':
		case 'x':
			ret.m = X;
			break;
		case 'Y':
		case 'y':
			ret.m = Y;
			break;
		case 'Z':
		case 'z':
			ret.m = Z;
			break;

		}


		bool wide = matches[3].str() == "w";
		if (wide || ret.wide)
		{
			ret.wide = true;
			if (layers == "" && ret.layers == 1) ret.layers = 2;
		}

		std::string cw(matches[4].str());
		if (cw == "") ret.cwturn = 1;
		else ret.cwturn = std::stoi(cw);

		bool ccw = matches[5].str() == "'";
		if (ccw) ret.cwturn = -ret.cwturn;
	}
	else {
		//std::cout << "Match not found\n";
	}
	return ret;
}


std::wstring FunnyCube::rotToWStr(RotInfo r)
{
	if (r.m == BadMove) return L"Bad Move :(";

	std::wstring ret(L"Move: ");

	ret += MoveAsWStr[r.m];

	ret += L", Layers = ";
	ret += std::to_wstring(r.layers);

	ret += L", Turns = ";
	ret += std::to_wstring(r.cwturn);

	ret += L", Wide: ";
	ret += r.wide ? L"True" : L"False";

	return ret;
}

std::string FunnyCube::rotToStr(RotInfo r)
{
	if (r.m == BadMove) return "Bad Move :(";

	std::string ret("Move: ");

	ret += MoveAsStr[r.m];

	ret += ", Layers = ";
	ret += std::to_string(r.layers);

	ret += ", Turns = ";
	ret += std::to_string(r.cwturn);

	ret += ", Wide: ";
	ret += r.wide ? "True" : "False";

	return ret;
}


std::wstring FunnyCube::rotToNotationW(RotInfo r) const
{
	if (r.m == BadMove) return L"-";

	std::wstring ret(L"");

	if (r.cwturn == 0) return L"-";
	if (r.layers >= 3 || (!r.wide && r.layers == 2)) ret = std::to_wstring(r.layers);

	ret += MoveAsWStr[r.m];

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
			ret[ret.length() - 1] = towlower(ret[ret.length() - 1]);
		}
		else if (r.wide && (r.layers >= 3 || (r.layers == 2 && m_size > 3)))
		{
			ret += L"w";
		}
		break;
	}

	if (std::abs(r.cwturn) != 1) ret += std::to_wstring(std::abs(r.cwturn));
	if (r.cwturn < 0) ret += L"'";

	return ret;
}

std::string FunnyCube::rotToNotation(RotInfo r) const
{
	if (r.m == BadMove) return "-";

	std::string ret("");

	if (r.cwturn == 0) return "-";
	if (r.layers >= 3 || (!r.wide && r.layers == 2)) ret = std::to_string(r.layers);

	ret += MoveAsStr[r.m];

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
			ret[ret.length() - 1] = tolower(ret[ret.length() - 1]);
		}
		else if (r.wide && (r.layers >= 3 || (r.layers == 2 && m_size > 3)))
		{
			ret += "w";
		}
		break;
	}

	if (std::abs(r.cwturn) != 1) ret += std::to_string(std::abs(r.cwturn));
	if (r.cwturn < 0) ret += "'";

	return ret;
}

std::vector<FunnyCube::RotInfo> FunnyCube::strToRots(std::wstring s, const AlgSaver *a)
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
		RotInfo r = strToRot(cmds[i]);
		if(r.m != BadMove)
			rots.push_back(r);
		else if (a != nullptr)
		{
			AlgSaver::Alg alg = a->get(cmds[i]);
			if (alg.name != L"")
			{
				std::vector<FunnyCube::RotInfo> rots2 = strToRots(alg.alg, a);
				rots.insert(rots.end(), rots2.begin(), rots2.end());
			}
		}
	}

	return rots;
}

std::vector<FunnyCube::RotInfo> FunnyCube::strToRots(std::string s)
{
	std::vector<FunnyCube::RotInfo> rots;

	std::vector<std::string> cmds;// = split(s, L' ');

	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, ' ')) {
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

std::wstring FunnyCube::rotsToNotationW(std::vector<RotInfo> rots) const
{
	std::wstring ret;

	for (unsigned i = 0; i < rots.size(); ++i)
	{
		ret += rotToNotationW(rots[i]);
		ret += L" ";
	}

	return ret;
}

std::string FunnyCube::rotsToNotation(std::vector<RotInfo> rots) const
{
	std::string ret;

	for (unsigned i = 0; i < rots.size(); ++i)
	{
		ret += rotToNotation(rots[i]);
		ret += " ";
	}

	return ret;
}

std::vector<FunnyCube::RotInfo> FunnyCube::reverseRots(std::vector<RotInfo> rots)
{
	std::vector<RotInfo> ret;
	for (unsigned i = 0; i < rots.size(); ++i)
	{
		ret.push_back(rots[rots.size() - i - 1].reversed());
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

			Facelet temp = get(s, row, col);
			set(s, row, col, get(s, m_size - col - 1, row));
			set(s, m_size - col - 1, row, get(s, m_size - row - 1, m_size - col - 1));
			set(s, m_size - row - 1, m_size - col - 1, get(s, col, m_size - row - 1));
			set(s, col, m_size - row - 1, temp);


		}
	}
}

void FunnyCube::faceCCW(Side s)
{
	for (int row = 0; row < (m_size + 1) / 2; ++row)
	{
		for (int col = 0; col < m_size / 2; ++col)
		{
			Facelet temp = get(s, row, col);
			set(s, row, col, get(s, col, m_size - row - 1));
			set(s, col, m_size - row - 1, get(s, m_size - row - 1, m_size - col - 1));
			set(s, m_size - row - 1, m_size - col - 1, get(s, m_size - col - 1, row));
			set(s, m_size - col - 1, row, temp);
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
		Facelet toUse[4];

		//Get the four colors to use
		for (int i = 0; i < 4; ++i)
		{
			int next = (i - cwturn + 4) % 4;

			if (useRow[next]) toUse[i] = get(s[next], usingRowCol[next], startZero[next] ? (other) : (m_size - other - 1));
			else toUse[i] = get(s[next], startZero[next] ? (other) : (m_size - other - 1), usingRowCol[next]);
		}

		//Set the four colors using
		for (int i = 0; i < 4; ++i)
		{
			if (useRow[i]) set(s[i], usingRowCol[i], startZero[i] ? (other) : (m_size - other - 1), toUse[i]);
			else set(s[i], startZero[i] ? (other) : (m_size - other - 1), usingRowCol[i], toUse[i]);
		}

	}
}