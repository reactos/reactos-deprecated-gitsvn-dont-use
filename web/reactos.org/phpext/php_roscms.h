#ifndef PHP_ROSCMS_H
#define PHP_ROSCMS_H

extern zend_module_entry roscms_module_entry;
#define phpext_roscms_ptr &roscms_module_entry

#ifdef PHP_WIN32
#define PHP_ROSCMS_API __declspec(dllexport)
#else
#define PHP_ROSCMS_API
#endif

#ifdef ZTS
#include "TSRM.h"
#endif

PHP_MINFO_FUNCTION(roscms);

PHP_FUNCTION(roscms_unicode_escape);

#endif	/* PHP_ROSCMS_H */
