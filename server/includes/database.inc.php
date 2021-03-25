<?php

  $db_servername = "IP OF THE SQL SERVER";
  $db_username = "USERNAME";
  $db_password = "PASSWORD";
  $db_name = "NAME OF THE DATABASE ON THE SQL SERVER";

  $conn = mysqli_connect($db_servername, $db_username, $db_password, $db_name);
/*
  if (! $conn) {
    echo "Error: Unable to connect to MySQL." . PHP_EOL . "<br>";
    echo "Debugging errno: " . mysqli_connect_errno() . PHP_EOL . "<br>";
    echo "Debugging error: " . mysqli_connect_error() . PHP_EOL . "<br>";

  }
  else {
    echo "Connected sucessfully";
  }*/
?>
