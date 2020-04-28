#pragma once

namespace Neon
{
	// -----------------------------------------------------------
	// VersionID struct
	// Helper struct for version util
	// -----------------------------------------------------------

	struct VersionID
	{
		VersionID(const unsigned int _id)
			: ID(_id) 
		{ }
		
		VersionID(const unsigned char _major, const unsigned char _minor, const unsigned char _patch, const unsigned char _tweak)
			: Major(_major)
			, Minor(_minor)
			, Patch(_patch)
			, Tweak(_tweak)
		{}

		union
		{
			unsigned int ID = -1;
			struct { unsigned char Major, Minor, Patch, Tweak; };
		};
	};
}