/*
 *  TOPPERS/SSP Kernel
 *      Smallest Set Profile Kernel
 *
 *  Copyright (C) 2011 by Meika Sugimoto
 * 
 *  ã‹L’˜ìŒ ŽÒ‚ÍCˆÈ‰º‚Ì (1)?(4) ‚ÌðŒ‚ð–ž‚½‚·ê‡‚ÉŒÀ‚èC–{ƒ\ƒtƒgƒEƒF
 *  ƒAi–{ƒ\ƒtƒgƒEƒFƒA‚ð‰ü•Ï‚µ‚½‚à‚Ì‚ðŠÜ‚ÞDˆÈ‰º“¯‚¶j‚ðŽg—pE•¡»E‰ü•ÏE
 *  Ä”z•ziˆÈ‰ºC—˜—p‚ÆŒÄ‚Ôj‚·‚é‚±‚Æ‚ð–³ž‚Å‹–‘ø‚·‚éD
 *  (1) –{ƒ\ƒtƒgƒEƒFƒA‚ðƒ\[ƒXƒRã
??ƒh‚ÌŒ`‚Å—˜—p‚·‚éê‡‚É‚ÍCã‹L‚Ì’˜ìŒ 
 *      •\Ž¦C‚±‚Ì—˜—pðŒ‚¨‚æ‚Ñ‰º‹L‚Ì–³•ÛØ‹K’è‚ªC‚»‚Ì‚Ü‚Ü‚ÌŒ`‚Åƒ\[ƒX
 *      ƒR[ƒh’†‚ÉŠÜ‚Ü‚ê‚Ä‚¢‚é‚±‚ÆD
 *  (2) –{ƒ\ƒtƒgƒEƒFƒA‚ðCƒ‰ƒCƒuƒ‰ƒŠŒ`Ž®‚È‚ÇC‘¼‚Ìƒ\ƒt
ƒgƒEƒFƒAŠJ”­‚ÉŽg—p
 *      ‚Å‚«‚éŒ`‚ÅÄ”z•z‚·‚éê‡‚É‚ÍCÄ”z•z‚É”º‚¤ƒhƒLƒ…ƒƒ“ƒgi—˜—pŽÒƒ}
 *      ƒjƒ…ƒAƒ‹‚È‚Çj‚ÉCã‹L‚Ì’˜ìŒ •\Ž¦C‚±‚Ì—˜—pðŒ‚¨‚æ‚Ñ‰º‹L‚Ì–³•Û
 *      Ø‹K’è‚ðŒf
Ú‚·‚é‚±‚ÆD
 *  (3) –{ƒ\ƒtƒgƒEƒFƒA‚ðC‹@Ší‚É‘g‚Ýž‚Þ‚È‚ÇC‘¼‚Ìƒ\ƒtƒgƒEƒFƒAŠJ”­‚ÉŽg—p
 *      ‚Å‚«‚È‚¢Œ`‚ÅÄ”z•z‚·‚éê‡‚É‚ÍCŽŸ‚Ì‚¢‚¸‚ê‚©‚ÌðŒ‚ð–ž‚½‚·‚±‚ÆD
 *    (a) Ä”z•z‚Éä
??‚¤ƒhƒLƒ…ƒƒ“ƒgi—˜—pŽÒƒ}ƒjƒ…ƒAƒ‹‚È‚Çj‚ÉCã‹L‚Ì’˜ì
 *        Œ •\Ž¦C‚±‚Ì—˜—pðŒ‚¨‚æã
??‰º‹L‚Ì–³•ÛØ‹K’è‚ðŒfÚ‚·‚é‚±‚ÆD
 *    (b) Ä”z•z‚ÌŒ`‘Ô‚ðC•Ê‚É’è‚ß‚é•û–@‚É‚æ‚Á‚ÄCTOPPERS
ƒvƒƒWƒFƒNƒg‚É•ñ
 *        ‚·‚é‚±‚ÆD
 *  (4) –{ƒ\ƒtƒgƒEƒFƒA‚Ì—˜—p‚É‚æ‚è’¼Ú“I‚Ü‚½‚ÍŠÔÚ“I‚É¶‚¶‚é‚¢‚©‚È‚éæ
??ŠQ
 *      ‚©‚ç‚àCã‹L’˜ìŒ ŽÒ‚¨‚æ‚ÑTOPPERSƒvƒƒWƒFƒNƒg‚ð–ÆÓ‚·‚é‚±‚ÆD‚Ü‚½C
 *      –{ƒ\ƒtƒgã‚
?ƒFƒA‚Ìƒ†[ƒU‚Ü‚½‚ÍƒGƒ“ƒhƒ†[ƒU‚©‚ç‚Ì‚¢‚©‚È‚é——R‚ÉŠî‚Ã
 *      ‚­¿‹‚©‚ç‚àCã‹L’˜ìŒ ŽÒ‚¨‚æ‚ÑTOPPERSƒvƒƒWƒFƒNƒg‚ð–ÆÓ‚·‚é‚±‚ÆD
 * 
 *  –{ƒ\ƒtƒgƒEƒFƒA‚ÍC–³•ÛØ‚Å’ñ‹Ÿ‚³‚ê‚Ä‚¢‚é‚à‚Ì‚Å‚ ‚éDã‹L’˜ìŒ ŽÒ‚¨‚æ
 *  ‚ÑTOPPERSƒvƒã
??ƒFƒNƒg‚ÍC–{ƒ\ƒtƒgƒEƒFƒA‚ÉŠÖ‚µ‚ÄC“Á’è‚ÌŽg—p–Ú“I‚É‘Î‚·
 *  ‚é“K‡«‚àŠÜ‚ß‚ÄC‚¢‚©‚È‚éä
??Ø‚às‚í‚È‚¢D‚Ü‚½C–{ƒ\ƒtƒgƒEƒFƒA‚Ì—˜—p
 *  ‚É‚æ‚è’¼Ú“I‚Ü‚½‚ÍŠÔÚ“I‚É¶‚¶‚½‚¢‚©‚È‚é‘¹ŠQ‚ÉŠÖ‚µ‚Ä‚àC‚»‚ÌÓ”C‚ð•‰
 *  ‚í‚È‚¢D
 * 
 */

