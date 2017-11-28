PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE nodes(id integer primary key autoincrement, name text not null);
CREATE TABLE edges(id integer primary key autoincrement, node1 integer not null, node2 integer not null);
CREATE TABLE attributes(id integer primary key autoincrement, node_id integer not null, name_attribute text not null, value text not null);
DELETE FROM sqlite_sequence;
COMMIT;
