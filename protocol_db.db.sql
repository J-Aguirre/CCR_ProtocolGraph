BEGIN TRANSACTION;
CREATE TABLE IF NOT EXISTS `relations` (
	`id`	integer PRIMARY KEY AUTOINCREMENT,
	`node1`	integer NOT NULL,
	`node2`	integer NOT NULL,
	FOREIGN KEY(`node1`) REFERENCES `nodes`(`id`),
	FOREIGN KEY(`node2`) REFERENCES `nodes`(`id`)
);
CREATE TABLE IF NOT EXISTS `nodes` (
	`id`	integer PRIMARY KEY AUTOINCREMENT,
	`name`	text NOT NULL UNIQUE
);
CREATE TABLE IF NOT EXISTS `attributes` (
	`id`	integer PRIMARY KEY AUTOINCREMENT,
	`node_id`	integer NOT NULL,
	`name_attribute`	text NOT NULL,
	`value`	text NOT NULL,
	FOREIGN KEY(`node_id`) REFERENCES `nodes`(`id`)
);
COMMIT;
