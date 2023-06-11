**FREE
dcl-f regdspfil workstn sfile(filelist:rrn) infds(dspfds);

dcl-f regf usage(*input) keyed;

dcl-s noofpages packed(2);
dcl-s preno packed(2);
dcl-s pfilnam like(filename);
dcl-s rrn packed(4);
dcl-s size bindec(4);
DCL-DS DSPFDS;
    DSP_FLAG1 CHAR(2) POS(367); // Display flags
    DSP_AID CHAR(1) POS(369); // AID byte
    CURSOR CHAR(2) POS(370); // Cursor location
    DATA_LEN INT(10) POS(372); // Actual data len
    SF_RRN INT(5) POS(376); // Subfile rrn
    MIN_RRN INT(5) POS(378); // Subfile min rrn
    NUM_RCDS INT(5) POS(380); // Subfile num rcds
    ACT_CURS CHAR(2) POS(382); // Active window cursor location
    DSP_MAJOR CHAR(2) POS(401); // Major ret code
    DSP_MINOR CHAR(2) POS(403); // Minor ret code
END-DS;

Begsr *inzsr;
   exsr clrsfl;
   exsr loadsfl;
   exsr dspsfl;
   *inlr = *on;
Endsr;

begsr clrsfl;
    rrn = 0 ;
    drecno = 0 ;
    *in91 = *off;
    *in92 = *off;
    write filelista;
endsr;


Begsr loadsfl;
   //read PF and load subfile
   setll filnam regf;
   read regf;
   dow not %eof(regf);
       d_filen = filename;
       d_filet = filetype;
       rrn = rrn + 1;
       write filelist;
       read regf;
   enddo;
   pfilnam = filnam;
   if rrn > 0;
      *in91 = *on;
      drecno = 1;
      if min_rrn <> 0;
          drecno = min_rrn;
      endif;
   else;
      *in91= *off;
   endif;
Endsr;

Begsr Dspsfl;
   write header;
   write trailer;
   Dow *in03 = *off;
       *in92 = *on;
       exfmt filelista;
       If *in52 = *on;
          *in93 = *on;
       Endif;
       drecno = min_rrn;
       if filnam <> pfilnam ;
           exsr clrsfl;
           exsr loadsfl;
       endif;
   EndDo;
EndSr;

