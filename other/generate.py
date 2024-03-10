def addPronouns(keywords : {}):
    A = [('S','1st Person'), ('W','1st Person Exclusionary'), ('Q','2nd Person'), ('T','3rd Person')]
    B = ['\0', ('W','Neuter'), ('Q','Genderless'), ('S','Female'), ('T', 'Male')]
    C = [('S', 'Noun'), ('W','Adjective')]
    D = ['\0', 'T']

    c_header = ""

    c_header += "#define MAX_PRONOUN_PERSON  {}\n".format(len(A))
    c_header += "#define MAX_PRONOUN_GENDER  {}\n".format(len(B))
    c_header += "#define MAX_PRONOUN_TYPES   {}\n".format(len(C))
    c_header += "#define MAX_PRONOUN_PLURALS {}\n".format(len(D))
    c_header += "\n"
    c_header += "const char PRONOUN_TABLE[MAX_PRONOUN_PERSON][MAX_PRONOUN_GENDER][MAX_PRONOUN_TYPES][MAX_PRONOUN_PLURALS][32] = {"

    for a in A:
        person = "{}ie".format(a[0])
        person_description = a[1]

        c_header += "\n    { \\\\ " + "{}\n".format(person_description)

        for b in B:
            gender = ""
            gender_description = ""

            if b != '\0':
                gender = "{}el".format(b[0])
                gender_description = "; " + b[1]

            for c in C:
                status = "{}oe".format(c[0])
                status_description = c[1]

                for d in D:
                    pronoun = person+gender+status
                    description = "Pronoun: " + person_description + gender_description + "; " + status_description

                    if d != '\0':
                        pronoun += "{}oe".format(d[0])
                        description += "; Plural"
                    else:
                        description += "; Singular"

                    if pronoun in keywords:
                        print("Error: There should be no duplicate pronouns. '{}' has been declared before.".format(pronoun))

                    keywords[pronoun] = description

        if a != A[-1]:
            c_header += "    },"
        else:
            c_header += "    }\n"

    c_header += "};\n"

    print(c_header)

def addConjunctions(keywords : {}):
    conjunctions = []
    conjunctions.append(("Seh", "And conjunction"))
    conjunctions.append(("Sel", "Or conjunction"))
    conjunctions.append(("Tel", "Because conjunction"))
    conjunctions.append(("Weh", "So conjunction"))

    for d in conjunctions:
        if d[0] in keywords:
            print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

        keywords[d[0]] = d[1]

def addIfElses(keywords : {}):
    delimiters = []
    delimiters.append(("Tie", "If <sentences>"))
    delimiters.append(("Teh", "Else if"))
    delimiters.append(("Tee", "Else"))
    delimiters.append(("Toe", "Then"))
    delimiters.append(("Sie", "Group begin"))
    delimiters.append(("Soe", "Group end"))

    for d in delimiters:
        if d[0] in keywords:
            print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

        keywords[d[0]] = d[1]

def addDelimiters(keywords : {}):
    delimiters = []
    delimiters.append(("See", "End of sentence"))
    delimiters.append(("Qee", "End of sentence with Yes/No?"))

    for d in delimiters:
        if d[0] in keywords:
            print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

        keywords[d[0]] = d[1]

def addPrepositions(keywords : {}):
    # These where influnced from Toki Pona.
    # I had divided there meanings from Toki Pona and came up with 10 prepositions.
    # Esperanto has over 20 prepositions, so I decided it would be easier to to use Toki Pona's instead.
    prepositions = []
    prepositions.append(("SelSel", "Preposition: 'similar to' noun"))
    prepositions.append(("TelSeh", "Preposition: because noun"))
    prepositions.append(("TeeWel", "Preposition: 'using' noun"))
    prepositions.append(("QeeTelQee", "Preposition: to noun"))
    prepositions.append(("TelQeeTel", "Preposition: from noun"))
    prepositions.append(("QehTel", "Preposition: 'on|at' noun"))
    prepositions.append(("QelSee", "Preposition: 'in/inside of' noun"))
    prepositions.append(("WelQeeQee", "Preposition: 'for' noun"))
    prepositions.append(("SehWee", "Preposition: 'because of' noun"))
    prepositions.append(("SeeSee", "Preposition: 'of' noun"))

    for d in prepositions:
        if d[0] in keywords:
            print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

        keywords[d[0]] = d[1]


def addNumbers(keywords : {}):
    one   = "Se"
    two   = "Qe"
    three = "Te"
    four  = "We"

    kind = {("e", "first place"), ("l", "second place"), ("h", "third place")}

    for k in kind:
        for x in range(0, 16):
            number = ""
            description = ""

            if x == 0:
                continue

            if (x & 0b1000) != 0:
                number += four + k[0]
            if (x & 0b0100) != 0:
                number += three + k[0]
            if (x & 0b0010) != 0:
                number += two + k[0]
            if (x & 0b0001) != 0:
                number += one + k[0]

            description = "Number {} in the {}".format(x,k[1])

            number += "Tee"

            if number in keywords:
                print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(number))

            keywords[number] = description

    kind = {("l", "lower from decimal"), ("h", "higher from decimal")}

    for k in kind:
        for x in range(0, 16):
            number = ""
            description = ""

            if x == 0:
                continue

            if (x & 0b1000) != 0:
                number += four + "e"
            if (x & 0b0100) != 0:
                number += three + "e"
            if (x & 0b0010) != 0:
                number += two + "e"
            if (x & 0b0001) != 0:
                number += one + "e"

            description = "Number {} in {}".format(x,k[1])

            number += "Te" + k[0]

            if number in keywords:
                print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(number))

            keywords[number] = description

    number_starts = []
    number_starts.append(("TeeWelTee", "Positive number beginning"))
    number_starts.append(("SeeWehTee", "Negative number beginning"))

    for d in number_starts:
        if d[0] in keywords:
            print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

        keywords[d[0]] = d[1]

def addCorrelatives(keywords : {}):
    types = [("QieQelQoe", "None"), ("Seh", "Every"), ("Sel", "Some"), ("Teh", "What"), ("Weh", "That")]
    kinds = [("SeeWeh", "Time"), ("SeeTel", "Reason"), ("SeeQolTeh", "Amount"), ("SeeWoe", "Demonstrative"),  ("SeeQel", "Place")]

    for t in types:
        for k in kinds:
            correlative = t[0] + k[0]

            if correlative in keywords:
                print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(correlative))

            keywords[correlative] = t[1] + " " + k[1]

keywords = {}

addPronouns(keywords)
addConjunctions(keywords)
addIfElses(keywords)
addDelimiters(keywords)
addPrepositions(keywords)
addNumbers(keywords)
addCorrelatives(keywords)

print("Keywords amount is ", len(keywords))
for key in keywords:
    print(key, ": ", keywords[key])


