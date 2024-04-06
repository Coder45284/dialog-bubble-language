def addPronouns(keywords : {}):
    A = [('S','1st Person'), ('W','1st Person Exclusionary'), ('T','3rd Person'), ('Q','2nd Person')]
    B = ['\0', ('S','Female'), ('W','Neuter'), ('T', 'Male'), ('Q','Genderless')]
    C = [('S', 'NOUN'), ('W','ADJECTIVE')]
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

        c_header += "\n    { // " + "{}\n".format(person_description)

        for b in B:
            gender = ""
            gender_description = "Unspecified"

            if b != '\0':
                gender = "{}el".format(b[0])
                gender_description = b[1]

            c_header += "\n        { // " + "{}".format(gender_description)

            for c in C:
                status = "{}oe".format(c[0])
                status_description = c[1]

                c_header += "\n            { // " + "{}\n".format(status_description)

                for d in D:
                    pronoun = person+gender+status
                    description = "Pronoun: " + person_description + "; " +  gender_description + "; " + status_description

                    if d != '\0':
                        pronoun += "{}oe".format(d[0])
                        description += "; Plural"
                    else:
                        description += "; Singular"

                    if pronoun in keywords:
                        print("Error: There should be no duplicate pronouns. '{}' has been declared before.".format(pronoun))

                    if d != D[-1]:
                        c_header += "                \"BLANK\", // {}\n".format("Singular")
                    else:
                        c_header += "                \"BLANK\"  // {}\n".format("Plural")

                    keywords[pronoun] = (status_description, description)

                if c != C[-1]:
                    c_header += "            },"
                else:
                    c_header += "            }\n"

            if b != B[-1]:
                c_header += "        },"
            else:
                c_header += "        }\n"

        if a != A[-1]:
            c_header += "    },"
        else:
            c_header += "    }\n"

    c_header += "};\n"

    # print(c_header)

def addSentenceConjunctions(keywords : {}):
    conjunctions = []
    conjunctions.append(("Seh", "CONJUNCTION", "And"))
    conjunctions.append(("Sel", "CONJUNCTION", "Or"))
    conjunctions.append(("Tel", "CONJUNCTION", "Because"))
    conjunctions.append(("Weh", "CONJUNCTION", "So"))

    for d in conjunctions:
        if d[0] in keywords:
            print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

        keywords[d[0]] = (d[1], d[2])

def addPhraseConjunctions(keywords : {}):
    conjunctions = []
    conjunctions.append(("Qel", "PHRASE_CONJUNCTION", "And"))
    conjunctions.append(("Qeh", "PHRASE_CONJUNCTION", "Or"))

    for d in conjunctions:
        if d[0] in keywords:
            print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

        keywords[d[0]] = (d[1], d[2])

def addIfElses(keywords : {}):
    delimiters = []
    delimiters.append(("Tie", "IF"))
    delimiters.append(("Teh", "ELIF"))
    delimiters.append(("Tee", "ELSE"))
    delimiters.append(("Toe", "THEN"))
    delimiters.append(("Sie", "GROUP_BEGIN"))
    delimiters.append(("Soe", "GROUP_END"))

    for d in delimiters:
        if d[0] in keywords:
            print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

        keywords[d[0]] = (d[1], "")

def addDelimiters(keywords : {}):
    delimiters = []
    delimiters.append(("See", "DELIMITER", "END"))
    delimiters.append(("Qee", "DELIMITER", "END Yes|No"))

    for d in delimiters:
        if d[0] in keywords:
            print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

        keywords[d[0]] = (d[1], d[2])

def addPrepositions(keywords : {}):
    # These where influnced from Toki Pona.
    # I had divided there meanings from Toki Pona and came up with 10 prepositions.
    # Esperanto has over 20 prepositions, so I decided it would be easier to to use Toki Pona's instead.
    prepositions = []
    prepositions.append(("SelSel",    "PROPOSITION", "Similar to"))
    prepositions.append(("TelSeh",    "PROPOSITION", "Before"))
    prepositions.append(("TeeWel",    "PROPOSITION", "Using"))
    prepositions.append(("QeeTelQee", "PROPOSITION", "To"))
    prepositions.append(("TelQeeTel", "PROPOSITION", "From"))
    prepositions.append(("QehTel",    "PROPOSITION", "On|At"))
    prepositions.append(("QelSee",    "PROPOSITION", "In/Inside of"))
    prepositions.append(("WelQeeQee", "PROPOSITION", "For"))
    prepositions.append(("SehWee",    "PROPOSITION", "Because of"))
    prepositions.append(("SeeSee",    "PROPOSITION", "Of"))

    for d in prepositions:
        if d[0] in keywords:
            print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

        keywords[d[0]] = (d[1], d[2])


