
# HASHCASH -------------------------------------------------------------------
PHP_ARG_ENABLE(hashcash, whether to enable hashcash support,
# Make sure that the comment is aligned:
[  --enable-hashcash Enable hashcash support])

# MAIN -------------------------------------------------------------------------
if test "$PHP_HASHCASH" != "no"; then
	PHP_ADD_LIBRARY(hashcash, 1, HASHCASH_SHARED_LIBADD)
	PHP_NEW_EXTENSION(hashcash, hashcash.c, $ext_shared, , $PHP_HASHCASH_FLAGS)
	PHP_SUBST(HASHCASH_SHARED_LIBADD)
fi

