#pragma once
#include <string>

namespace Neon
{
	namespace Graphics
	{
		inline std::string GetVendorByPCI(const unsigned int _PCIID)
		{
			switch (_PCIID)
			{
				case 0x10DE: return "NVidia"; 
				case 0x1002:
				case 0x1022: return "AMD";
				case 0x8086: 
				case 0x8087:
				case 0x163C: return "Intel";
			}

			return "unknown";
		}
	}
}