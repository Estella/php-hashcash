<?php

/**
 * Open a hashcash database
 * 
 * @param string $filename
 * @return resource The database resource
 */
function hashcash_db_open($filename) {}

/**
 * Check if a record is in a hashcash database
 * 
 * @param resource $db
 * @param string $token
 * @param string $period
 * @return boolean If the record exists in the database
 */
function hashcash_db_in($db, $token, $period) {}

/**
 * Add a record to a hashcash database
 * 
 * @param resource $db
 * @param string $token
 * @param string $period
 * @return boolean If the operation succeeds
 */
function hashcash_db_add($db, $token, $period) {}

/**
 * Close a database resource
 * 
 * @param resource $db
 * @return boolean If the operation succeeds
 */
function hashcash_db_close($db) {}

/**
 * Mint a token (simple interface)
 * 
 * @param string $resource
 * @param integer $bits
 * @param integer $anon_period
 * @param string $ext
 * @param integer $compress
 * @return string The token
 */
function hashcash_simple_mint($resource, $bits, $anon_period, $ext, $compress) {}
