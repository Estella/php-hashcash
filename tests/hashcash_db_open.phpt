--TEST--
hashcash_db_open function
--SKIPIF--
<?php 
if( !extension_loaded('hashcash') ) die('skip ');
 ?>
--FILE--
<?php
$db = hashcash_db_open('db1');
var_dump($db);
?>
--EXPECT--
resource(4) of type (Hascash Database)