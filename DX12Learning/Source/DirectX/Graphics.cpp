#include "Graphics.h"

namespace Alrescha
{
	void Graphics::Create(HWND hwnd)
	{
#ifdef _DEBUG
		//デバッグレイヤーをオンに.
		EnableDebugLayer();
#endif // _DEBUG


		//@brief デバイスの生成.
		//@pram nullptr デバイス作成時に使用するビデオアダプターのポイント(nullptrの場合はデフォルト).
		//@pram 最低限必要なフィーチャーレベル.
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
#ifdef _DEBUG
		auto result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG,IID_PPV_ARGS(&m_DxgiFactory));
#else
		auto result = CreateDXGIFactory1(IID_PPV_ARGS(&m_DxgiFactory));
#endif // _DEBUG

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

		//@brief コマンドアロケーターの作成(https://docs.microsoft.com/ja-jp/windows/win32/api/d3d12/nf-d3d12-id3d12device-createcommandallocator).
		//@pram D3D12_COMMAND_LIST_TYPE_DIRECT：コマンドリストのタイプを指定.
		//参考：https://docs.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_command_list_type.
		result = m_Deveice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CmdAllocator));

		//@brief コマンドリストの作成(https://docs.microsoft.com/ja-jp/windows/win32/api/d3d12/nf-d3d12-id3d12device-createcommandlist).
		//@pram 0 シングルGPUの動作の場合は０を指定
		//@pram D3D12_COMMAND_LIST_TYPE_DIRECT：コマンドリストのタイプを指定.
		//参考：https://docs.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_command_list_type.
		result = m_Deveice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CmdAllocator, nullptr, IID_PPV_ARGS(&m_CmdList));

		//@brief コマンドキューの作成.
		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};

		//タイムアウトなし.
		cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		//アダプターを１つしか使わないときは０で良い,
		cmdQueueDesc.NodeMask = 0;

		//プライオリティは特に指定なし.
		cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

		//コマンドリストと合わせる.
		cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		//キュー作成.
		result = m_Deveice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_CmdQueue));

		//スワップチェーン作成.
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};

		swapchainDesc.Width = 1280;
		swapchainDesc.Height = 720;
		swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchainDesc.Stereo = false;
		swapchainDesc.SampleDesc.Count = 1;
		swapchainDesc.SampleDesc.Quality = 0;
		swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
		swapchainDesc.BufferCount = 2;

		//バックバッファーは伸び縮み可能.
		swapchainDesc.Scaling = DXGI_SCALING_STRETCH;

		//フリップ後は速やかに破棄.
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		//特に指定なし.
		swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

		//ウィンドウ←→フルスクリーン切り替え可能.
		swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		result = m_DxgiFactory->CreateSwapChainForHwnd(
			m_CmdQueue,
			hwnd,
			&swapchainDesc,
			nullptr,
			nullptr,
			(IDXGISwapChain1**)&m_SwapChain
		);

		//ディスクリプタヒープを作成.
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//レンダーターゲットビューなのでRTV.
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = 2;//表裏の２つ.
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;//特に指定なし.

		ID3D12DescriptorHeap* rtvHeaps = nullptr;

		result = m_Deveice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&rtvHeaps));

		DXGI_SWAP_CHAIN_DESC swcDesc = {};

		result = m_SwapChain->GetDesc(&swcDesc);

		std::vector<ID3D12Resource*> backBuffers(swcDesc.BufferCount);
		for (int idx = 0; idx < swcDesc.BufferCount; ++idx)
		{
			result = m_SwapChain->GetBuffer(idx, IID_PPV_ARGS(&backBuffers[idx]));

			D3D12_CPU_DESCRIPTOR_HANDLE handle = rtvHeaps->GetCPUDescriptorHandleForHeapStart();
			handle.ptr += idx * m_Deveice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

			m_Deveice->CreateRenderTargetView(backBuffers[idx], nullptr, handle);

		}

		result = m_CmdAllocator->Reset();

		//現在のバックバッファーを指すインデックスを取得.
		auto bbIdx = m_SwapChain->GetCurrentBackBufferIndex();

		auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();

		rtvH.ptr += bbIdx * m_Deveice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		m_CmdList->OMSetRenderTargets(1, &rtvH, true, nullptr);

		//画面クリア.
		float clearColor[] = { 1.0f,1.0f,0.0f,1.0f };//黄色.

		m_CmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

		//命令のクローズ.
		m_CmdList->Close();

		//コマンドリストの実行.
		ID3D12CommandList* cmdLists[] = { m_CmdList };

		m_CmdQueue->ExecuteCommandLists(1, cmdLists);

		m_CmdAllocator->Reset();//キューをクリア.
		m_CmdList->Reset(m_CmdAllocator, nullptr);//再びコマンドリストからためる準備.

		//フリップ.
		m_SwapChain->Present(1, 0);

	}
	void Graphics::EnableDebugLayer()
	{
		ID3D12Debug* debugLayer = nullptr;
		auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));

		debugLayer->EnableDebugLayer();//デバッグレイヤーを有効化する.
		debugLayer->Release();//有効かしたらインターフェースを開放する.

	}
}