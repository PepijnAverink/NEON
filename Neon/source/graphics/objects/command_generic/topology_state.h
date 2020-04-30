#pragma once
#include "./graphics/objects/command_generic/topology.h"

namespace Neon
{
	namespace Graphics
	{
		class TopologyState
		{
		public:
			static TopologyState* Create(const Topology _topology);
			virtual ~TopologyState() {}

			virtual void SetTopology(const Topology _topology) = 0;
			inline const Topology GetTopology() const { return m_Topology; }

		protected:
			TopologyState(const Topology _topology);

			Topology m_Topology;
		};
	}
}