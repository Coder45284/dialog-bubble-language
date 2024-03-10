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
                "I/Me (item)", // Singular
                "We/Us (item)" // Plural
            },
            { // Adjective
                "My (item)", // Singular
                "Our (item)" // Plural
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
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Female
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Neuter
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Male
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Genderless
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        }
    },
    { // 3rd Person

        { // Unspecified
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Female
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Neuter
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Male
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Genderless
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        }
    },
    { // 2nd Person

        { // Unspecified
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Female
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Neuter
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Male
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        },
        { // Genderless
            { // Noun
                "BLANK", // Singular
                "BLANK"  // Plural
            },
            { // Adjective
                "BLANK", // Singular
                "BLANK"  // Plural
            }
        }
    }
};
