
// QBertGUIDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "QBertGUI.h"
#include "QBertGUIDlg.h"
#include "afxdialogex.h"


#include <string>
#include <sstream>

#include <iostream>
#include <fstream>


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
	m_Algs.addAlgsFromFile(L"..\\Notes\\Data\\pll.qbert", L"PLL");
	m_Algs.addAlgsFromFile(L"..\\Notes\\Data\\oll.qbert", L"OLL");
	m_Algs.addAlgsFromFile(L"..\\Notes\\Data\\5x5L2E.qbert", L"5x5L2E");

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

	//file to string
	std::wifstream file(L"..\\Notes\\Data\\recall.qbert");
	std::wstring allLines = std::wstring((std::istreambuf_iterator<wchar_t>(file)), std::istreambuf_iterator<wchar_t>());

	std::wstringstream ss(allLines);
	std::wstring line;

	std::wstring alignMove = L"";

	//loop over each line
	while (std::getline(ss, line)) {
		if (!line.empty() && !line.starts_with(L"//")) {
			//if starts with "*" is category or thing
			if (line[0] == L'*' || line[0] == L'#')
			{
				AlgSaver algs = m_Algs;
				//category, tags
				auto splitted = split(line, L',');
				for (unsigned i = 0; i < splitted.size(); ++i)
				{
					std::wstring tag = splitted[i];
					if (!tag.empty())
					{
						if (tag[0] == L'*')
						{
							tag = tag.substr(1);
							algs = algs.getCategory(tag);
						}
						else if (tag[0] == L'#')
						{
							tag = tag.substr(1);
							algs = algs.getTagged(tag);
						}
					}
				}

				AlgSaver::Alg a = algs.getRandom();
				if (a.index != -1)
				{
					//Manage algsaver numbers
					m_Algs.updateRandTickets(algs, a, -1, 0);

					//Alignmnet
					if (alignMove != L"")
					{
						std::wstring filler;
						if (!AlgSaver::hasTag(a, L"Symmetric")) filler = AlgSaver::randomSideTurn(alignMove, AlgSaver::hasTag(a, L"Half_Symmetric"));
						else filler = L"";
						if (filler != L"")
						{
							simpleFormula += filler + L" ";
							extendedFormula += filler + L" ";
						}
						alignMove = L"";
					}
					simpleFormula += a.category + L"-" + a.name + L" ";
					extendedFormula += a.alg + L" ";
				}
			}
			else if (line[0] == L'!')
			{
				//Alignment of some face
				alignMove = line.substr(1);	
			}
			else
			{
				//replay just as is
				simpleFormula += line + L" ";
				extendedFormula += m_Cube.rotsToNotationW(m_Cube.strToRots(line, &m_Algs)) + L" ";
			}
		}
	}

	if (alignMove != L"")
	{
		std::wstring lastAlign = AlgSaver::randomSideTurn(alignMove);
		if (lastAlign != L"")
		{
			simpleFormula += lastAlign + L" ";
			extendedFormula += lastAlign + L" ";
		}
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
