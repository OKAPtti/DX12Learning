#include <Windows.h>
#include <wrl.h>

#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

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

#ifdef _DEBUG
int main()
{
	

#else
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
#endif
	DebugOutputFormatString("Show window test.");
	getchar();
	return 0;
}


