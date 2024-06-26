
// QBertGUIDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "QBertGUI.h"
#include "QBertGUIDlg.h"
#include "afxdialogex.h"


#include <string>
#include <sstream>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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

void AaronTest()
{
	MyCubeLite a;

	a.test();

	MyCubeLite b(a);
	MyCubeLite c = a;

	MyCubeLite d;
	MyCubeLite e = d;

	d.test();

	e.test();

}


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CQBertGUIDlg ダイアログ



CQBertGUIDlg::CQBertGUIDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_QBERTGUI_DIALOG, pParent)//, m_Cube(7)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//IMPORTANT !!! FOR RICH EDIT TO WORK !!! WHYY IS THIS NOT EXPLAINED? ??? IDK !!!
	AfxInitRichEdit2();
}

void CQBertGUIDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CMD, CComboCmd);
	//DDX_Control(pDX, IDC_RICH_DISPLAY, m_RichDisplay);
	DDX_Control(pDX, IDC_RICH_DISPLAY, m_RichDisplay);
}

BEGIN_MESSAGE_MAP(CQBertGUIDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BTN_GO, &CQBertGUIDlg::OnBnClickedBtnGo)
//	ON_BN_CLICKED(IDC_BTN_UNDO, &CQBertGUIDlg::OnBnClickedYes)
	ON_BN_CLICKED(IDC_BTN_GO, &CQBertGUIDlg::OnBnClickedBtnGo)
	ON_BN_CLICKED(IDC_BTN_Keep, &CQBertGUIDlg::OnBnClickedBtnKeep)
	ON_BN_CLICKED(IDC_BTN_RESET, &CQBertGUIDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_UNDOMORE, &CQBertGUIDlg::OnBnClickedBtnUndomore)
	ON_BN_CLICKED(IDC_BTN_UNDO1, &CQBertGUIDlg::OnBnClickedBtnUndo1)
	ON_BN_CLICKED(IDC_BTN_CUSTOM, &CQBertGUIDlg::OnBnClickedBtnCustom)
	ON_BN_CLICKED(IDC_BTN_CUSTOM2, &CQBertGUIDlg::OnBnClickedBtnCustom2)
	ON_BN_CLICKED(IDC_BTN_CUSTOM3, &CQBertGUIDlg::OnBnClickedBtnCustom3)
	ON_BN_CLICKED(IDC_BTN_CUSTOM4, &CQBertGUIDlg::OnBnClickedBtnCustom4)
END_MESSAGE_MAP()


// CQBertGUIDlg メッセージ ハンドラー

BOOL CQBertGUIDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。
	AaronTest();

	//Algs
	setupAlgs();

	//rich teext background color
	m_RichDisplay.SetBackgroundColor(0, RGB(75, 75, 75));
	SetBackgroundColor(RGB(75, 75, 75));

	setCubeText();


	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CQBertGUIDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CQBertGUIDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CQBertGUIDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CQBertGUIDlg::OnBnClickedBtnGo()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	CString other;
	CComboCmd.GetWindowTextW(other);

	if (other.Left(6) == "Color ")
	{//special color in command
		m_Cube.colorIn(std::wstring(other.Right(other.GetLength() - 6)));
		m_Cube.assignPieceInfo();
		setCubeText();
	}
	else
	{
		getCmdAndRotate();
	}
}

void CQBertGUIDlg::setCubeText()
{

	std::wstring text;// = getCubeText();
	text += getExtraInfoText();

	//m_RichDisplay.SetWindowTextW(m_Cube.asString().c_str());
	m_RichDisplay.SetWindowTextW(text.c_str());

	//COLOR IN THE SIDES !!!
	//colorCubeText(0);


}

void CQBertGUIDlg::getCmdAndRotate()
{
	CString other;
	CComboCmd.GetWindowTextW(other);

	std::vector<FunnyCube::RotInfo> rots = m_Cube.strToRots(std::wstring(other), &m_Algs);

	m_Cube.turn(rots);

	if (rots.size() > 0)
	{
		//set the cmds list
		m_Cmds.push_back(rots);
	}

	setCubeText();

	//Combo stuff
	if (other != "")
	{
		int i = CComboCmd.FindStringExact(0, other);
		if (i != -1)CComboCmd.DeleteString(i);

		CComboCmd.InsertString(0, other);
	}

	if (m_keepText) CComboCmd.SetWindowTextW(other);
	else CComboCmd.SetWindowTextW(L"");

}

