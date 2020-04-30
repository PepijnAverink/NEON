#pragma once
#include "./graphics/objects/command_generic/topology_state.h"
#include "./graphics/api/directx12/objects/command_generic/dx12_topology.h"

namespace Neon
{
	namespace Graphics
	{
		class DX12TopologyState : public TopologyState
		{
		public:
			DX12TopologyState(const Topology _topology);
			virtual ~DX12TopologyState();

			virtual void SetTopology(const Topology _topology) override;

		private:
			friend class DX12CommandBuffer;
			D3D_PRIMITIVE_TOPOLOGY m_InternalTopology;
		};
	}
}