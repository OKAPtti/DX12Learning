#include "Graphics.h"

namespace Alrescha
{
	void Graphics::Create()
	{
		//@brief �f�o�C�X�̐���.
		//@pram nullptr �f�o�C�X�쐬���Ɏg�p����r�f�I�A�_�v�^�[�̃|�C���g(nullptr�̏ꍇ�̓f�t�H���g).
		//@pram �Œ���K�v�ȃt�B�[�`���[���x��.
		//@pram �ϒ�����.
		//@remarks ���̊֐��̓f�o�b�O�p�ł��B�f�o�b�O���ɂ������삵�܂���.
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
		auto result = CreateDXGIFactory1(IID_PPV_ARGS(&m_DxgiFactory));

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



	}
}