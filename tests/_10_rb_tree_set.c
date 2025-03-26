#include "codex/allocators/debug_allocator.h"
#include "codex/ds/comparator.h"
#include "codex/ds/rbtree.h"

#include "codex/testing/assert.h"
#include "rbtree.h"


int main() {
	Allocator *a = form_debug_allocator(&std_allocator);

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
	
	size_t excluded_word_count = 6;
	char excluded_words[][16] = {
    	"GlitchSigil",
    	"EchoChamber",
    	"FakeRoot",
    	"FalseSelf",
    	"HyperVoid",
    	"PhantomKey"
	};

	assert_valid_rbtree(rb);
	for (size_t i = 0; i < word_count; ++i) {
		rb->set(rb, words[i], NULL);
	}

	for (size_t i = 0; i < excluded_word_count; ++i) {
		RBTreeNode *node = rb->get(rb, excluded_words[i]);
		assert_null(node);
	}

	for (size_t i = 0; i < word_count; ++i) {
		RBTreeNode *node = rb->get(rb, words[i]);
		assert_not_null(node);
	}
	// TODO: memory check
	return 0;
}