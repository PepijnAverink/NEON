#include "./graphics/api/directx12/objects/command_generic/dx12_topology_state.h"

namespace Neon
{
	namespace Graphics
	{
		DX12TopologyState::DX12TopologyState(const Topology _topology)
			: TopologyState(_topology)
			, m_InternalTopology(GetDX12Topology(m_Topology))
		{ }

		DX12TopologyState::~DX12TopologyState() 
		{ }

		void DX12TopologyState::SetTopology(const Topology _topology)
		{
			// Set member
			m_Topology			= _topology;
			m_InternalTopology	= GetDX12Topology(m_Topology);
		}
	}
}