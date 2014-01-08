#pragma once

#include "Types.h"
#include <string>

std::string GetTagCacheKey(s_tag_type type, const std::string& tag_name);
s_tag_entry* LookupTag(ident tagId);
s_tag_entry* LookupTag(s_tag_type type, const std::string& tag_name);
void BuildTagCache(); // call at start of each new game.
s_tag_entry* LookupTag2(std::string key);