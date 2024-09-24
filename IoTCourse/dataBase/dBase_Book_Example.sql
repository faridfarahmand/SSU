-- This example demonstrates how to create multiple database tables,
-- each with its own PRIMARY KEY, and connect them using FOREIGN KEYS.
-- We will use JOIN to list the related entries from the connected tables.

-- Create Authors Table
CREATE TABLE Authors (
    author_id INT AUTO_INCREMENT PRIMARY KEY, -- Primary key
    name VARCHAR(100) NOT NULL,
    country VARCHAR(50)
);

-- Create Publishers Table
CREATE TABLE Publishers (
    publisher_id INT AUTO_INCREMENT PRIMARY KEY, -- Primary key
    name VARCHAR(100) NOT NULL,
    address VARCHAR(255)
);

-- Create Books Table
CREATE TABLE Books (
    book_id INT AUTO_INCREMENT PRIMARY KEY, -- Primary key
    title VARCHAR(255) NOT NULL,
    author_id INT, -- Foreign key referencing Authors
    publisher_id INT, -- Foreign key referencing Publishers
    published_year YEAR,
    genre VARCHAR(50),
    price DECIMAL(10, 2),
    -- Define Foreign Key constraints
    FOREIGN KEY (author_id) REFERENCES Authors(author_id),
    FOREIGN KEY (publisher_id) REFERENCES Publishers(publisher_id)
);

INSERT INTO Authors (name, country)
VALUES 
('George Orwell', 'United Kingdom'),
('J.K. Rowling', 'United Kingdom'),
('Haruki Murakami', 'Japan');

INSERT INTO Publishers (name, address)
VALUES 
('Penguin Books', '375 Hudson Street, New York, NY'),
('Bloomsbury Publishing', '50 Bedford Square, London, UK'),
('Shinchosha Publishing', 'Shinjuku, Tokyo, Japan');

INSERT INTO Books (title, author_id, publisher_id, published_year, genre, price)
VALUES
('1984', 1, 1, 1949, 'Dystopian', 19.99),
('Harry Potter and the Philosopher\'s Stone', 2, 2, 1997, 'Fantasy', 25.99),
('Kafka on the Shore', 3, 3, 2002, 'Magical Realism', 15.99);


-- Show all records in the Authors table
SELECT * FROM Authors;

-- Show all records in the Publishers table
SELECT * FROM Publishers;

-- Show all records in the Books table
SELECT * FROM Books;

-- Insert a book into the Books table with a valid foreign key (author_id)
INSERT INTO Books (title, author_id, publisher_id, published_year, genre, price)
VALUES ('The Lord of the Rings', 4, 1, 1954, 'Fantasy', 29.99);

SELECT Books.title, Authors.name, Publishers.name
FROM Books
JOIN Authors ON Books.author_id = Authors.author_id
JOIN Publishers ON Books.publisher_id = Publishers.publisher_id;