std::wstring CQBertGUIDlg::getCubeText()
{
	std::wstring text;

	/*
	3x3
	　　　　　■■■
	　　　　　■■■
	　　　　　■■■

	　■■■　■■■　■■■　■■■
	　■■■　■■■　■■■　■■■
	　■■■　■■■　■■■　■■■

	　　　　　■■■
	　　　　　■■■
	　　　　　■■■
	
	*/

	for (int i = 0; i < m_Cube.size(); ++i)
	{
		for (int j = 0; j < m_Cube.size() + 2; ++j) text += L"　　";
		for (int j = 0; j < m_Cube.size(); ++j) text += L" ■";
		text += L"\n";
	}
	text += L"\n";

	for (int i = 0; i < m_Cube.size(); ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			text += L"　　";
			for (int k = 0; k < m_Cube.size(); ++k) text += L" ■";
		}
		text += L"\n";
	}

	for (int i = 0; i < m_Cube.size(); ++i)
	{
		text += L"\n";
		for (int j = 0; j < m_Cube.size() + 2; ++j) text += L"　　";
		for (int j = 0; j < m_Cube.size(); ++j) text += L" ■";
	}

	return text;
}

void CQBertGUIDlg::colorCubeText(int startChar)
{
	//CRichEditCtrl FormatRange something something

	//https://learn.microsoft.com/en-us/windows/win32/api/richedit/ns-richedit-charformata
	CHARFORMAT cf = { sizeof(cf) };
	cf.dwMask = CFM_COLOR;
	//cf.crTextColor = RGB(255, 0, 0);
	//m_RichDisplay.SetSel((m_Cube.size() + 2) * 4 + m_Cube.size() * 2 + 4, (m_Cube.size() + 2) * 4 + m_Cube.size() * 2 + 5); row 2 col 2 lol
	//m_RichDisplay.SetSelectionCharFormat(cf);

	//UP
	long ind = startChar;
	for (int row = 0; row < m_Cube.size(); ++row)
	{
		ind += (m_Cube.size() + 2) * 2; //spaces at start
		for (int col = 0; col < m_Cube.size(); ++col)
		{
			ind += 1; //space before square
					  //Get color
			cf.crTextColor = m_Cube.getColor(FunnyCube::Up, row, col);
			//Set color
			m_RichDisplay.SetSel(ind, ind + 1);
			m_RichDisplay.SetSelectionCharFormat(cf);

			ind += 1; //sqare
		}
		ind += 1; //newline
	}
	ind += 1; //newline

	long indSave = ind;
	//LEFT
	ind = indSave;
	for (int row = 0; row < m_Cube.size(); ++row)
	{
		//     2 spaces per side = 2
		//	   size #squares per side = 2*m_Cube.size()
		//	   my spaces = 2
		ind += (2 + 2 * m_Cube.size()) * 0 + (2); //spaces at start
		for (int col = 0; col < m_Cube.size(); ++col)
		{
			ind += 1; //space before square
					  //Get color
			cf.crTextColor = m_Cube.getColor(FunnyCube::Left, row, col);
			//Set color
			m_RichDisplay.SetSel(ind, ind + 1);
			m_RichDisplay.SetSelectionCharFormat(cf);

			ind += 1; //square
		}
		//     2 spaces per side = 2
		//	   size #squares per side = 2*m_Cube.size()
		//	   newline = 1
		ind += (2 + 2 * m_Cube.size()) * 3 + (1);
	}

	//FRONT
	ind = indSave;
	for (int row = 0; row < m_Cube.size(); ++row)
	{
		//     2 spaces per side = 2
		//	   size #squares per side = 2*m_Cube.size()
		//	   my spaces = 2
		ind += (2 + 2 * m_Cube.size()) * 1 + (2); //spaces at start
		for (int col = 0; col < m_Cube.size(); ++col)
		{
			ind += 1; //space before square
					  //Get color
			cf.crTextColor = m_Cube.getColor(FunnyCube::Front, row, col);
			//Set color
			m_RichDisplay.SetSel(ind, ind + 1);
			m_RichDisplay.SetSelectionCharFormat(cf);

			ind += 1; //square
		}
		//     2 spaces per side = 2
		//	   size #squares per side = 2*m_Cube.size()
		//	   newline = 1
		ind += (2 + 2 * m_Cube.size()) * 2 + (1);
	}

	//RIGHT
	ind = indSave;
	for (int row = 0; row < m_Cube.size(); ++row)
	{
		//     2 spaces per side = 2
		//	   size #squares per side = 2*m_Cube.size()
		//	   my spaces = 2
		ind += (2 + 2 * m_Cube.size()) * 2 + (2); //spaces at start
		for (int col = 0; col < m_Cube.size(); ++col)
		{
			ind += 1; //space before square
					  //Get color
			cf.crTextColor = m_Cube.getColor(FunnyCube::Right, row, col);
			//Set color
			m_RichDisplay.SetSel(ind, ind + 1);
			m_RichDisplay.SetSelectionCharFormat(cf);

			ind += 1; //square
		}
		//     2 spaces per side = 2
		//	   size #squares per side = 2*m_Cube.size()
		//	   newline = 1
		ind += (2 + 2 * m_Cube.size()) * 1 + (1);
	}

	//BACK
	ind = indSave;
	for (int row = 0; row < m_Cube.size(); ++row)
	{
		//     2 spaces per side = 2
		//	   size #squares per side = 2*m_Cube.size()
		//	   my spaces = 2
		ind += (2 + 2 * m_Cube.size()) * 3 + (2); //spaces at start
		for (int col = 0; col < m_Cube.size(); ++col)
		{
			ind += 1; //space before square
					  //Get color
			cf.crTextColor = m_Cube.getColor(FunnyCube::Back, row, col);
			//Set color
			m_RichDisplay.SetSel(ind, ind + 1);
			m_RichDisplay.SetSelectionCharFormat(cf);

			ind += 1; //square
		}
		//     2 spaces per side = 2
		//	   size #squares per side = 2*m_Cube.size()
		//	   newline = 1
		ind += (2 + 2 * m_Cube.size()) * 0 + (1);
	}


	//DOWN
	for (int row = 0; row < m_Cube.size(); ++row)
	{
		ind += (m_Cube.size() + 2) * 2 + 1; //spaces at start + newline
		for (int col = 0; col < m_Cube.size(); ++col)
		{
			ind += 1; //space before square
					  //Get color
			cf.crTextColor = m_Cube.getColor(FunnyCube::Down, row, col);
			//Set color
			m_RichDisplay.SetSel(ind, ind + 1);
			m_RichDisplay.SetSelectionCharFormat(cf);

			ind += 1; //sqare
		}
	}

}

