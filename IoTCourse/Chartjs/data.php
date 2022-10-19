<?php
//setting header to json
header('Content-Type: application/json');

//database
define('DB_HOST', 'localhost');
define('DB_USERNAME', 'id108_db_faridfarahmand');
define('DB_PASSWORD', 'Ssu234');
define('DB_NAME', 'id1908_sensor');

//get connection
$mysqli = new mysqli(DB_HOST, DB_USERNAME, DB_PASSWORD, DB_NAME);

if(!$mysqli){
  die("Connection failed: " . $mysqli->error);
}

//query to get data from the table
$query = sprintf("SELECT id, temperature, time_received FROM tempSensor");

//execute query
$result = $mysqli->query($query);

//loop through the returned data
$data = array();
foreach ($result as $row) {
  $data[] = $row;
}

//close connection
$mysqli->close();

//now print the data
print json_encode($data);
