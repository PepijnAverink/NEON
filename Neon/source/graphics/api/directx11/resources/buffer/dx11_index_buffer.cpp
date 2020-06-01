#include "./graphics/api/directx11/resources/buffer/dx11_index_buffer.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

namespace Neon
{
	namespace Graphics
	{
		DX11IndexBuffer::DX11IndexBuffer(CommandBuffer* _commandBuffer, const IndexBufferDescriptor* _indexBufferDescriptor)
			: IndexBuffer(_indexBufferDescriptor)
		{
			m_IndexBufferFormat = DXGI_FORMAT_R16_UINT;

			D3D11_BUFFER_DESC indexBufferDesc;
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.ByteWidth = _indexBufferDescriptor->Size;
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.CPUAccessFlags = 0;
			indexBufferDesc.MiscFlags = 0;
			indexBufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA indexData;
			indexData.pSysMem = _indexBufferDescriptor->Indices;
			indexData.SysMemPitch = 0;
			indexData.SysMemSlicePitch = 0;

			// Create the index buffer
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer));

			// Set label
			DX11_ThrowIfFailed(m_IndexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(_indexBufferDescriptor->Name) - 1, _indexBufferDescriptor->Name.c_str()));
		}

		DX11IndexBuffer::~DX11IndexBuffer()
		{

		}

		void* DX11IndexBuffer::Map() const
		{
			return nullptr;
		}

		void DX11IndexBuffer::Unmap() const
		{

		}
	}
}