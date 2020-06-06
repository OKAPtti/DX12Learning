#pragma once

namespace Alrescha
{
	class Window
	{
	public:

		//=====================.
		//����/���.
		//=====================.
		bool Create();

		void Destroy();

		//=====================.
		//�擾.
		//=====================.

		HWND GetHwnd()const { return m_Hwnd; }
	private:
		//�E�B���h�E�n���h��.
		HWND m_Hwnd = nullptr;

		static LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	};

}