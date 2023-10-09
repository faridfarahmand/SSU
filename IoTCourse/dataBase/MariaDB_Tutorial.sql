NOTES: 
--| Click on SQL
--| Make sure the Retain Query Box and Enable Foreign Key are checked. 
--| In the editor type the following and press GO. Do one mySQL command at a time 
--| Link to this page: https://github.com/faridfarahmand/SSU/blob/main/IoTCourse/dataBase/MariaDB_Tutorial.sql 

-- Explore the databases
SHOW DATABASES;

-- Use the appropriate database
USE id19686307_myfirstdbase;

-- Example of table with constraint
-- NOTE: A MySQL database can have tables that do not have a primary key (such as the case below), 
--      but it's generally not recommended.
CREATE TABLE t2 (name VARCHAR(30) CHECK (CHAR_LENGTH(name)>2), start_date DATE, 
  end_date DATE CHECK (start_date IS NULL OR end_date IS NULL OR start_date<end_date));
INSERT INTO t2(name, start_date, end_date) VALUES('Ione', '2003-12-15', '2014-11-09');
INSERT INTO t2(name, start_date, end_date) VALUES('Io', '2003-12-15', '2014-11-09'); -- will fail the constraint
INSERT INTO t2(name, start_date, end_date) VALUES('Ione', NULL, '2014-11-09');
INSERT INTO t2(name, start_date, end_date) VALUES('Ione', '2015-12-15', '2014-11-09'); -- will fail the constraint
-------------------

-- Example of auto_increment / auto_increment is defined differently in other databases
-- AUTO_INCLREMENT is used to automaically increment the product_id every time there is an insert
CREATE TABLE product (
  product_id INT AUTO_INCREMENT PRIMARY KEY,
  product_name VARCHAR(200),
  price INT
);
INSERT INTO product (product_name, price)
VALUES ('Desk chair', 50);
-------------------


-- Create a table
CREATE TABLE Book(  
id INT NOT NULL AUTO_INCREMENT,  
name VARCHAR(100) NOT NULL,  
PRIMARY KEY (id)); 

-- NOTE: A null value in a relational database is used when the value in a column 
-- is unknown or missing. A null is neither an empty string (for character or 
-- datetime data types) nor a zero value (for numeric data types).

-- NOTE: Primary keys are a central element in SQL databases. They provide a unique value 
-- that can identify a specific row in a table, which opens up a lot of other benefits.

SHOW TABLES; -- see available tables 
DESC Book; -- to see the structure of the table 

INSERT INTO Book  
(id, name) 
VALUES(1, 'MariaDB Book'); -- values for each column

INSERT INTO Book  (id, name) 
VALUES(2, 'SQLDB Book'); -- values for each column

SELECT * from Book; -- check the entries in your table
 
-- -- remove the table completely sometimes you need to remove the columns first
DROP TABLE ` Book ` ;  -- you may have to retype this; don't copy and paste

-- Remove the columns
ALTER TABLE Book 
  DROP `id`,
  DROP `name`;

-- Create a table
CREATE TABLE Book(  
id INT NOT NULL AUTO_INCREMENT,  
name VARCHAR(100) NOT NULL,  
PRIMARY KEY (id)); 

CREATE TABLE Price(  
id INT NOT NULL AUTO_INCREMENT,  
price float NOT NULL,  
PRIMARY KEY (id)); -- primary key is the id

-- Insert books 
INSERT INTO Book -- insert multiple records
(id, name)  
VALUES  
(1,'MariaDB Book1'), 
(2,'MariaDB Book2'),  
(3,'MariaDB Book3'),  
(4,'MariaDB Book4'),  
(5,'MariaDB Book5');  

-- insert associated prices - pay attention to upper/lower cases
INSERT INTO Price 
(id, price)  
VALUES
(1, 251),   
(2, 252),  
(3, 223),  
(4, 194),  
(5, 305);  


SELECT * from Book; -- check the entries in your table

-- Your OUTPUT    
name   	id	
MariaDB Book1a	1	
MariaDB Book2	2	
MariaDB Book3	3	
MariaDB Book4	4	
MariaDB Book5	5	

SELECT * from Price; -- check the entries in your table
-- YOUT OUTPUT 
price id 
250 1
252 2
223 3
194 4
305 5
251 6
                                                                  
--- Update/change the price
UPDATE Price
SET price = 250
WHERE id = 1;

-- Let us change the name of the book named Book to MariaDB Book1a. Notice that the book has an id of 1. Here is the command for this:
UPDATE Book
SET name = 'MariaDB Book1a'
WHERE id = 1;

-- Insert a new price not associated with a book! 
INSERT INTO Price 
(id, price)  
VALUES
(1, 251);

-- We need to delete the last record from the table. 
DELETE FROM Price
WHERE id = 6; 

-- Show only the prices less than 250
SELECT *   
FROM Price 
WHERE price < 250; 

SELECT *  
FROM Price
WHERE id > 3  
OR price < 250;

-- To see all names that end with 4, you can run the following command:
SELECT name  
FROM Book  
WHERE name LIKE '%4'; 

-- To see all names that start with 2, you can run the following command
-- order the results by price low to high
SELECT * FROM Price
WHERE price LIKE '2%'
ORDER BY price;  

-- The goal is to join the name column from the Book table and the price column from Price table 
-- into a single table. This is possible with an inner join, as demonstrated below:
SELECT Book.name, Price.price 
FROM Book   
INNER JOIN Price 
ON Book.id = Price.id;

-- Creating a VIEW; 
-- VIEW is a virtual representatin of several tables together. Let's create a VIEW of Book and Price tables
-- First let's modify our tables
ALTER TABLE websites
  DROP PRIMARY KEY; 
  
-- Create a table
CREATE TABLE Book2(  
id INT NOT NULL AUTO_INCREMENT,  
name VARCHAR(100) NOT NULL,  
PRIMARY KEY (id)); 
    
CREATE TABLE Price2(  
id INT NOT NULL,
price float NOT NULL, 
Foreign Key (id) REFERENCES Book2(id));
 
-- Insert books 
INSERT INTO Book2 -- insert multiple records
(id, name)  
VALUES  
(1,'MariaDB Book1'), 
(2,'MariaDB Book2'),  
(3,'MariaDB Book3'),  
(4,'MariaDB Book4'),  
(5,'MariaDB Book5');  

INSERT INTO Book2 -- You must first register the Book and then give it a price
(id, name)  
VALUES  
(6,'MariaDB Book6'); 

-- insert associated prices - pay attention to upper/lower cases
INSERT INTO Price2 
(id, price)  
VALUES
(6, 251);

-- Creating a VEIW of all tables
CREATE VIEW COMBINED AS SELECT
    Book2.name,
    Book2.id,
    Price2.price
FROM
    Book2,
    Price2
WHERE
    Book2.id = Price2.id; 
    
SELECT * FROM COMBINED;

DROP VIEW IF EXISTS
    COMBINED;
