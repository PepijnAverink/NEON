#include "./graphics/api/directx11/resources/texture/dx11_texture2D.h"
#include "./graphics/api/directx11/objects/command/dx11_command_buffer.h"
#include "./graphics/api/directx11/dx11_graphics_context.h"
#include "./graphics/api/directx11/dx11_error.h"

#include "./utilities/casting/casting_helper.h"

namespace Neon
{
	namespace Graphics
	{
		DX11Texture2D::DX11Texture2D(CommandBuffer* _commandBuffer, const Texture2DDescriptor* _texture2DDectiptor)
			: Texture2D(_texture2DDectiptor)
		{
			unsigned int* data = new unsigned int[m_Width * m_Height];
			for (int i = 0; i < m_Width * m_Height; ++i)
			{
				data[i] = _texture2DDectiptor->Color;
			}

			// Setup the description of the texture
			D3D11_TEXTURE2D_DESC textureDesc;
			textureDesc.Height				= m_Height;
			textureDesc.Width				= m_Width;
			textureDesc.MipLevels			= 0;
			textureDesc.ArraySize			= 1;
			textureDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.SampleDesc.Count	= 1;
			textureDesc.SampleDesc.Quality	= 0;
			textureDesc.Usage				= D3D11_USAGE_DEFAULT;
			textureDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			textureDesc.CPUAccessFlags		= 0;
			textureDesc.MiscFlags			= D3D11_RESOURCE_MISC_GENERATE_MIPS;

			// Create the empty texture
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateTexture2D(&textureDesc, NULL, &m_texture));

			// Copy the targa image data into the texture
			unsigned int rowPitch = m_Width * sizeof(unsigned int);
			NEON_CAST(DX11CommandBuffer*, _commandBuffer)->m_DeferedContext->UpdateSubresource(m_texture, 0, NULL, data, rowPitch, 0);

			// Setup the shader resource view description
			D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
			srvDesc.Format						= textureDesc.Format;
			srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
			srvDesc.Texture2D.MostDetailedMip	= 0;
			srvDesc.Texture2D.MipLevels			= -1;

			// Create the shader resource view for the texture.
			DX11_ThrowIfFailed(DX11GraphicsContext::GetInstance()->GetGraphicsDevice()->CreateShaderResourceView(m_texture, &srvDesc, &m_textureView));
			// Set label
			DX11_ThrowIfFailed(m_textureView->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof(_texture2DDectiptor->Name) - 1, _texture2DDectiptor->Name.c_str()));

			// Generate mipmaps for this texture.
		//	DX11GraphicsContext::GetGraphicsDeviceContext()->GenerateMips(m_textureView);
		}
	}
}