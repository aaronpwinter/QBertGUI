
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
	MyCube c;

	MyCube d = c;

	c.turn({ c.R, 1, 1, true });

	d.turn({ c.L, 1, 1, true });

	MyCube e(c);

	e.turn({ c.U, 1, 1, true });

	int i = 1;

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
	getCmdAndRotate();
}

void CQBertGUIDlg::setCubeText()
{

	std::wstring text = getCubeText();
	text += getExtraInfoText();

	//m_RichDisplay.SetWindowTextW(m_Cube.asString().c_str());
	m_RichDisplay.SetWindowTextW(text.c_str());

	//COLOR IN THE SIDES !!!
	colorCubeText(0);


}

void CQBertGUIDlg::getCmdAndRotate()
{
	CString other;
	CComboCmd.GetWindowTextW(other);

	std::vector<FunnyCube::RotInfo> rots = m_Cube.strToRots(std::wstring(other));

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
			cf.crTextColor = m_Cube.getColorRef(FunnyCube::Up, row, col);
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
			cf.crTextColor = m_Cube.getColorRef(FunnyCube::Left, row, col);
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
			cf.crTextColor = m_Cube.getColorRef(FunnyCube::Front, row, col);
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
			cf.crTextColor = m_Cube.getColorRef(FunnyCube::Right, row, col);
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
			cf.crTextColor = m_Cube.getColorRef(FunnyCube::Back, row, col);
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
			cf.crTextColor = m_Cube.getColorRef(FunnyCube::Down, row, col);
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
	/*
	//Write back cmd just for posterity
	if (m_Cmds.size() >= 1)
	{
		text += L"\n\n\nOrg:";
		std::vector<FunnyCube::RotInfo> &cmds = m_Cmds[m_Cmds.size() - 1];

		for (unsigned i = 0; i < cmds.size(); ++i)
		{
			text += L" ";
			text += m_Cube.rotToNotation(cmds[i]);
		}
		text += L"\nRev:";
		for (unsigned i = 0; i < cmds.size(); ++i)
		{
			text += L" ";
			FunnyCube::RotInfo r = cmds[cmds.size() - i - 1];
			r.cwturn = -r.cwturn;
			text += m_Cube.rotToNotation(r);
		}
	}
	*/

	//Write Solve info
	auto p1 = m_Cube.getPhase1Coords();
	auto p2 = m_Cube.getPhase2Coords();
	text += L"\n\nPHASE 1 (";
	text += std::to_wstring(p1.x1) + L", " + std::to_wstring(p1.x2) + L", " + std::to_wstring(p1.x3) + L")";
	text += L" -> " + std::to_wstring(m_Cube.stepsFromG1()) + L"\n";
	text += L"G1? " + std::to_wstring(m_Cube.isG1()) + L"\n";

	text += L"\nPHASE 2 (";
	text += std::to_wstring(p2.x1) + L", " + std::to_wstring(p2.x2) + L", " + std::to_wstring(p2.x3) + L")";
	if (m_Cube.isG1()) text += L" -> " + std::to_wstring(m_Cube.stepsFromSolved()) + L"\n";
	else text += L"\n";
	text += L"Solved? " + std::to_wstring(m_Cube.solved());

	return text;


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
			text += m_Cube.rotToNotation(cmds[i]);
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
			std::wstring text(m_Cube.rotToNotation(cmds[cmds.size()-1]));
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
	std::vector<MyCube::RotInfo> rots = m_Cube.solve();

	//get funny string
	std::wstring text = m_Cube.rotsToNotation(rots);
	text += L"\n";
	text += m_Cube.rotsToNotation(m_Cube.reverseRots(rots));

	text += L"\n" + std::to_wstring(rots.size());

	setCubeText();

	CString cText;
	m_RichDisplay.GetWindowTextW(cText);
	std::wstring curText(cText);
	curText += L"\n\n" + text;
	m_RichDisplay.SetWindowTextW(curText.c_str());

	colorCubeText(0);
}