std::wstring CQBertGUIDlg::getExtraInfoText()
{
	std::wstring text;
	
	
	MyCubeLite lite = m_Cube.getLite();
	//Write Solve info
	auto p3 = lite.getPhase1Coords();
	auto p4 = lite.getPhase2Coords();
	text += L"\n\nPHASE 1 (";
	text += std::to_wstring(p3.x1) + L", " + std::to_wstring(p3.x2) + L", " + std::to_wstring(p3.x3) + L")";
	text += L" -> " + std::to_wstring(lite.stepsFromG1()) + L"\n";
	text += L"G1? " + std::to_wstring(lite.isG1()) + L"\n";

	text += L"\nPHASE 2 (";
	text += std::to_wstring(p4.x1) + L", " + std::to_wstring(p4.x2) + L", " + std::to_wstring(p4.x3) + L")";
	if (lite.isG1()) text += L" -> " + std::to_wstring(lite.stepsFromSolved()) + L"\n";
	else text += L"\n";
	text += L"Solved? " + std::to_wstring(lite.solved()) += L"\n";


	return text;

}

void CQBertGUIDlg::setupAlgs()
{
	//m_Algs.addAlg(L"T", L"R U R' U' R' F R2 U' R' U' R U R' F'", L"PLL");
	//m_Algs.addAlg(L"Ep", L"x' R U' R' D R U R' D' R U R' D R U' R' D' x", L"PLL");

	m_Algs.addAlgs(
L"\n\
*Beginner, Full\n\
Ua, R U R' U R' U' R2 U' R' U R' U R\n\
Ub, R' U R3 U' R' U' R' U R U R2\n\
H,  (M2' U' M2') U2 (M2' U' M2'), Symmetric\n\
Z,M' U' M2' U' M2' U' M' U2 M2, Half_Symmetric\n\
T, R U R' U' R' F R2 U' R' U' R U R' F'\n\
Y,  F (R U' R' U') (R U R' F') (R U R' U') (R' F R F') \n\
*Full\n\
Aa, x R' U R' D2 R U' R' D2 R2 x'\n\
Ab, x R2 D2 R U R' D2 R U' R x'\n\
E, x' R U' R' D R U R' D' R U R' D R U' R' D' x, Half_Symmetric\n\
F,  (R' U' F') (R U R' U') (R' F R2 U') (R' U' R U) (R' U R) \n\
Jb,  (R U R' F') (R U R' U') R' F R2 U' R'\n\
Ja,  (L' U' L F) (L' U' L U) L F' L2' U L \n\
Ra,  (R U' R' U') (R U R) D (R' U' R) D' (R' U2 R')\n\
Rb,  (R' U2 R U2') R' F (R U R' U') R' F' R2 \n\
V,  R' U R U' R' f' U' R U2 R' U' R U' R' f R\n\
Na,  (R U R' U) (R U R' F') (R U R' U') (R' F R2 U') R' U2 (R U' R'), Symmetric\n\
Nb,  (r D r' U2) (r D r' U2) (r D r' U2) (r D r' U2) (r D r'), Symmetric\n\
Ga,  R2 U (R' U R' U') (R U' R2) D U' (R' U R D')\n\
Gb,  R U' R' U2 R' U' F R f' R' U R S R U' R'\n\
Gc,  R2' F2 (R U2 R U2') R' F (R U R' U') R' F R2\n\
Gd,  (R U R' U') D (R2 U' R U') (R' U R' U) R2 D'\n\
",
		L"PLL");

	m_Algs.addAlgs(
L"\n\
*Full, Cross\n\
Sune, R U R' U R U2' R'\n\
Anti_Sune, R U2 R' U' R U' R'\n\
Cross,R U R' U R U' R' U R U2 R', Half_Symmetric\n\
Bruno,R U2' R2' U' R2 U' R2' U2' R\n\
Chameleon,(r U R' U') (r' F R F')\n\
Headlights, R2 D R' U2 R D' R' U2 R', Practice\n\
Bowtie, F' (r U R' U') r' F R\n\
\n\
*Full, Dot\n\
Blank,(R U2') (R2' F R F') U2' (R' F R F'), Half_Symmetric\n\
Zamboni,f (U R U' R') f' F (U R U' R') F' \n\
Slash,(R U R' U) (R' F R F') U2' (R' F R F')\n\
X, M' U' R' U' R U M2 U' R' U r, Symmetric, Practice\n\
Bunny,M U (R U R' U') M' (R' F R F')\n\
Crown,(r U R' U R U2 r') (r' U' R U' R' U2 r)\n\
Anti_Mini_Slash,F U R U' R F' U' F R U R' U' F'\n\
Mini_Slash,F U R U' R F' U F R U R' U' F'\n\
\n\
*Full, T_Shape\n\
T, F R U R' U' F'\n\
Key, R U R' U' R' F R F'\n\
\n\
*Full, Square\n\
Righty_Square,  r U2 R' U' R U' r'\n\
Lefty_Square, r' U2' R U R' U r\n\
\n\
*Full, Corners\n\
Arrow, (r U R' U') M (U R U' R')\n\
H, (R U R' U') M' (U R U' r'), Half_Symmetric\n\
\n\
*Full, Lightning\n\
Lightning,r U R' U R U2' r'\n\
Reverse_Lightning,r' U' R U' R' U2 r\n\
Downstairs,r' (R2 U R' U R U2 R') U M'\n\
Upstairs,M' (R' U' R U' R' U2 R) U' M\n\
Big_Lightning,R' F (R U R' U') F' U R\n\
Lefty_Big_Lightning, L F' (L' U' L U) F U' L'\n\
\n\
*Full, P_Shape\n\
P,F U R U' R' F'\n\
Inverse_P,F' U' L' U L F\n\
Couch,R' U' F (U R U' R') F' R\n\
Anti_Couch,S (R U R' U') (R' F R f')\n\
\n\
*Full, C_Shape\n\
Seeing_Headlights,R' U' (R' F R F') U R\n\
City,(R U R2' U') (R' F R U) R U' F'\n\
\n\
*Full, Fish\n\
Mounted_Fish,F R' F' R U R U' R'\n\
Fish_Salad,(R U2') (R2' F R F') (R U2' R')\n\
Kite,(R U R' U') R' F (R2 U R' U') F'\n\
Anti_Kite,(R U R' U) (R' F R F') (R U2' R')\n\
\n\
*Full, L_Shape\n\
Breakneck,F (R U R' U') (R U R' U') F'\n\
Anti_Breakneck,F' L' U' L U L' U' L U F\n\
Frying_Pan,(r' U' R U') (R' U R U') R' U2 r\n\
Anti_Frying_Pan,(r U R' U) (R U' R' U) R U2' r'\n\
Front_Squeezy,r' U r2 U' r2' U' r2 U r'\n\
Back_Squeezy,r U' r2' U r2 U r2' U' r\n\
\n\
*Full, W_Shape\n\
Mario,(R U R' U) (R U' R' U') (R' F R F')\n\
Wario,(R' U' R U') (R' U R U) l U' R' U x\n\
\n\
*Full, Line\n\
Ant,F (U R U' R') (U R U' R') F'\n\
Rice_Cooker,(R U R' U R U') y (R U' R') F' y', Practice\n\
Streetlights,r U r' (U R U' R') (U R U' R') r U' r', Half_Symmetric, Practice\n\
Highway,(R' F R U) (R U' R2' F') R2 U' R' (U R U R'), Half_Symmetric, Practice\n\
\n\
*Full, Knight\n\
Squeegee,(r' U' r) (R' U' R U) (r' U r)\n\
Anti_Squeegee,(r U r') (R U R' U') (r U' r')\n\
Gun,F U R U' R2' F' R U (R U' R')\n\
Anti_Gun,(R' F R) (U R' F' R) (F U' F')\n\
\n\
*Full, Awkward, Practice\n\
Poodle,(R U R' U R U2' R') F (R U R' U') F'\n\
Anti_Poodle,(R' U' R U' R' U2 R) F (R U R' U') F'\n\
Bruh,r2 D' (r U r') D r2 (U' r' U' r)\n\
Anti_Bruh,F U (R U2 R' U') (R U2 R' U') F'\n\
",
		L"OLL");



}


