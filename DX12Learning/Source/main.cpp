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

LRESULT WindowProcedure(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

#ifdef _DEBUG
int main()
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
	HWND hwnd = CreateWindow(w.lpszClassName,	//クラス名.
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

	//ウィンドウ表示.
	ShowWindow(hwnd, SW_SHOW);

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

	//登録解除.
	UnregisterClass(w.lpszClassName, w.hInstance);

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
