#include "../Halo.h"
#include "../../../Common/Streams.h"

namespace halo 
{
	
	// Temporarily modifies globals\vehicle_hit_environment and uses it to apply damage.
	bool ApplyDamage(ident receiver, ident causer, 	float dmg, int flags);

	// Apply the specified damage tag to the receiver
	void ApplyDamage(ident receiver, ident causer, const s_tag_entry& dmg_tag, float multiplier, int flags);
}