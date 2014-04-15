--TEST--
hashcash_simple_mint function
--SKIPIF--
<?php 
if( !extension_loaded('hashcash') ) die('skip ');
 ?>
--FILE--
<?php
$stamp = hashcash_simple_mint("hashcash", 0, 0, "", 0);
var_dump($stamp);
?>
--EXPECTREGEX--
string\(52\) \"1:0:.+:hashcash::.+:.+\"