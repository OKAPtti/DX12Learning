//@brief コンソール画面にフォーマット付き文字列を表示.
//@pram format フォーマット(%dとか%fとかの).
//@pram 可変長引数.
//@remarks この関数はデバッグ用です。デバッグ時にしか動作しません.
void DebugOutputFormatString(const char* format, ...)
{
#ifdef _DEBUG
	va_list valist;
	va_start(valist, format);
	vprintf(format, valist);
	va_end(valist);
#endif // _DEBUG
}

#include "Window/Window.h"
#include "DirectX/Graphics.h"
using namespace Alrescha;

#ifdef _DEBUG
int main()
{
	SPtr<Window> window = std::make_shared<Window>();
	if (window->Create() == false)
	{
		_ASSERT_EXPR(false, L"ウィンドウの生成に失敗");
	}
	
	DXGraphics.Create(window->GetHwnd());

	MSG msg = {};

	while (true)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//アプリケーション終了.
		if (msg.message == WM_QUIT) break;

	}

	window->Destroy();

#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif
	DebugOutputFormatString("Show window test.");
	getchar();
	return 0;
}

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	//ウィンドウが破棄されたとき.
	if (msg == WM_DESTROY)
	{
		PostQuitMessage(0);		//OSに対して「もうこのアプリは終わる」と伝える.
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}
