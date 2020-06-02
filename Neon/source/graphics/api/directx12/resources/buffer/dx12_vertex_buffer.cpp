#include "./graphics/api/directx12/resources/buffer/dx12_vertex_buffer.h"
#include "./graphics/api/directx12/dx12_graphics_context.h"
#include "./graphics/api/directx12/dx12_error.h"
#include "./graphics/api/directx12/objects/command/dx12_command_buffer.h"

#include "DX12/d3dx12.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		DX12VertexBuffer::DX12VertexBuffer(CommandBuffer* _commandBuffer, const VertexBufferDescriptor* _vertexBufferDescriptor)
			: VertexBuffer(_vertexBufferDescriptor)
		{
			// create default heap
			DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(_vertexBufferDescriptor->Size),
				D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_VertexBuffer));
			m_VertexBuffer->SetName(L"Vertex Buffer Resource Heap");
		
			// create upload heap
			ID3D12Resource* vBufferUploadHeap; // TODO:: this leaks memory
			DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),  D3D12_HEAP_FLAG_NONE,  &CD3DX12_RESOURCE_DESC::Buffer(_vertexBufferDescriptor->Size),
				D3D12_RESOURCE_STATE_GENERIC_READ,  nullptr, IID_PPV_ARGS(&vBufferUploadHeap));
			vBufferUploadHeap->SetName(L"Vertex Buffer Upload Resource Heap");
		
			// store vertex buffer in upload heap
			D3D12_SUBRESOURCE_DATA vertexData = {};
			vertexData.pData				  = reinterpret_cast<BYTE*>(_vertexBufferDescriptor->Vertices);
			vertexData.RowPitch				  = _vertexBufferDescriptor->Size;
			vertexData.SlicePitch			  = _vertexBufferDescriptor->Size;
		
			// copy the data from the upload heap to the default heap
			UpdateSubresources(NEON_CAST(DX12CommandBuffer*, _commandBuffer)->m_CommandListObj, m_VertexBuffer, vBufferUploadHeap, 0, 0, 1, &vertexData);
		
			// transition the vertex buffer data from copy destination state to vertex buffer state
			NEON_CAST(DX12CommandBuffer*, _commandBuffer)->m_CommandListObj->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_VertexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

			// Fill in view object
			m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
			m_VertexBufferView.StrideInBytes  = _vertexBufferDescriptor->VertexStride;
			m_VertexBufferView.SizeInBytes    = _vertexBufferDescriptor->Size;
		}

		DX12VertexBuffer::~DX12VertexBuffer()
		{
			m_VertexBuffer->Release();
		}

		void* DX12VertexBuffer::Map() const
		{
			return nullptr;
		}

		void DX12VertexBuffer::Unmap() const
		{

		}
	}
}