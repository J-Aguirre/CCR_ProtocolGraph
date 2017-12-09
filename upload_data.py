import sqlite3

SERVER = 0
RULE = 3
DB = sqlite3.connect(
    '/home/lalo/college/networking/FinalProtocolGraph/protocol_db.db')
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

    for l in line:
        if ((ord(l) >= ord('a') and ord(l) <= ord('z')) or
                (ord(l) >= ord('A') and ord(l) <= ord('Z')) or
                ord(l) == ord(" ") or ord(l) == ord("\n")):
            value = True
        else:
            value = False
            break
    # print "------------------------------------"

    return value


def insert_node_db(word):
    # print "word: ", word
    idd = None

    try:
        cursor.execute(
            '''insert into nodes(name) values(:word)''',
            {'word': word})
        # print "word inserted"
        return True, idd
    except:
        # print "NODE REPEATED!!!"
        nodes = cursor.execute(
            '''select id from nodes where name = :word''', {'word': word}
        )
        for n in nodes:
            idd = n
        # print "idd: ", idd
        return False, idd[0]
    # DB.commit()


def insert_relation_db(node1, node2):
    # print "INSERT_RELATION_DB!!! "
    cursor.execute(
        '''insert into relations(node1, node2) values(:node1, :node2)''',
        {'node1': node1, 'node2': node2}
    )
    # print "relation inserted"
    # DB.commit()


def insert_nodes(words):
    value_ascii = sum_values_ascii(words[0])
    # print "value_ascii", value_ascii
    servers = which_servers(value_ascii)
    # print "servers: ", servers

    for s in servers:
        if SERVER == s:
            answer, id_n = insert_node_db(words[0])
            id_node_1 = cursor.lastrowid if answer else id_n
            # print "WILL INSERT NODE 2!!!!"
            answer, id_n = insert_node_db(words[1])
            id_node_2 = cursor.lastrowid if answer else id_n
            # print "ID_NODE_1: ", id_node_1
            # print "ID_NODE_2: ", id_node_2
            insert_relation_db(id_node_1, id_node_2)


def read_file(file_name):
    with open(file_name) as f:
        # line = f.readline()
        while True:
            line = f.readline()
            if line:
                # print "line before: ", line
                if filter_allowed(line):
                    # continue
                    # print "line after: ", line
                    words = line.split(" ")
                    print "words: ", words
                    insert_nodes(words)
            else:
                break

    DB.commit()


if __name__ == "__main__":
    read_file('/home/lalo/college/networking/FinalProtocolGraph/en.wiki.big')
    # read_file('/home/lalo/college/networking/FinalProtocolGraph/test.txt')
