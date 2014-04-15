--TEST--
hashcash_db_close function
--SKIPIF--
<?php 
if( !extension_loaded('hashcash') ) die('skip ');
 ?>
--FILE--
<?php
$db = hashcash_db_open('db1');
$ret = hashcash_db_close($db);
var_dump($ret);
?>
--EXPECT--
bool(true)