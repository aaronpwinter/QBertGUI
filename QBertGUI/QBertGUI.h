
// QBertGUI.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CQBertGUIApp:
// このクラスの実装については、QBertGUI.cpp を参照してください
//

class CQBertGUIApp : public CWinApp
{
public:
	CQBertGUIApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CQBertGUIApp theApp;
