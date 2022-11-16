NOTES: 
--| Theses are sample database command to generate a quick table for your sensor data. 
--| We are assuming the table has the following columns: 
--| Node; arrivalTime; Hum, Temp

--| Link to this page: https://github.com/faridfarahmand/SSU/blob/main/IoTCourse/dataBase/SampleSQLSensorData.sql.sql 

SELECT * FROM student_database_1.Farid_Farahmand_SensordB;
SHOW TABLES;
SET FOREIGN_KEY_CHECKS=0; -- need it to drop Table in case there is a foreignkey
DROP TABLE Farid_Farahmand_SensordB;

ALTER TABLE student_database_1.Farid_Farahmand_SensordB 
  DROP `NodeId`,
  DROP `ArrivalTime`,
  DROP `Temp`;
  

SHOW DATABASES;
SHOW SCHEMAS;

SELECT student_database_1 ;
SHOW TABLES FROM student_database ;
SHOW COLUMNS FROM test_table_1; 

CREATE TABLE Farid_Farahmand_ExtdB ( 
arrivalTime TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, // UTC time
nodeId VARCHAR(6) NOT NULL,
Temp INT NOT NULL,
temp DECIMAL(6,4) NOT NULL; DECIMAL(M, D) where M is the maximum number of digits (the precision) and D is the number of digits
Hum INT NOT NULL); 

-- Insert data
INSERT INTO Farid_Farahmand_ExtdB (nodeId, arrivalTime, Temp, Hum) VALUES ('node_1','2022-10-01 10:15:01', 12, 89); -- specify time
INSERT INTO Farid_Farahmand_ExtdB (nodeId, Temp, Hum) VALUES ('node_1', 12, 89); -- Default UTC time
INSERT INTO Farid_Farahmand_ExtdB (nodeId, Temp, Hum) VALUES ('node_1', 12, 89); -- Default UTC time
INSERT INTO Farid_Farahmand_ExtdB (nodeId, arrivalTime, Temp, Hum) -- time is specified
	VALUES 	('node_1', '2022-10-01 10:15:01', 22, 89),
			('node_1', '2022-10-01 11:15:01', 32, 56),
			('node_1', '2022-10-01 12:15:01', 22, 23),
			('node_1', '2022-10-01 13:15:01', 52, 87),
			('node_1', '2022-10-01 14:15:01', 62, 12),
			('node_1', '2022-10-01 15:15:01', 72, 92),
			('node_1', '2022-10-01 16:15:01', 32, 17);
            
SELECT * FROM Farid_Farahmand_ExtdB;
SET SQL_SAFE_UPDATES = 0; -- set safe mode 
DELETE FROM Farid_Farahmand_ExtdB WHERE arrivalTime <= (NOW() + INTERVAL 1 DAY); -- delete records next (+)/last(-) 24 hours
DELETE FROM Farid_Farahmand_ExtdB WHERE arrivalTime <= (NOW() - INTERVAL 24 HOUR); -- delete records next (+)/last(-) 24 hours
