#define MAX_CORRELATIVE_TYPES 5
#define MAX_CORRELATIVE_KINDS 5

typedef enum {
    CORREL_NONE  = 0, // QieQelQoe prefix
    CORREL_EVERY = 1, // Seh       prefix
    CORREL_SOME  = 2, // Sel       prefix
    CORREL_WHAT  = 3, // Teh       prefix
    CORREL_THAT  = 4  // Weh       prefix
} CorrelativeType;

typedef enum {
    CORREL_PLACE         = 0, // SeeQel    postfix
    CORREL_AMOUNT        = 1, // SeeQolTeh postfix
    CORREL_REASON        = 2, // SeeTel    postfix
    CORREL_TIME          = 3, // SeeWeh    postfix
    CORREL_DEMONSTRATIVE = 4  // SeeWoe    postfix
} CorrelativeKind;

static const char CORRELATIVE_TABLE[MAX_CORRELATIVE_TYPES][MAX_CORRELATIVE_KINDS][20] = {
    { // Place
        "Nowhere",    // None
        "Everywhere", // Every
        "Somewhere",  // Some
        "Where",      // What
        "There"       // That
    },
    { // Amount
        "None (no amount)", // None
        "All (amount)",     // Every
        "Some (amount)",    // Some
        "How much",         // What
        "That amount"       // That
    },
    { // Reason
        "No reason",    // None
        "Every reason", // Every
        "Some reason",  // Some
        "Why",          // What
        "That reason"   // That
    },
    { // Time
        "Never",     // None
        "Everytime", // Every
        "Sometime",  // Some
        "When",      // What
        "Then"       // That
    },
    { // Demonstrative
        "No one/Nothing",      // None
        "Everyone/Everything", // Every
        "Someone/Something",   // Some
        "Who/What",            // What
        "That/That (one)"      // That
    }
};

