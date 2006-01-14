#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "php.h"
#include "php_ini.h"
#include "ext/standard/info.h"
#include "php_roscms.h"

/*
 * Every user visible function must have an entry in roscms_functions[].
 */
function_entry roscms_functions[] = {
	PHP_FE(roscms_unicode_escape,NULL)
	{NULL, NULL, NULL}	/* Must be the last line in roscms_functions[] */
};

zend_module_entry roscms_module_entry = {
#if ZEND_MODULE_API_NO >= 20010901
	STANDARD_MODULE_HEADER,
#endif
	"roscms",
	roscms_functions,
	NULL,
	NULL,
	NULL,
	NULL,
	PHP_MINFO(roscms),
#if ZEND_MODULE_API_NO >= 20010901
	"1.0", /* Replace with version number for your extension */
#endif
	STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_ROSCMS
ZEND_GET_MODULE(roscms)
#endif

PHP_MINFO_FUNCTION(roscms)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "roscms support", "enabled");
	php_info_print_table_end();
}
/* {{{ proto string roscms_unicode_escape(string arg)
   Return a string in which characters > 127 have been HTML-escaped */
PHP_FUNCTION(roscms_unicode_escape)
{
  unsigned char *in, *out, *pi, *po;
  unsigned in_len, out_len;
  unsigned i;
  unsigned val, len;
  int has_high;
  char msg[128];
  unsigned size;

  if (FAILURE == zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "s",
                                       &in, &in_len))
    {
      RETURN_NULL();
    }

  /* We make a first pass over the input string. If there are high chars,
     we use this pass to compute the total amount of mem required. If there
     aren't any high chars (will probably happen a lot), we can skip the
     conversion */
  size = 0;
  i = 0;
  pi = in;
  has_high = 0;
  while (i < in_len)
    {
      if (*pi <= 127)
        {
          size++;
          i++;
          pi++;
        }
      else
        {
          if (192 <= *pi && *pi < 224)
            {
              i += 2;
              pi += 2;
            }
          else if (224 <= *pi && *pi < 240)
            {
              i += 3;
              pi += 3;
            }
          else if (240 <= *pi && *pi < 248)
            {
              i += 4;
              pi += 4;
            }
          else if (248 <= *pi && *pi < 252)
            {
              i += 5;
              pi += 5;
            }
          else if (252 <= *pi && *pi < 254)
            {
              i += 6;
              pi += 6;
            }
          else
            {
              RETURN_NULL();
            }
          has_high = 1;
          size += 8; /* &#65535; */
        }
    }

  /* Now check if any high chars were found. If not, just return the input */
  if (! has_high) 
    {
      RETURN_STRINGL((char *) in, in_len, 1);
    }

  size++;
  out = emalloc(size);
  if (NULL == out)
    {
      RETURN_NULL();
    }
  
  i = 0;
  pi = in;
  out_len = 0;
  po = out;
  while (i < in_len)
    {
      if (*pi <= 127)
        {
          i++;
          *po++ = *pi++;
          out_len++;
        }
      else
        {
          if (192 <= *pi && *pi < 224)
            {
              val = (pi[0] - 192) * 64 +
                    (pi[1] - 128);
              i += 2;
              pi += 2;
            }
          else if (224 <= *pi && *pi < 240)
            {
              val = (pi[0] - 224) * 4096 +
                    (pi[1] - 128) * 64 +
                    (pi[2] - 128);
              i += 3;
              pi += 3;
            }
          else if (240 <= *pi && *pi < 248)
            {
              val = (pi[0] - 240) * 262144 +
                    (pi[1] - 128) * 4096 +
                    (pi[2] - 128) * 64 +
                    (pi[3] - 128);
              i += 4;
              pi += 4;
            }
          else if (248 <= *pi && *pi < 252)
            {
              val = (pi[0] - 248) * 16777216 +
                    (pi[1] - 128) * 262144 +
                    (pi[2] - 128) * 4096 +
                    (pi[3] - 128) * 64 +
                    (pi[4] - 128);
              i += 5;
              pi += 5;
            }
          else if (252 <= *pi && *pi < 254)
            {
              val = (pi[0] - 252) * 1073741824 +
                    (pi[1] - 128) * 16777216 +
                    (pi[2] - 128) * 262144 +
                    (pi[3] - 128) * 4096 +
                    (pi[4] - 128) * 64 +
                    (pi[5] - 128);
              i += 6;
              pi += 6;
            }
          else
            {
              RETURN_NULL();
            }
          php_sprintf((char *) po, "&#%u;", val);
          out_len += strlen((char *)po);
          po = out + out_len;
        }
    }
  *po = '\0';

  if (size < out_len + 1)
    {
      sprintf(msg, "roscms_unicode_escape: buffer overflow size %u needed %d",
              size, out_len);
      zend_error(E_ERROR, msg);
    }

  RETURN_STRINGL((char *) out, out_len, 0);
}
