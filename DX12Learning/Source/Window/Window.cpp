#include "Window.h"

namespace Alrescha
{

	bool Window::Create()
	{
		//ウィンドウクラスの生成&登録.
		WNDCLASSEX w = {};

		w.cbSize = sizeof(WNDCLASSEX);
		w.lpfnWndProc = (WNDPROC)WindowProcedure;	//コールバック関数の指定.
		w.lpszClassName = "DX12Sample";				//アプリケーションクラス名.
		w.hInstance = GetModuleHandle(nullptr);		//ハンドル取得.

		RegisterClassEx(&w);						//ウィンドウクラスの登録.

		//ウィンドウサイズ.
		constexpr LONG window_width = 1280;
		constexpr LONG window_height = 720;
		RECT wrc = { 0,0,window_width,window_height };

		//ウィンドウのサイズを補正.
		AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

		const char* clsName = "DX12テスト";

		//ウィンドウオブジェクト生成.
		m_Hwnd = CreateWindow(w.lpszClassName,	//クラス名.
			clsName,								//タイトルバー名.
			WS_OVERLAPPEDWINDOW,					//タイトルバーと境界線があるウィンドウ.
			CW_USEDEFAULT,							//表示するx座標はOSに任せる.
			CW_USEDEFAULT,							//表示するy座標はOSに任せる.
			wrc.right - wrc.left,					//ウィンドウ幅.
			wrc.bottom - wrc.top,					//ウィンドウ高.
			nullptr,								//親ウィンドウハンドル.
			nullptr,								//メニューハンドル.
			w.hInstance,							//呼び出しアプリケーションハンドル.
			nullptr);								//追加パラメータ.

		//生成失敗.
		if (m_Hwnd == nullptr) return false;

		//ウィンドウ表示.
		ShowWindow(m_Hwnd, SW_SHOW);

		//生成成功.
		return true;
	}

	void Window::Destroy()
	{
		if (m_Hwnd)
		{
			DestroyWindow(m_Hwnd);
			m_Hwnd = nullptr;
		}
	}

	LRESULT Window::WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		//ウィンドウが破棄されたとき.
		if (msg == WM_DESTROY)
		{
			PostQuitMessage(0);		//OSに対して「もうこのアプリは終わる」と伝える.
			return 0;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}