#pragma once

#include "Halo.h"
#include <string>

namespace halo
{


	s_tag_entry* LookupTag(ident tagId);
	s_tag_entry* LookupTag(s_tag_type type, const std::string& tag_name);
	void BuildTagCache(); // call at start of each new game.
}