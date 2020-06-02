#include "./graphics/api/directx12/resources/buffer/dx12_index_buffer.h"
#include "./graphics/api/directx12/dx12_graphics_context.h"
#include "./graphics/api/directx12/dx12_error.h"
#include "./graphics/api/directx12/objects/command/dx12_command_buffer.h"

#include "DX12/d3dx12.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		DX12IndexBuffer::DX12IndexBuffer(CommandBuffer* _commandBuffer, const IndexBufferDescriptor* _indexBufferDescriptor)
			: IndexBuffer(_indexBufferDescriptor)
		{
			// create default heap to hold index buffer
			DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(_indexBufferDescriptor->Size),
				D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&m_IndexBuffer));
			m_IndexBuffer->SetName(L"Index Buffer Resource Heap");

			// create upload heap to upload index buffer
			ID3D12Resource* iBufferUploadHeap;
			DX12GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(_indexBufferDescriptor->Size),
				D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&iBufferUploadHeap));
			iBufferUploadHeap->SetName(L"Index Buffer Upload Resource Heap");

			// store vertex buffer in upload heap
			D3D12_SUBRESOURCE_DATA indexData = {};
			indexData.pData = reinterpret_cast<BYTE*>(_indexBufferDescriptor->Indices);
			indexData.RowPitch = _indexBufferDescriptor->Size;
			indexData.SlicePitch = _indexBufferDescriptor->Size;

			// copy the data from the upload heap to the default heap
			UpdateSubresources(NEON_CAST(DX12CommandBuffer*, _commandBuffer)->m_CommandListObj, m_IndexBuffer, iBufferUploadHeap, 0, 0, 1, &indexData);

			// transition the vertex buffer data from copy destination state to vertex buffer state
			NEON_CAST(DX12CommandBuffer*, _commandBuffer)->m_CommandListObj->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_IndexBuffer, D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER));

			// create a vertex buffer view
			m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
			m_IndexBufferView.Format		 = DXGI_FORMAT_R16_UINT; // Abstract format
			m_IndexBufferView.SizeInBytes	 = _indexBufferDescriptor->Size;
		}

		DX12IndexBuffer::~DX12IndexBuffer()
		{
			m_IndexBuffer->Release();
		}

		void* DX12IndexBuffer::Map() const
		{
			return nullptr;
		}

		void DX12IndexBuffer::Unmap() const
		{

		}
	}
}