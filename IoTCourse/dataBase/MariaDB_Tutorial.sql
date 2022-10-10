NOTES: 
--| Click on SQL - make sure your screen setting is he same as below (pay attention to check marks):
--| Make sure the Retain Query Box and Enable Foreign Key are checked. 
--| In the editor type the following and press GO. Do one mySQL command at a time 

-- Create a table
CREATE TABLE Book(  
id INT NOT NULL AUTO_INCREMENT,  
name VARCHAR(100) NOT NULL,  
PRIMARY KEY (id)); 

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
ALTER TABLE `Book` 
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

  
