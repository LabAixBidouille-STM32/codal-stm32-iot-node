/**
 ******************************************************************************
 * @file    list.h
 * @author  MCD Application Team
 * @brief   Header file for linked list library.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics International N.V.
 * All rights reserved.</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted, provided that the following conditions are met:
 *
 * 1. Redistribution of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. Neither the name of STMicroelectronics nor the names of other 
 *    contributors to this software may be used to endorse or promote products 
 *    derived from this software without specific written permission.
 * 4. This software, including modifications and/or derivative works of this 
 *    software, must execute solely and exclusively on microcontroller or
 *    microprocessor devices manufactured by or for STMicroelectronics.
 * 5. Redistribution and use of this software other than as permitted under 
 *    this license is void and will automatically terminate your rights under 
 *    this license. 
 *
 * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS" 
 * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
 * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
 * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT 
 * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

#ifndef _LIST_H_
#define _LIST_H_

/* Includes ------------------------------------------------------------------*/
#include <common.h>

#ifdef __GNUC__
typedef struct __packed _tListNode {
#else
typedef __packed struct _tListNode {
#endif
  struct _tListNode * next;
  struct _tListNode * prev;
} tListNode;

typedef tListNode* pListNode;

void list_init_head (tListNode * listHead);

uint8_t list_is_empty (tListNode * listHead);

void list_insert_head (tListNode * listHead, tListNode * node);

void list_insert_tail (tListNode * listHead, tListNode * node);

void list_remove_node (tListNode * node);

void list_remove_head (tListNode * listHead, tListNode ** node );

void list_remove_tail (tListNode * listHead, tListNode ** node );

void list_insert_node_after (tListNode * node, tListNode * ref_node);

void list_insert_node_before (tListNode * node, tListNode * ref_node);

int list_get_size (tListNode * listHead);

void list_get_next_node (tListNode * ref_node, tListNode ** node);

void list_get_prev_node (tListNode * ref_node, tListNode ** node);

#endif /* _LIST_H_ */
