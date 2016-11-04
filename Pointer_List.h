/***************************************************************************************
Arxeio Epikefalidas	: Pointer_List.h -to sigkekrimeno apotelei to arxeio protipo 
Syggrafeas			: Y.Kotrwnis
Skopos			    : Ylopoihsh me deiktes, ATD Syndedemenh Lista
Ana8ewrhsh			: D.Mhlios
                  Ena arxeio diepafhs gia oles tis ylopoihseis listas
****************************************************************************************/
#ifndef __POINTER__LIST_ 
#define __POINTER__LIST_


/*dhlwseis typwn*/

typedef struct info_node  * info_deikti;

typedef struct typos_komvou * typos_deikti;

/*dhlwseis synarthsewn*/

/*dhmioyrgia listas/kastastrofi*/
info_deikti LIST_dimiourgia();
void LIST_katastrofi(info_deikti * linfo);

/*prakseis elegxoy*/
int	LIST_keni(const info_deikti  linfo);

/*prakseis eisagwgh-diagrafh*/
int eisagogi_telos (info_deikti *linfo, int num);
int eksagogi_arxi (info_deikti *linfo);

/* Epilogh Kombou */
void LIST_epomenos(info_deikti  const linfo, typos_deikti * const p, int * const error);
void LIST_first(const info_deikti  linfo, typos_deikti * const first, int * const error);

/*prakseis anazhthshs*/
int anazhthsh (info_deikti linfo, int num);

#endif /*#ifndef __POINTER__LIST_ */
