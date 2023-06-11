             CMD        PROMPT('SEARCH')
             PARM       KWD(DIR) TYPE(*CHAR) LEN(20) RSTD(*NO) +
                          MIN(1) MAX(5) ALWUNPRT(*YES) ALWVAR(*NO) +
                          CASE(*MIXED) PROMPT('DIR NAME' 1)
             PARM       KWD(FILTYPE) TYPE(*CHAR) LEN(4) RSTD(*NO) +
                          MIN(1) MAX(4) ALWUNPRT(*YES) ALWVAR(*NO) +
                          CASE(*MIXED) PROMPT('FILE EXT' 2)
             PARM       KWD(FILPATT) TYPE(*CHAR) LEN(65) RSTD(*NO) +
                          MIN(1) MAX(65) ALWUNPRT(*YES) ALWVAR(*NO) +
                          CASE(*MIXED) PROMPT('FILENAME PATTERN' 3)
