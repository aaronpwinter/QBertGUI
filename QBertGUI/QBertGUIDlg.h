
// QBertGUIDlg.h : ヘッダー ファイル
//

#pragma once


#include "FunnyCube.h"
#include "MyCube.h"
#include "MyCubeLite.h"
#include "AlgSaver.h"

// CQBertGUIDlg ダイアログ
class CQBertGUIDlg : public CDialogEx
{
// コンストラクション
public:
	CQBertGUIDlg(CWnd* pParent = nullptr);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_QBERTGUI_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


private:
	void setCubeText();
	void getCmdAndRotate();

private:
	std::wstring getCubeText(); //this is literally like the block part
	void colorCubeText(int startChar);
	std::wstring getExtraInfoText();

	void setupAlgs();

private:
	CComboBox CComboCmd;
	CRichEditCtrl m_RichDisplay;

	bool m_keepText = false;

	MyCube m_Cube;
	AlgSaver m_Algs;

	std::vector< std::vector<FunnyCube::RotInfo> > m_Cmds;

public:
	afx_msg void OnBnClickedBtnGo();
	afx_msg void OnBnClickedBtnKeep();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnUndomore();
	afx_msg void OnBnClickedBtnUndo1();
	afx_msg void OnBnClickedBtnCustom();
	afx_msg void OnBnClickedBtnCustom2();
	afx_msg void OnBnClickedBtnCustom3();
	afx_msg void OnBnClickedBtnCustom4();
};
