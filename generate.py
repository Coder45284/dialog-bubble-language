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


keywords = {}

addPronouns(keywords)

for key in keywords:
    print(key, ": ", keywords[key])


