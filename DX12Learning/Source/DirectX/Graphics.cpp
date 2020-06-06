#include "Graphics.h"

namespace Alrescha
{
	void Graphics::Create()
	{
		//@brief デバイスの生成.
		//@pram nullptr デバイス作成時に使用するビデオアダプターのポイント(nullptrの場合はデフォルト).
		//@pram 最低限必要なフィーチャーレベル.
		//@pram 可変長引数.
		//@remarks この関数はデバッグ用です。デバッグ時にしか動作しません.
		//参考：https://docs.microsoft.com/ja-jp/windows/win32/api/d3d12/nf-d3d12-d3d12createdevice.
		//==================================================
		//IID_PPV_ARGS関数:ポインターのアドレスを引数に渡すと
		//REFIIDとポインターのアドレスに解釈される.
		//==================================================

		D3D_FEATURE_LEVEL levels[] =
		{
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL featureLevel;

		//デバイス生成が可能なフィーチャーレベルを検索.
		for (auto&& lv : levels)
		{
			if (D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(&m_Deveice)) == S_OK)
			{
				//可能なレベルを格納する.
				featureLevel = lv;
				break;
			}
		}

		//ファクトリ生成.
		auto result = CreateDXGIFactory1(IID_PPV_ARGS(&m_DxgiFactory));

		//アダプターの列挙用.
		std::vector<IDXGIAdapter*> adapters;

		//ここに特定の名前を持つアダプターオブジェクトが入る.
		IDXGIAdapter* tmpAdapter = nullptr;

		//利用可能なアダプターを列挙.
		for (int i = 0; m_DxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			adapters.push_back(tmpAdapter);
		}

		for (auto&& adpt : adapters)
		{
			DXGI_ADAPTER_DESC desc = {};
			adpt->GetDesc(&desc);//アダプターの説明オブジェクト取得.

			//アダプターの名前を取得.
			std::wstring strDesc = desc.Description;

			//探したいアダプターの名前を確認.
			if (strDesc.find(L"NVIDIA") != std::string::npos)
			{
				tmpAdapter = adpt;
				break;
			}
		}



	}
}