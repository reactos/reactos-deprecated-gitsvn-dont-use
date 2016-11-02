/*
 *  GOTO.C - goto internal batch command.
 *
 *  History:
 *
 *    16 Jul 1998 (Hans B Pufal)
 *        started.
 *
 *    16 Jul 1998 (John P Price)
 *        Separated commands into individual files.
 *
 *    27-Jul-1998 (John P Price <linux-guru@gcfl.net>)
 *        added config.h include
 *
 *    28 Jul 1998 (Hans B Pufal) [HBP_003]
 *        Terminate label on first space character, use only first 8 chars of
 *        label string
 *
 *    24-Jan-1999 (Eric Kohl)
 *        Unicode and redirection safe!
 *
 *    27-Jan-1999 (Eric Kohl)
 *        Added help text ("/?").
 *
 *    28-Apr-2005 (Magnus Olsen <magnus@greatlord.com>)
 *        Remove all hardcoded strings in En.rc
 */

#include "precomp.h"


/*
 * Perform GOTO command.
 *
 * Only valid if batch file current.
 *
 */

INT cmd_goto (LPTSTR param)
{
    LPTSTR tmp, tmp2;

    TRACE ("cmd_goto (\'%s\')\n", debugstr_aw(param));

    if (!_tcsncmp (param, _T("/?"), 2))
    {
        ConOutResPaging(TRUE,STRING_GOTO_HELP1);
        return 0;
    }

    /* if not in batch -- error!! */
    if (bc == NULL)
    {
        return 1;
    }

    if (*param == _T('\0'))
    {
        ConErrResPrintf(STRING_GOTO_ERROR1);
        ExitBatch();
        return 1;
    }

    /* terminate label at first space char */
    tmp = param+1;
    while (!_istcntrl (*tmp) && !_istspace (*tmp) &&  (*tmp != _T(':')))
        tmp++;
    *(tmp) = _T('\0');

    /* jump to end of the file */
    if ( _tcsicmp( param, _T(":eof"))==0)
    {
        bc->mempos=bc->memsize;		/* position at the end of the batchfile */
        return 0;
    }

    /* jump to begin of the file */
    bc->mempos=0;

    while (BatchGetString (textline, sizeof(textline) / sizeof(textline[0])))
    {
        int pos;
        INT_PTR size;

        /* Strip out any trailing spaces or control chars */
        tmp = textline + _tcslen (textline) - 1;

        while (tmp > textline && (_istcntrl (*tmp) || _istspace (*tmp) ||  (*tmp == _T(':'))))
            tmp--;
        *(tmp + 1) = _T('\0');

        /* Then leading spaces... */
        tmp = textline;
        while (_istspace (*tmp))
            tmp++;

        /* All space after leading space terminate the string */
        size = _tcslen(tmp) -1;
        pos=0;
        while (tmp+pos < tmp+size)
        {
            if (_istspace(tmp[pos]))
                tmp[pos]=_T('\0');
            pos++;
        }

        tmp2 = param;
        /* use whole label name */
        if ((*tmp == _T(':')) && ((_tcsicmp (++tmp, param) == 0) || (_tcsicmp (tmp, ++tmp2) == 0)))
            return 0;

    }

    ConErrResPrintf(STRING_GOTO_ERROR2, param);
    ExitBatch();
    return 1;
}

/* EOF */