void CQBertGUIDlg::OnBnClickedBtnKeep()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_keepText = !m_keepText;

}


void CQBertGUIDlg::OnBnClickedBtnReset()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	m_Cube.reset();
	m_Cmds.clear();
	setCubeText();
}


void CQBertGUIDlg::OnBnClickedBtnUndomore()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	//Write back cmd just for posterity
	if (m_Cmds.size() >= 1)
	{
		std::vector<FunnyCube::RotInfo> &cmds = m_Cmds[m_Cmds.size() - 1];

		//get funny string
		std::wstring text;
		for (unsigned i = 0; i < cmds.size(); ++i)
		{
			text += m_Cube.rotToNotationW(cmds[i]);
			text += L" ";
		}
		CComboCmd.SetWindowTextW(text.c_str());

		for (unsigned i = 0; i < cmds.size(); ++i)
		{
			FunnyCube::RotInfo r = cmds[cmds.size() - i - 1];
			r.cwturn = -r.cwturn;
			m_Cube.turn(r);
		}

		m_Cmds.pop_back();
	}


	setCubeText();
}


void CQBertGUIDlg::OnBnClickedBtnUndo1()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	if (m_Cmds.size() >= 1)
	{
		std::vector<FunnyCube::RotInfo> &cmds = m_Cmds[m_Cmds.size() - 1];

		//get funny string
		if (cmds.size() >= 1)
		{
			std::wstring text(m_Cube.rotToNotationW(cmds[cmds.size()-1]));
			CComboCmd.SetWindowTextW(text.c_str());


			FunnyCube::RotInfo r = cmds[cmds.size() - 1];
			r.cwturn = -r.cwturn;
			m_Cube.turn(r);

			cmds.pop_back();
		}
		
		if (cmds.size() == 0) m_Cmds.pop_back();
	}


	setCubeText();
}


