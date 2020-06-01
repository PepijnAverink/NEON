#include "./graphics/api/directx11/resources/buffer/dx11_vertex_buffer.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

namespace Neon
{
	namespace Graphics
	{
		DX11VertexBuffer::DX11VertexBuffer(CommandBuffer* _commandBuffer, const VertexBufferDescriptor* _vertexBufferDescriptor)
			: VertexBuffer(_vertexBufferDescriptor)
		{
			D3D11_BUFFER_DESC vertexBufferDesc;
			vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			vertexBufferDesc.ByteWidth = _vertexBufferDescriptor->Size;
			vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			vertexBufferDesc.CPUAccessFlags = 0;
			vertexBufferDesc.MiscFlags = 0;
			vertexBufferDesc.StructureByteStride = 0;

			D3D11_SUBRESOURCE_DATA vertexData;
			vertexData.pSysMem = _vertexBufferDescriptor->Vertices;
			vertexData.SysMemPitch = 0;
			vertexData.SysMemSlicePitch = 0;

			// Create the vertex buffer
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer));

			// Set label
			DX11_ThrowIfFailed(m_VertexBuffer->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(_vertexBufferDescriptor->Name) - 1, _vertexBufferDescriptor->Name.c_str()));
		}

		DX11VertexBuffer::~DX11VertexBuffer()
		{

		}

		void * DX11VertexBuffer::Map() const
		{
			return nullptr;
		}

		void DX11VertexBuffer::Unmap() const
		{

		}
	}
}