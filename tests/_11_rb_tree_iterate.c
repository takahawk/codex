#include "codex/mem/debug_allocator.h"
#include "codex/ds/comparator.h"
#include "codex/ds/rbtree.h"

#include "codex/testing/assert.h"


int main() {
	Allocator *a = form_debug_allocator(&STD_ALLOCATOR);

	RBTree/*char**/ *rb = form_rbtree(a, str_comparator);

	size_t word_count = 40;
	char words[][16] = {
	    "Emulacrum",
	    "Zero",
	    "Reflexia",
	    "TheMainframe",
	    "Subworld",
	    "Simulacrum",
	    "RootAccess",
	    "Anamnesis",
	    "VoidEcho",
	    "ShadowRealm",
	    "4by55",
	    "WuWei",
	    "FractalSelf",
	    "DreamLink",
	    "Backdoor",
	    "PsyShift",
	    "Karma",
	    "Kriya",
	    "Prana",
	    "TheMind",
	    "TheVoice",
	    "TheEye",
	    "TheSoul",
	    "Overworld",
	    "SubSim",
	    "SupraSim",
	    "TheGreatNet",
	    "UnrealReal",
	    "Anatta",
	    "Samsara",
	    "Maya",
	    "Monad",
	    "Emanation",
	    "TrueName",
	    "PhaseState",
	    "ZeroState",
	    "RootVoid",
	    "Sigil",
	    "Tathata",
	    "NoSelf"
	};

	char sorted_words[][16] = {
	    "4by55",
	    "Anamnesis",
	    "Anatta",
	    "Backdoor",
	    "DreamLink",
	    "Emanation",
	    "Emulacrum",
	    "FractalSelf",
	    "Karma",
	    "Kriya",
	    "Maya",
	    "Monad",
	    "NoSelf",
	    "Overworld",
	    "PhaseState",
	    "Prana",
	    "PsyShift",
	    "Reflexia",
	    "RootAccess",
	    "RootVoid",
	    "Samsara",
	    "ShadowRealm",
	    "Sigil",
	    "Simulacrum",
	    "SubSim",
	    "Subworld",
	    "SupraSim",
	    "Tathata",
	    "TheEye",
	    "TheGreatNet",
	    "TheMainframe",
	    "TheMind",
	    "TheSoul",
	    "TheVoice",
	    "TrueName",
	    "UnrealReal",
	    "VoidEcho",
	    "WuWei",
	    "Zero",
	    "ZeroState"
	};
	
	for (size_t i = 0; i < word_count; ++i) {
		rb->set(rb, words[i], NULL);
	}

	size_t i = 0;
	Iterator *it = rb->form_iterator(rb);
	while (it->has_next(it)) {
		RBTreeNode *node = it->next(it);

		assert_str_equals(node->key, sorted_words[i++]);
	}
	// TODO: memory check
	return 0;
}
