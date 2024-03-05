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


keywords = {}

addPronouns(keywords)
addDelimiters(keywords)

print(len(keywords))
for key in keywords:
    print(key, ": ", keywords[key])


