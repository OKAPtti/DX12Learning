#include "Window.h"

namespace Alrescha
{

	bool Window::Create()
	{
		//�E�B���h�E�N���X�̐���&�o�^.
		WNDCLASSEX w = {};

		w.cbSize = sizeof(WNDCLASSEX);
		w.lpfnWndProc = (WNDPROC)WindowProcedure;	//�R�[���o�b�N�֐��̎w��.
		w.lpszClassName = "DX12Sample";				//�A�v���P�[�V�����N���X��.
		w.hInstance = GetModuleHandle(nullptr);		//�n���h���擾.

		RegisterClassEx(&w);						//�E�B���h�E�N���X�̓o�^.

		//�E�B���h�E�T�C�Y.
		constexpr LONG window_width = 1280;
		constexpr LONG window_height = 720;
		RECT wrc = { 0,0,window_width,window_height };

		//�E�B���h�E�̃T�C�Y��␳.
		AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

		const char* clsName = "DX12�e�X�g";

		//�E�B���h�E�I�u�W�F�N�g����.
		m_Hwnd = CreateWindow(w.lpszClassName,	//�N���X��.
			clsName,								//�^�C�g���o�[��.
			WS_OVERLAPPEDWINDOW,					//�^�C�g���o�[�Ƌ��E��������E�B���h�E.
			CW_USEDEFAULT,							//�\������x���W��OS�ɔC����.
			CW_USEDEFAULT,							//�\������y���W��OS�ɔC����.
			wrc.right - wrc.left,					//�E�B���h�E��.
			wrc.bottom - wrc.top,					//�E�B���h�E��.
			nullptr,								//�e�E�B���h�E�n���h��.
			nullptr,								//���j���[�n���h��.
			w.hInstance,							//�Ăяo���A�v���P�[�V�����n���h��.
			nullptr);								//�ǉ��p�����[�^.

		//�������s.
		if (m_Hwnd == nullptr) return false;

		//�E�B���h�E�\��.
		ShowWindow(m_Hwnd, SW_SHOW);

		//��������.
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
		//�E�B���h�E���j�����ꂽ�Ƃ�.
		if (msg == WM_DESTROY)
		{
			PostQuitMessage(0);		//OS�ɑ΂��āu�������̃A�v���͏I���v�Ɠ`����.
			return 0;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
}