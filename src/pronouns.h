#define MAX_PRONOUN_PERSON  4
#define MAX_PRONOUN_GENDER  5
#define MAX_PRONOUN_TYPES   2
#define MAX_PRONOUN_PLURALS 2

const char PRONOUN_TABLE[MAX_PRONOUN_PERSON][MAX_PRONOUN_GENDER][MAX_PRONOUN_TYPES][MAX_PRONOUN_PLURALS][32] = {
    { // 1st Person

        { // Unspecified
            { // Noun
                "I/Me", // Singular
                "We/Us" // Plural
            },
            { // Adjective
                "My", // Singular
                "Our" // Plural
            }
        },
        { // Female
            { // Noun
                "I/Me (female)", // Singular
                "We/Us (female)" // Plural
            },
            { // Adjective
                "My (female)", // Singular
                "Our (female)" // Plural
            }
        },
        { // Neuter
            { // Noun
                "I/Me (neuter)", // Singular
                "We/Us (neuter)" // Plural
            },
            { // Adjective
                "My (neuter)", // Singular
                "Our (neuter)" // Plural
            }
        },
        { // Male
            { // Noun
                "I/Me (male)", // Singular
                "We/Us (male)" // Plural
            },
            { // Adjective
                "My (male)", // Singular
                "Our (male)" // Plural
            }
        },
        { // Genderless
            { // Noun
                "I/Me (non-binary)", // Singular
                "We/Us (non-binary)" // Plural
            },
            { // Adjective
                "My (non-binary)", // Singular
                "Our (non-binary)" // Plural
            }
        }
    },
    { // 1st Person Exclusionary

        { // Unspecified
            { // Noun
                "I/Me, not you", // Singular
                "We/Us, not you" // Plural
            },
            { // Adjective
                "My, not you", // Singular
                "Our, not you" // Plural
            }
        },
        { // Female
            { // Noun
                "I/Me (female), not you", // Singular
                "We/Us (female), not you" // Plural
            },
            { // Adjective
                "My (female), not you", // Singular
                "Our (female), not you" // Plural
            }
        },
        { // Neuter
            { // Noun
                "I/Me (neuter), not you", // Singular
                "We/Us (neuter), not you" // Plural
            },
            { // Adjective
                "My (neuter), not you", // Singular
                "Our (neuter), not you" // Plural
            }
        },
        { // Male
            { // Noun
                "I/Me (male), not you", // Singular
                "We/Us (male), not you" // Plural
            },
            { // Adjective
                "My (male), not you", // Singular
                "Our (male), not you" // Plural
            }
        },
        { // Genderless
            { // Noun
                "I/Me (non-binary), not you", // Singular
                "We/Us (non-binary), not you" // Plural
            },
            { // Adjective
                "My (non-binary), not you", // Singular
                "Our (non-binary), not you" // Plural
            }
        }
    },
    { // 3rd Person

        { // Unspecified
            { // Noun
                "They (singular)", // Singular
                "They"             // Plural
            },
            { // Adjective
                "Their (singular)", // Singular
                "Their"             // Plural
            }
        },
        { // Female
            { // Noun
                "She",              // Singular
                "They (all-female)" // Plural
            },
            { // Adjective
                "Her",               // Singular
                "Their (all-female)" // Plural
            }
        },
        { // Neuter
            { // Noun
                "It",               // Singular
                "They (all-neuter)" // Plural
            },
            { // Adjective
                "Its",               // Singular
                "Their (all-neuter)" // Plural
            }
        },
        { // Male
            { // Noun
                "He",             // Singular
                "They (all-male)" // Plural
            },
            { // Adjective
                "His",             // Singular
                "Their (all-male)" // Plural
            }
        },
        { // Genderless
            { // Noun
                "They (singular,all-non-binary)", // Singular
                "They (plural,all-non-binary)"    // Plural
            },
            { // Adjective
                "Their (singular,all-non-binary)", // Singular
                "Their (plural,all-non-binary)"    // Plural
            }
        }
    },
    { // 2nd Person

        { // Unspecified
            { // Noun
                "You (singular)",  // Singular
                "You all (plural)" // Plural
            },
            { // Adjective
                "Your (singular)",   // Singular
                "You all's (plural)" // Plural
            }
        },
        { // Female
            { // Noun
                "You (female,singular)",  // Singular
                "You all (female,plural)" // Plural
            },
            { // Adjective
                "Your (female,singular)",   // Singular
                "You all's (female,plural)" // Plural
            }
        },
        { // Neuter
            { // Noun
                "You (neuter,singular)",  // Singular
                "You all (neuter,plural)" // Plural
            },
            { // Adjective
                "Your (neuter,singular)",   // Singular
                "You all's (neuter,plural)" // Plural
            }
        },
        { // Male
            { // Noun
                "You (male,singular)",  // Singular
                "You all (male,plural)" // Plural
            },
            { // Adjective
                "Your (male,singular)",   // Singular
                "You all's (male,plural)" // Plural
            }
        },
        { // Genderless
            { // Noun
                "You (non-binary,singular)",  // Singular
                "You all (non-binary,plural)" // Plural
            },
            { // Adjective
                "Your (non-binary,singular)",   // Singular
                "You all's (non-binary,plural)" // Plural
            }
        }
    }
};