/*
 *  ƒLƒ…[‘€ìƒwƒbƒ_
 *
 *  SSPƒJ[ƒlƒ‹‚Å‚Íƒƒ‚ƒŠß–ñ‚Ì‚½‚ß‚ÉƒCƒ“ƒfƒbƒNƒX‚ðŽg—p‚µ‚½ƒLƒ…[‚ð—p‚¢‚é
 */

#ifndef TOPPERS_QUEUE_H
#define TOPPERS_QUEUE_H

#if defined __GNUC__
#ifndef Inline
#define Inline static inline
#endif
#endif


/* ƒLƒ…[‚ÌƒCƒ“ƒfƒbƒNƒX */
typedef uint8_t QUEIDX;

/* ƒLƒ…[\‘¢‘Ì */
typedef struct
{
	QUEIDX	next;
	QUEIDX	prev;
} QUEUE;


/* ƒLƒ…[ƒwƒbƒ_‚Ì‰Šú‰» */
Inline void
queue_initialize(QUEUE *queue , QUEIDX queue_null)
{
	queue->next = queue->prev = queue_null;
}

/* Žw’è‚µ‚½—v‘f‚Ì‘O‚ÉV‚½‚È—v‘f‚ð’Ç‰Á */
Inline  void
queue_insert_prev(QUEUE *top , QUEIDX queue , QUEIDX entry)
{
	top[entry].prev = top[queue].prev;
	top[entry].next = queue;
	top[top[queue].prev].next = entry;
	top[queue].prev = entry;
}


/* Žw’è‚µ‚½ƒLƒ…[‚ÌŽŸ‚Ì—v‘f‚ðíœ‚µC‚»‚ÌƒCƒ“ƒfƒbƒNƒX‚ð•Ô‚· */
Inline QUEIDX
queue_delete_next(QUEUE *top , QUEIDX queue)
{
	top[top[queue].next].prev = top[queue].prev;
	top[top[queue].prev].next = top[queue].next;
	
	return top[queue].next;
}


/*
 *  ƒLƒ…[‚ª‹ó‚©‚Ç‚¤‚©‚ÌƒeƒXƒg
 *
 *  queue‚É‚ÍƒLƒ…[ƒwƒbƒ_‚ðŽw’è‚·‚é‚±‚Æ
 */
Inline bool_t
queue_empty(QUEUE *queue , QUEIDX queue_null)
{
	bool_t empty;
	
	if(queue->next == queue_null)
	{
		empty = true;
	}
	else
	{
		empty = false;
	}
	
	return empty;
}

#endif /* TOPPERS_QUEUE_H */
