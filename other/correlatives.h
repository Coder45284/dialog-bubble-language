#define MAX_CORRELATIVE_TYPES 5
#define MAX_CORRELATIVE_KINDS 5

const char CORRELATIVE_TYPE_TABLE[MAX_CORRELATIVE_TYPES][12] = {
    "QieQelQoe", "Seh", "Sel", "Teh", "Weh"};
//   None         Every  Some   What   That

const char CORRELATIVE_KINDS_TABLE[MAX_CORRELATIVE_KINDS][12] = {
    "SeeQel", "SeeQolTeh", "SeeTel", "SeeWeh", "SeeWoe")};
//   Place     Amount       Reason    Time      Demonstrative

const char CORRELATIVE_TABLE[MAX_CORRELATIVE_TYPES][MAX_CORRELATIVE_KINDS][24] = {
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

