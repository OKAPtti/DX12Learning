#pragma once

namespace Alrescha
{
	class Window
	{
	public:

		//=====================.
		//生成/解放.
		//=====================.
		bool Create();

		void Destroy();

		//=====================.
		//取得.
		//=====================.

		HWND GetHwnd()const { return m_Hwnd; }
	private:
		//ウィンドウハンドル.
		HWND m_Hwnd = nullptr;

		static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};

}