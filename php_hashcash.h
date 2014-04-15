
#ifndef PHP_HASHCASH_H
#define PHP_HASHCASH_H

#ifdef __cplusplus
   extern "C" {
#endif

#define PHP_HASHCASH_NAME "hashcash"  
#define PHP_HASHCASH_VERSION "0.1.0"
#define PHP_HASHCASH_RELEASE "2014-04-15"
#define PHP_HASHCASH_BUILD "master"
#define PHP_HASHCASH_AUTHORS "John Boehr <jbboehr@gmail.com> (lead)"

extern zend_module_entry hashcash_module_entry;
#define phpext_hashcash_ptr &hashcash_module_entry


typedef struct _php_hashcash_db {
  void * db;
} php_hashcash_db;

#define PHP_HASHCASH_DB_RES_NAME "Hascash Database"


#ifdef __cplusplus
  } // extern "C" 
#endif

#endif	/* PHP_HASHCASH_H */
