#include "./graphics/api/directx11/resources/buffer/dx11_compute_buffer.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

namespace Neon
{
	namespace Graphics
	{
		DX11ComputeBuffer::DX11ComputeBuffer(const ComputeBufferDescriptor* _computeBufferDescriptor)
			: ComputeBuffer(_computeBufferDescriptor)
		{
			D3D11_BUFFER_DESC descGPUBuffer = {};
			descGPUBuffer.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
			descGPUBuffer.ByteWidth = _computeBufferDescriptor->Size;
			descGPUBuffer.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
			descGPUBuffer.StructureByteStride = 4;

			// Create the buffer
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateBuffer(&descGPUBuffer, NULL, &m_ComputeBuffer));

			D3D11_BUFFER_DESC descBuf = {};
			m_ComputeBuffer->GetDesc(&descBuf);

			D3D11_UNORDERED_ACCESS_VIEW_DESC descView = {};
			descView.ViewDimension		 = D3D11_UAV_DIMENSION_BUFFER;
			descView.Buffer.FirstElement = 0;
			descView.Format				 = DXGI_FORMAT_UNKNOWN;
			descView.Buffer.NumElements  = descBuf.ByteWidth / descBuf.StructureByteStride;

			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateUnorderedAccessView(m_ComputeBuffer, &descView, &m_ComputeBufferView));
		}

		DX11ComputeBuffer::~DX11ComputeBuffer()
		{

		}

		void* DX11ComputeBuffer::GetData()
		{
			ID3D11Buffer* debugbuf = NULL;

			D3D11_BUFFER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));
			m_ComputeBuffer->GetDesc(&desc);

			UINT byteSize = desc.ByteWidth;

			desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
			desc.Usage = D3D11_USAGE_STAGING;
			desc.BindFlags = 0;
			desc.MiscFlags = 0;

			if (SUCCEEDED(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateBuffer(&desc, NULL, &debugbuf)))
			{
				DX11GraphicsContext::GetInstance()->GetGraphicsDeviceContext()->CopyResource(debugbuf, m_ComputeBuffer);

				D3D11_MAPPED_SUBRESOURCE mappedResource;
				if (DX11GraphicsContext::GetInstance()->GetGraphicsDeviceContext()->Map(debugbuf, 0, D3D11_MAP_READ, 0, &mappedResource) != S_OK)
					return false;

				byte* outBuff = new byte[m_Size];
				memcpy(outBuff, mappedResource.pData, m_Size);

				DX11GraphicsContext::GetInstance()->GetGraphicsDeviceContext()->Unmap(debugbuf, 0);

				for (int x = 0; x < 16; x++)
				{
					for (int y = 0; y < 16; y++)
					{
						int i = outBuff[(y * 16 + x) * sizeof(int)];
						printf("%i, ", i);
					}
					printf("\n");
				}
			}
			return nullptr;
		}
	}
}