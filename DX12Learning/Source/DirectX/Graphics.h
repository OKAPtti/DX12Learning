#pragma once

#include "../Singleton.h"

namespace Alrescha
{

	class Graphics :public Singleton<Graphics>
	{
	public:
		friend class Singleton<Graphics>;
		void Create();
	private:
		ID3D12Device* m_Deveice = nullptr;
		IDXGIFactory6* m_DxgiFactory = nullptr;
		IDXGISwapChain4* m_SwapChain = nullptr;
	protected:
		Graphics() {}
		~Graphics()override final {}
	};
#define DXGraphic Graphics::GetInstance()
}