PRAGMA foreign_keys=OFF;
BEGIN TRANSACTION;
CREATE TABLE nodes(id integer primary key autoincrement, name text not null);
INSERT INTO nodes VALUES(1,'Peru');
CREATE TABLE relations(id integer primary key autoincrement, node1 integer not null, node2 integer not null, foreign key(node1) references nodes(id),foreign key(node2) references nodes(id));
CREATE TABLE attributes(id integer primary key autoincrement, node_id integer not null, name_attribute text not null, value text not null, foreign key(node_id) references nodes(id));
DELETE FROM sqlite_sequence;
INSERT INTO sqlite_sequence VALUES('nodes',1);
COMMIT;