def addNumbers(keywords : {}):
    one   = "Se"
    two   = "Qe"
    three = "Te"
    four  = "We"

    kind = {("e", "NUMBER_1"), ("l", "NUMBER_10"), ("h", "NUMBER_100")}

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

            number += "Tee"

            description = "Error: Number {} in the {}".format(number,k[1])
            print(description)

            if number in keywords:
                print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(number))

            keywords[number] = (k[1], "{}".format(x))

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

            keywords[number] = ("NUMBER_PLACE", "{}".format(x))

    number_starts = []
    number_starts.append(("TeeWelTee", "NUMBER_SIGN",  1))
    number_starts.append(("SeeWehTee", "NUMBER_SIGN", -1))

    for d in number_starts:
        if d[0] in keywords:
            print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

        keywords[d[0]] = (d[1], "{}".format(d[2]))

def addCorrelatives(keywords : {}):
    types = [("QieQelQoe", "None"), ("Seh", "Every"), ("Sel", "Some"), ("Teh", "What"), ("Weh", "That")]
    kinds = [('SeeQel', 'Place'), ('SeeQolTeh', 'Amount'), ('SeeTel', 'Reason'), ('SeeWeh', 'Time'), ('SeeWoe', 'Demonstrative')]

    c_header = ""

    c_header += "#define MAX_CORRELATIVE_TYPES {}\n".format(len(types))
    c_header += "#define MAX_CORRELATIVE_KINDS {}\n".format(len(kinds))
    c_header += "\n"
    c_header += "const char CORRELATIVE_TYPE_TABLE[MAX_CORRELATIVE_TYPES][12] = {\n"
    c_header += "    \"QieQelQoe\", \"Seh\", \"Sel\", \"Teh\", \"Weh\"};\n"
    c_header += "//   None         Every  Some   What   That\n\n"
    c_header += "const char CORRELATIVE_KINDS_TABLE[MAX_CORRELATIVE_KINDS][12] = {\n"
    c_header += "    \"SeeQel\", \"SeeQolTeh\", \"SeeTel\", \"SeeTel\", \"SeeWoe\"};\n"
    c_header += "//   Place     Amount       Reason    Time      Demonstrative\n\n"
    c_header += "const char CORRELATIVE_TABLE[MAX_CORRELATIVE_TYPES][MAX_CORRELATIVE_KINDS][32] = {"

    for k in kinds:
        c_header += "\n    {" + " // {}\n".format(k[1])

        for t in types:
            correlative = t[0] + k[0]

            if correlative in keywords:
                print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(correlative))

            keywords[correlative] = ("NOUN", t[1] + " " + k[1])

            if t != types[-1]:
                c_header += "        \"BLANK\", // {}\n".format(t[1])
            else:
                c_header += "        \"BLANK\"  // {}\n".format(t[1])

        if k != kinds[-1]:
            c_header += "    },"
        else:
            c_header += "    }\n"

    c_header += "};\n"

    # print(c_header)

keywords = {}

addPronouns(keywords)
addSentenceConjunctions(keywords)
addPhraseConjunctions(keywords)
addIfElses(keywords)
addDelimiters(keywords)
addPrepositions(keywords)
addNumbers(keywords)
addCorrelatives(keywords)

print("Keywords amount is ", len(keywords))
for key in keywords:
    print(key, ": ", keywords[key])

text = 'static const char text[] = "'
token = 'static const int token_types[] = {'
return_text = 'static const char *return_text[] = {'

for key in keywords:
    text += key + " "
    token += keywords[key][0] + ", "
    return_text += '"' + keywords[key][1] + '", '

text += '";'
token += '};'
return_text += '};'

print(text, end="\n\n")
print(token, end="\n\n")
print(return_text, end="\n\n")
