def addPronouns(keywords : {}):
    A = [('S','1st Person'), ('Q','2nd Person'), ('T','3rd Person')]
    B = ['\0', ('W','Neuter'), ('Q','Genderless'), ('S','Female'), ('T', 'Male')]
    C = [('S', 'Noun'), ('W','Adjective')]
    D = ['\0', 'T']

    for a in A:
        person = "{}ie".format(a[0])
        person_description = a[1]
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

def addDelimiters(keywords : {}):
    delimiter = []
    delimiter.append(("Sel", "OR Conjunction"))
    delimiter.append(("See", "end of sentence"))
    delimiter.append(("Seh", "AND Conjunction"))
    delimiter.append(("Qee", "end of sentence with Yes/No?"))
    delimiter.append(("Tie", "if <sentences>"))
    delimiter.append(("Tel", "Because Conjunction"))
    delimiter.append(("Toe", "end if <sentences>"))
    delimiter.append(("Weh", "So Conjunction"))
    delimiter.append(("Tee", "else"))
    delimiter.append(("Sie", "group begin"))
    delimiter.append(("Soe", "group end"))
    delimiter.append(("Teh", "else if"))

    for d in delimiter:
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
                if k[0] == "e":
                    number += "SeeWee" # Zero marker
                else:
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
                print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

            keywords[number] = description

    kind = {("l", "before decimal"), ("h", "after decimal")}

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
                print("Error: There should be no duplicate definitions. '{}' has been declared before.".format(d[0]))

            keywords[number] = description

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
addDelimiters(keywords)
addNumbers(keywords)
addCorrelatives(keywords)

print(len(keywords))
for key in keywords:
    print(key, ": ", keywords[key])


