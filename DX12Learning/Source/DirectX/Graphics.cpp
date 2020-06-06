#include "Graphics.h"

namespace Alrescha
{
	void Graphics::Create(HWND hwnd)
	{
#ifdef _DEBUG
		//�f�o�b�O���C���[���I����.
		EnableDebugLayer();
#endif // _DEBUG


		//@brief �f�o�C�X�̐���.
		//@pram nullptr �f�o�C�X�쐬���Ɏg�p����r�f�I�A�_�v�^�[�̃|�C���g(nullptr�̏ꍇ�̓f�t�H���g).
		//@pram �Œ���K�v�ȃt�B�[�`���[���x��.
		//�Q�l�Fhttps://docs.microsoft.com/ja-jp/windows/win32/api/d3d12/nf-d3d12-d3d12createdevice.
		//==================================================
		//IID_PPV_ARGS�֐�:�|�C���^�[�̃A�h���X�������ɓn����
		//REFIID�ƃ|�C���^�[�̃A�h���X�ɉ��߂����.
		//==================================================

		D3D_FEATURE_LEVEL levels[] =
		{
			D3D_FEATURE_LEVEL_12_1,
			D3D_FEATURE_LEVEL_12_0,
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
		};

		D3D_FEATURE_LEVEL featureLevel;

		//�f�o�C�X�������\�ȃt�B�[�`���[���x��������.
		for (auto&& lv : levels)
		{
			if (D3D12CreateDevice(nullptr, lv, IID_PPV_ARGS(&m_Deveice)) == S_OK)
			{
				//�\�ȃ��x�����i�[����.
				featureLevel = lv;
				break;
			}
		}

		//�t�@�N�g������.
#ifdef _DEBUG
		auto result = CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG,IID_PPV_ARGS(&m_DxgiFactory));
#else
		auto result = CreateDXGIFactory1(IID_PPV_ARGS(&m_DxgiFactory));
#endif // _DEBUG

		//�A�_�v�^�[�̗񋓗p.
		std::vector<IDXGIAdapter*> adapters;

		//�����ɓ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������.
		IDXGIAdapter* tmpAdapter = nullptr;

		//���p�\�ȃA�_�v�^�[���.
		for (int i = 0; m_DxgiFactory->EnumAdapters(i, &tmpAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
		{
			adapters.push_back(tmpAdapter);
		}

		for (auto&& adpt : adapters)
		{
			DXGI_ADAPTER_DESC desc = {};
			adpt->GetDesc(&desc);//�A�_�v�^�[�̐����I�u�W�F�N�g�擾.

			//�A�_�v�^�[�̖��O���擾.
			std::wstring strDesc = desc.Description;

			//�T�������A�_�v�^�[�̖��O���m�F.
			if (strDesc.find(L"NVIDIA") != std::string::npos)
			{
				tmpAdapter = adpt;
				break;
			}
		}

		//@brief �R�}���h�A���P�[�^�[�̍쐬(https://docs.microsoft.com/ja-jp/windows/win32/api/d3d12/nf-d3d12-id3d12device-createcommandallocator).
		//@pram D3D12_COMMAND_LIST_TYPE_DIRECT�F�R�}���h���X�g�̃^�C�v���w��.
		//�Q�l�Fhttps://docs.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_command_list_type.
		result = m_Deveice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CmdAllocator));

		//@brief �R�}���h���X�g�̍쐬(https://docs.microsoft.com/ja-jp/windows/win32/api/d3d12/nf-d3d12-id3d12device-createcommandlist).
		//@pram 0 �V���O��GPU�̓���̏ꍇ�͂O���w��
		//@pram D3D12_COMMAND_LIST_TYPE_DIRECT�F�R�}���h���X�g�̃^�C�v���w��.
		//�Q�l�Fhttps://docs.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_command_list_type.
		result = m_Deveice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CmdAllocator, nullptr, IID_PPV_ARGS(&m_CmdList));

		//@brief �R�}���h�L���[�̍쐬.
		D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};

		//�^�C���A�E�g�Ȃ�.
		cmdQueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

		//�A�_�v�^�[���P�����g��Ȃ��Ƃ��͂O�ŗǂ�,
		cmdQueueDesc.NodeMask = 0;

		//�v���C�I���e�B�͓��Ɏw��Ȃ�.
		cmdQueueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;

		//�R�}���h���X�g�ƍ��킹��.
		cmdQueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		//�L���[�쐬.
		result = m_Deveice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_CmdQueue));

		//�X���b�v�`�F�[���쐬.
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};

		swapchainDesc.Width = 1280;
		swapchainDesc.Height = 720;
		swapchainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapchainDesc.Stereo = false;
		swapchainDesc.SampleDesc.Count = 1;
		swapchainDesc.SampleDesc.Quality = 0;
		swapchainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
		swapchainDesc.BufferCount = 2;

		//�o�b�N�o�b�t�@�[�͐L�яk�݉\.
		swapchainDesc.Scaling = DXGI_SCALING_STRETCH;

		//�t���b�v��͑��₩�ɔj��.
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;

		//���Ɏw��Ȃ�.
		swapchainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;

		//�E�B���h�E�����t���X�N���[���؂�ւ��\.
		swapchainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

		result = m_DxgiFactory->CreateSwapChainForHwnd(
			m_CmdQueue,
			hwnd,
			&swapchainDesc,
			nullptr,
			nullptr,
			(IDXGISwapChain1**)&m_SwapChain
		);

		//�f�B�X�N���v�^�q�[�v���쐬.
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};

		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;//�����_�[�^�[�Q�b�g�r���[�Ȃ̂�RTV.
		heapDesc.NodeMask = 0;
		heapDesc.NumDescriptors = 2;//�\���̂Q��.
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;//���Ɏw��Ȃ�.

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

		//���݂̃o�b�N�o�b�t�@�[���w���C���f�b�N�X���擾.
		auto bbIdx = m_SwapChain->GetCurrentBackBufferIndex();

		auto rtvH = rtvHeaps->GetCPUDescriptorHandleForHeapStart();

		rtvH.ptr += bbIdx * m_Deveice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		m_CmdList->OMSetRenderTargets(1, &rtvH, true, nullptr);

		//��ʃN���A.
		float clearColor[] = { 1.0f,1.0f,0.0f,1.0f };//���F.

		m_CmdList->ClearRenderTargetView(rtvH, clearColor, 0, nullptr);

		//���߂̃N���[�Y.
		m_CmdList->Close();

		//�R�}���h���X�g�̎��s.
		ID3D12CommandList* cmdLists[] = { m_CmdList };

		m_CmdQueue->ExecuteCommandLists(1, cmdLists);

		m_CmdAllocator->Reset();//�L���[���N���A.
		m_CmdList->Reset(m_CmdAllocator, nullptr);//�ĂуR�}���h���X�g���炽�߂鏀��.

		//�t���b�v.
		m_SwapChain->Present(1, 0);

	}
	void Graphics::EnableDebugLayer()
	{
		ID3D12Debug* debugLayer = nullptr;
		auto result = D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer));

		debugLayer->EnableDebugLayer();//�f�o�b�O���C���[��L��������.
		debugLayer->Release();//�L����������C���^�[�t�F�[�X���J������.

	}
}