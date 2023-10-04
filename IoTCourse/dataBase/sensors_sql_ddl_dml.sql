-- sensors.sensors definition

CREATE TABLE sensors (
  node varchar(20),
  time_receive datetime,
  temperature int(11),
  humidity int(11)
);

-- insert data
INSERT INTO sensors (node,time_receive,temperature,humidity) VALUES
	 ('node-2','2023-10-04 10:05:35',82,15),
	 ('node-3','2023-10-04 10:06:29',93,85),
	 ('node-1','2023-10-04 10:05:34',84,25),
	 ('node-1','2023-10-03 13:57:19',84,25),
	 ('node-2','2023-10-03 10:05:35',82,15),
	 ('node-3','2023-10-03 10:06:29',93,85),
	 ('node-1','2023-10-03 10:05:34',84,25),
	 ('node-1','2023-10-03 13:58:34',84,25),
	 ('node-2','2023-10-03 13:58:34',87,28);
INSERT INTO sensors (node,time_receive,temperature,humidity) VALUES
	 ('node-2','2023-10-03 13:58:34',74,21),
	 ('node-2','2023-10-03 10:08:31',72,10),
	 ('node-3','2023-10-03 10:09:09',73,8),
	 ('node-1','2023-10-03 10:02:18',74,9);
	 
-- DML commands
select * from sensors;

update sensors set humidity=30 where node='node-1';

select * from sensors;

delete from sensors where node='node-4';

select * from sensors;

select max(temperature) from sensors;

select min(humidity) from sensors;

select * from sensors;

select avg(temperature) from sensors;

select max(humidity) from sensors;

select min(time_receive) from sensors;

select node,
       avg(temperature),
       avg(humidity)
  from sensors
  group by node;
	 
