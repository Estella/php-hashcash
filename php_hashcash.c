
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "php.h"
#include "php_ini.h"
#include "main/SAPI.h"
#include "Zend/zend_API.h"
#include "Zend/zend_alloc.h"
#include "ext/standard/info.h"
#include "ext/standard/php_string.h"

#include "sdb.h"
#include "hashcash.h"

#include "php_hashcash.h"

/* {{{ Globals --------------------------------------------------------------- */

int le_hashcash_db;
int hashcash_last_error;
int hashcash_module_number;

/* }}} ---------------------------------------------------------------------- */
/* {{{ Functions ------------------------------------------------------------ */

ZEND_BEGIN_ARG_INFO_EX(hashcash_db_open_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
    ZEND_ARG_INFO(0, filename)
ZEND_END_ARG_INFO()

PHP_FUNCTION(hashcash_db_open)
{
  // Declarations
  php_hashcash_db * db_res;
  DB * db;
  int err = 0;
  
  char * filename = NULL;
  int filename_len = 0;
  
  // Parameters
  if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s", &filename, 
          &filename_len) == FAILURE) {
      RETURN_NULL();
  }
  
  // Allocate db
  db = (DB *) malloc(sizeof(DB));
  if( !db ) {
    RETURN_FALSE;
  }
  
  int ret = hashcash_db_open(db, (const char *) filename, &err);
  if( !ret ) {
    hashcash_last_error = err;
    RETURN_FALSE;
  }
  
  db_res = (php_hashcash_db *) emalloc(sizeof(php_hashcash_db));
  memset(db_res, 0, sizeof(php_hashcash_db));
  db_res->db = db;

  ZEND_REGISTER_RESOURCE(return_value, db_res, le_hashcash_db);
}


ZEND_BEGIN_ARG_INFO_EX(hashcash_db_in_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
    ZEND_ARG_INFO(0, db)
    ZEND_ARG_INFO(0, token)
    ZEND_ARG_INFO(0, period)
ZEND_END_ARG_INFO()

PHP_FUNCTION(hashcash_db_in)
{
  // Declarations
  php_hashcash_db * db_res;
  DB * db;
  int err = 0;
  
  zval * zdb;
  char * token = NULL;
  int token_len = 0;
  char * period = NULL;
  int period_len = 0;
  
  // Parameters
  if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rss", &zdb, 
          &token, &token_len, &period, &period_len) == FAILURE ) {
    RETURN_FALSE;
  }
  
  // Fetch resource
  ZEND_FETCH_RESOURCE(db_res, php_hashcash_db*, &zdb, -1, 
          PHP_HASHCASH_DB_RES_NAME, le_hashcash_db);
  db = (DB *) db_res->db;
  
  // Check in database
  int ret = hashcash_db_in(db, token, period, &err);
  if( err ) {
    hashcash_last_error = err;
    RETURN_FALSE;
  }
  
  if( ret ) {
    RETURN_TRUE;
  } else {
    RETURN_FALSE;
  }
}


ZEND_BEGIN_ARG_INFO_EX(hashcash_db_add_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
    ZEND_ARG_INFO(0, db)
    ZEND_ARG_INFO(0, token)
    ZEND_ARG_INFO(0, period)
ZEND_END_ARG_INFO()

PHP_FUNCTION(hashcash_db_add)
{
  // Declarations
  php_hashcash_db * db_res;
  DB * db;
  int err = 0;
  
  zval * zdb;
  char * token = NULL;
  int token_len = 0;
  char * period = NULL;
  int period_len = 0;
  
  // Parameters
  if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "rss", &zdb, 
          &token, &token_len, &period, &period_len) == FAILURE ) {
    RETURN_FALSE;
  }
  
  // Fetch resource
  ZEND_FETCH_RESOURCE(db_res, php_hashcash_db*, &zdb, -1, 
          PHP_HASHCASH_DB_RES_NAME, le_hashcash_db);
  db = (DB *) db_res->db;
  
  // Add to database
  int ret = hashcash_db_add(db, token, period, &err);
  if( !ret ) {
    hashcash_last_error = err;
    RETURN_FALSE;
  }
  
  RETURN_TRUE;
}


