create database if not exists universe;

create table if not exists actor_db(
    actor_id bigint not null,
	name varchar(32),
	game_data_size int,
	game_data_blob  BLOB,
	primary key(actor_id)
);