void CQBertGUIDlg::OnBnClickedBtnCustom()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	//std::vector<MyCubeLite::RotLite> rotsLite = m_Cube.getLite().solve(22);

	m_Cube.assignPieceInfo();

	std::vector<MyCube::RotInfo> rots = m_Cube.solve(23);

	//get funny string
	std::wstring text = m_Cube.rotsToNotationW(rots);
	text += L"\n";
	text += m_Cube.rotsToNotationW(m_Cube.reverseRots(rots));

	text += L"\n" + std::to_wstring(rots.size());

	setCubeText();

	CString cText;
	m_RichDisplay.GetWindowTextW(cText);
	std::wstring curText(cText);
	curText += L"\n\n" + text;
	m_RichDisplay.SetWindowTextW(curText.c_str());

	//colorCubeText(0);
}


void CQBertGUIDlg::OnBnClickedBtnCustom2()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。

	srand(unsigned(time(NULL)));

	std::wstring simpleFormula, extendedFormula, getToFormula;
	
	std::wstring filler;

	//OLL
	AlgSaver::Alg a = m_Algs.getCategory(L"OLL").getTagged(L"Practice").getRandom();

	if (a.index != -1)
	{
		//Manage algsaver numbers
		m_Algs.updateRandTickets(m_Algs.getCategory(L"OLL").getTagged(L"Practice"), a, -1, 0);

		//First OLL AUF thing
		if (!AlgSaver::hasTag(a, L"Symmetric")) filler = AlgSaver::randomSideTurn(L"U", AlgSaver::hasTag(a, L"Half_Symmetric"));
		else filler = L"";
		if (filler != L"")
		{
			simpleFormula += filler + L" ";
			extendedFormula += filler + L" ";
		}

		simpleFormula += a.category + L"-" + a.name + L" ";
		extendedFormula += a.alg + L" ";

	}

	//PLL
	a = m_Algs.getCategory(L"PLL").getRandom();
	if (a.index != -1)
	{
		//Manage algsaver numbers
		m_Algs.updateRandTickets(m_Algs.getCategory(L"PLL"), a, -1, 0);

		//Before PLL
		if (!AlgSaver::hasTag(a, L"Symmetric")) filler = AlgSaver::randomSideTurn(L"U", AlgSaver::hasTag(a, L"Half_Symmetric"));
		else filler = L"";
		if (filler != L"")
		{
			simpleFormula += filler + L" ";
			extendedFormula += filler + L" ";
		}

		simpleFormula += a.category + L"-" + a.name + L" ";
		extendedFormula += a.alg + L" ";

	}

	//AUF
	filler = AlgSaver::randomSideTurn(L"U");
	if (filler != L"")
	{
		simpleFormula += filler + L" ";
		extendedFormula += filler + L" ";
	}

	//Info stuff and stuff
	MyCube algsDo;
	algsDo.turn(algsDo.strToRots(extendedFormula));

	std::vector<MyCube::RotInfo> rots = algsDo.solve(21);

	//get funny string
	getToFormula = algsDo.rotsToNotationW(rots);


	std::wstring text;
	text += L"\nReverse: " + getToFormula;
	text += L"\n\n\nSimple: " + simpleFormula;
	text += L"\nExtended: " + extendedFormula;

	setCubeText();

	CString cText;
	m_RichDisplay.GetWindowTextW(cText);
	std::wstring curText(cText);
	curText += L"\n\n" + text;
	m_RichDisplay.SetWindowTextW(curText.c_str());

	//colorCubeText(0);
	

}


void CQBertGUIDlg::OnBnClickedBtnCustom3()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CQBertGUIDlg::OnBnClickedBtnCustom4()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}
