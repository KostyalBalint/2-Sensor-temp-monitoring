<?php

// Create connection
//$conn = new mysqli($db_servername, $db_username, $db_password, $db_name);
include_once 'includes/database.inc.php';
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$temp_1 = $_GET['temp_1'];
$temp_2 = $_GET['temp_2'];
$voltage = $_GET['voltage'];

$sql = "INSERT INTO temp_data (date, temp_1, temp_2, voltage)
VALUES (CURRENT_TIMESTAMP(), $temp_1, $temp_2, $voltage)";

if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}

$conn->close();
?>
