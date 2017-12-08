import sqlite3

SERVER = 0
RULE = 3
DB = sqlite3.connect('protocol_db.db')
cursor = DB.cursor()


def which_servers(value_ascii):
    """
    :return: [first_server, second_server]
    """

    return [value_ascii % RULE, (value_ascii + 1) % RULE]


def sum_values_ascii(word):
    summ = 0
    for i in word:
        summ += ord(i)

    return summ


def filter_allowed(line):
    value = True
    print "line: ", line
    for l in line:
        print "l: ", l
        print "ord('a'): ", ord('a')
        print "ord(l): ", ord(l)
        print "ord('z'): ", ord('z')
        print "\n"
        print "ord('A'): ", ord('A')
        print "ord(l): ", ord(l)
        print "ord('A'): ", ord('A')
        print "ord(" "): ", ord(" ")
        print "\n"
        print "\n"
        if ((ord(l) >= ord('a') and ord(l) <= ord('z')) or
                (ord(l) >= ord('A') and ord(l) <= ord('Z')) or
                ord(l) == ord(" ") or ord(l) == ord("\n")):
            value = True
        else:
            value = False
            break
    print "------------------------------------"

    return value


def insert_node_db(word):
    print "word: ", word
    cursor.execute('''insert into nodes(name) values(:word)''', {'word': word})
    print "word inserted"
    DB.commit()


def insert_node(word):
    value_ascii = sum_values_ascii(word)
    print "value_ascii", value_ascii
    servers = which_servers(value_ascii)
    print "servers: ", servers

    for s in servers:
        if SERVER == s:
            insert_node_db(word)


def read_file(file_name):
    with open(file_name) as f:
        # line = f.readline()
        while True:
            line = f.readline()
            if line:
                print "line before: ", line
                if filter_allowed(line):
                    # continue
                    print "line after: ", line
                    words = line.split(" ")
                    print "words: ", words
                    insert_node(words[0])
                    insert_node(words[1])
            else:
                break

    # DB.commit()


if __name__ == "__main__":
    read_file('/home/lalo/college/networking/FinalProtocolGraph/en.wiki.big')
    # read_file('/home/lalo/college/networking/FinalProtocolGraph/test.txt')
    # print "sum_values_ascii: ", sum_values_ascii("hola")