ZEND_BEGIN_ARG_INFO_EX(hashcash_db_close_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
    ZEND_ARG_INFO(0, db)
ZEND_END_ARG_INFO()

PHP_FUNCTION(hashcash_db_close)
{
  // Declarations
  php_hashcash_db * db_res;
  DB * db;
  int err = 0;
  
  zval * zdb;
  
  // Parameters
  if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "r", &zdb) == FAILURE ) {
    RETURN_FALSE;
  }
  
  // Fetch resource
  ZEND_FETCH_RESOURCE(db_res, php_hashcash_db*, &zdb, -1, 
          PHP_HASHCASH_DB_RES_NAME, le_hashcash_db);
  db = (DB *) db_res->db;
  
  // Close database
  if( !hashcash_db_close(db, &err) ) {
    hashcash_last_error = err;
    RETURN_FALSE;
  }
  
  zend_list_delete(Z_LVAL_P(zdb));
  RETURN_TRUE;
}


ZEND_BEGIN_ARG_INFO_EX(hashcash_simple_mint_args, ZEND_SEND_BY_VAL, ZEND_RETURN_VALUE, 0)
    ZEND_ARG_INFO(0, resource)
    ZEND_ARG_INFO(0, bits)
    ZEND_ARG_INFO(0, anon_period)
    ZEND_ARG_INFO(0, ext)
    ZEND_ARG_INFO(0, compress)
ZEND_END_ARG_INFO()

PHP_FUNCTION(hashcash_simple_mint)
{
  char * resource;
  long resource_len;
  long bits;
  long anon_period;
  char * ext;
  long ext_len;
  long compress;
  char * stamp;
  
  // Parameters
  if( zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "sllsl", 
          &resource, &resource_len, &bits, &anon_period, &ext, &ext_len,
          &compress) == FAILURE ) {
    RETURN_FALSE;
  }
  
  // Mint
  const char * r = (const char *) resource;
  unsigned b = (unsigned) bits;
  stamp = hashcash_simple_mint(r, b, anon_period, ext, compress);
  
  if( !stamp ) {
    RETURN_FALSE;
  }
  
  char * estamp = estrdup(stamp);
  free(stamp);
  
  RETURN_STRING(estamp, 0);
}

/* }}} ---------------------------------------------------------------------- */
/* {{{ Module Hooks --------------------------------------------------------- */

static PHP_MINIT_FUNCTION(hashcash)
{
  hashcash_module_number = module_number;
  le_hashcash_db = zend_register_list_destructors_ex(NULL, NULL, 
          PHP_HASHCASH_DB_RES_NAME, hashcash_module_number);
  return SUCCESS;
}

static PHP_MINFO_FUNCTION(hashcash)
{
  php_info_print_table_start();
  php_info_print_table_row(2, "Version", PHP_HASHCASH_VERSION);
  php_info_print_table_row(2, "Released", PHP_HASHCASH_RELEASE);
  php_info_print_table_row(2, "Authors", PHP_HASHCASH_AUTHORS);
  php_info_print_table_row(2, "Hashcash version", HASHCASH_VERSION);
  php_info_print_table_row(2, "Hashcash format version", HASHCASH_FORMAT_VERSION);
  php_info_print_table_end();
}

/* }}} ---------------------------------------------------------------------- */
/* {{{ Function Entry ------------------------------------------------------- */

zend_function_entry hashcash_functions[] = {
	PHP_FE(hashcash_db_open, hashcash_db_open_args)
	PHP_FE(hashcash_db_in, hashcash_db_in_args)
	PHP_FE(hashcash_db_add, hashcash_db_add_args)
	PHP_FE(hashcash_db_close, hashcash_db_close_args)
  PHP_FE(hashcash_simple_mint, hashcash_simple_mint_args)
};

/* }}} ---------------------------------------------------------------------- */
/* {{{ Module Entry --------------------------------------------------------- */

zend_module_entry hashcash_module_entry = {
  STANDARD_MODULE_HEADER,
	PHP_HASHCASH_NAME,              /* Name */
	hashcash_functions,             /* Functions */
	PHP_MINIT(hashcash),            /* MINIT */
	NULL,                           /* MSHUTDOWN */
	NULL,                           /* RINIT */
	NULL,                           /* RSHUTDOWN */
	PHP_MINFO(hashcash),            /* MINFO */
	PHP_HASHCASH_VERSION,           /* Version */
  STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_HASHCASH
  ZEND_GET_MODULE(hashcash)      // Common for all PHP extensions which are build as shared modules  
#endif

/* }}} ---------------------------------------------------------------------- */
