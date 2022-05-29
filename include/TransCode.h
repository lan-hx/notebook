//
// Created by lan on 2022/3/22.
//

#ifndef NOTEBOOK_INCLUDE_TRANSCODE_H_
#define NOTEBOOK_INCLUDE_TRANSCODE_H_

// i ver password_length password
#define OP_LOGIN 'i'
// g id
#define OP_GET 'g'
// t num ids
#define OP_GET_TOPIC 't'
// a topic_length topic content_length content
#define OP_ADD 'a'
// d id
#define OP_DEL 'd'
// u id topic_length topic content_length content
#define OP_UPD 'u'
// s word_length word
#define OP_SEARCH 's'
// l type
#define OP_LST 'l'
// o
#define OP_LOGOUT 'o'

// i ver
#define RES_LOGIN 'i'
// g topic_length topic content_length content
#define RES_GET 'g'
// t num (id topic_length topic)*num
#define RES_GET_TOPIC 't'
// a id
#define RES_ADD 'a'
// d
#define RES_DEL 'd'
// u
#define RES_UPD 'u'
// s num ids
#define RES_SEARCH 's'
// l num ids
#define RES_LST 'l'
// e op_code errstr_length errstr
#define RES_ERR 'e'

#define TYPE_NORMAL 0
#define TYPE_DELETE 1

#define VERSION 1

#endif //NOTEBOOK_INCLUDE_TRANSCODE_H_
