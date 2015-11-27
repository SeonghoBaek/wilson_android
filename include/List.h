/** 
*
*  @author		Seongho Baek
*
*/

#ifndef _LIST_H
#define _LIST_H

#define 	LIST_HEAD(T) public: T* p_next; T *p_prev 

#define 	LINKED_LIST(T) LIST_HEAD(T)

#define		LIST_FOR_EACH(ploop,plisthead)	\
	for( (ploop) = (plisthead)->p_next; (ploop) && (ploop) != (plisthead); (ploop) = (ploop)->p_next )


#define		LIST_FOR_EACH_PREV(ploop,plisthead)	\
	for( (ploop) = (plisthead)->p_prev; (ploop) != (plisthead); (ploop) = (ploop)->p_prev )

#define LIST_HEAD_INIT(pListHead) \
		pListHead->p_next = pListHead->p_prev = pListHead

#define LIST_ADDI(pPrevHead, pNextHead, pNewHead) \
do \
{ \
	pNextHead->p_prev = pNewHead; \
	pNewHead->p_next = pNextHead; \
	pNewHead->p_prev = pPrevHead; \
	pPrevHead->p_next = pNewHead; \
} while (0)

#define LIST_ADD(pNewHead, pListHead) \
	LIST_ADDI(pListHead, pListHead->p_next, pNewHead) 

#define LIST_DELI(pPrevHead, pNextHead) \
do \
{  \
	pPrevHead->p_next = pNextHead; \
	pNextHead->p_prev = pPrevHead; \
} while (0)

#define LIST_DEL(pListHead) \
	LIST_DELI(pListHead->p_prev, pListHead->p_next)

#define	LIST_INIT(pListHead)		LIST_HEAD_INIT(pListHead)

#define LIST_NEXT(pListHead)		pListHead->p_next;
#define LIST_PREV(pListHead)		pListHead->p_prev;

#endif
