#ifndef __POINTER__LIST_
#define __POINTER__LIST_


typedef struct info_node  * info_deikti;

typedef struct typos_komvou * typos_deikti;


info_deikti LIST_dimiourgia();

void LIST_katastrofi(info_deikti * linfo);

int LIST_keni(const info_deikti  linfo);

int eisagogi_telos (info_deikti *linfo, int num);

int eksagogi_arxi (info_deikti *linfo);

int anazhthsh (info_deikti linfo, int num);



#endif
