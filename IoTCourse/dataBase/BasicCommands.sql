/* This is a simple SQL tutorial that can be run in class 
Use this online MySQL server: https://onecompiler.com/mysql/43y9qw6qa */

-- 1. Create a table to hold sensor data
CREATE TABLE SensorData (
    id INT AUTO_INCREMENT PRIMARY KEY,   -- unique ID for each row (auto increments)
    sensor_date DATETIME NOT NULL,       -- timestamp of the measurement
    location VARCHAR(50),                -- where the sensor is located
    temperature FLOAT,                   -- temperature in °F
    sensor_number VARCHAR(20),           -- which sensor recorded the data
    humidity FLOAT                       -- humidity in %
);

-- 2. Insert 10 example rows of sensor data
INSERT INTO SensorData (sensor_date, location, temperature, sensor_number, humidity) VALUES
('2025-09-30 10:00:00', 'Lab A', 45.2, 'sensor_1', 55.0),
('2025-09-30 10:05:00', 'Lab A', 47.5, 'sensor_2', 53.0),
('2025-09-30 10:10:00', 'Lab B', 51.1, 'sensor_1', 60.0),
('2025-09-30 10:15:00', 'Lab B', 48.9, 'sensor_3', 59.5),
('2025-09-30 10:20:00', 'Lab C', 62.3, 'sensor_2', 61.2),
('2025-09-30 10:25:00', 'Lab C', 58.0, 'sensor_1', 57.0),
('2025-09-30 10:30:00', 'Lab D', 42.7, 'sensor_4', 50.1),
('2025-09-30 10:35:00', 'Lab D', 65.0, 'sensor_1', 64.0),
('2025-09-30 10:40:00', 'Lab E', 70.2, 'sensor_2', 68.5),
('2025-09-30 10:45:00', 'Lab E', 55.5, 'sensor_3', 54.0);

SELECT *
FROM SensorData
WHERE sensor_number = 'sensor_1';

-- 3. Delete one row (example: remove the row with id = 4)
DELETE FROM SensorData
WHERE id = 4;

-- 4. Search for all rows where temperature is greater than 50°F
SELECT *
FROM SensorData
WHERE temperature > 50;

-- 5 Update location from "Lab A" to "Lab x" for this exact row
UPDATE SensorData
SET location = 'Lab x'
WHERE sensor_number = 'sensor_1'
  AND sensor_date   = '2025-09-30 10:00:00'
ORDER BY id
LIMIT 1;

UPDATE SensorData
SET    location = 'Lab Y'
WHERE  id = 3;               -- or any other unique identifier

SELECT *
FROM SensorData
WHERE sensor_number = 'sensor_1';


