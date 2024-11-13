#ifndef COLORS_H_
#define COLORS_H_

static const char DEFAULT[] =       "\x1B[0m";

static const char TEXT_YELLOW[] =  "\x1B[33m";
static const char TEXT_GREEN[] =   "\x1B[32m";
static const char TEXT_BLUE[] =    "\x1B[34m";
static const char TEXT_RED[] =     "\x1B[31m";

static const char BCKGR_YELLOW[] = "\x1B[43m";
static const char BCKGR_GREEN[] =  "\x1B[42m";
static const char BCKGR_BLUE[] =   "\x1B[44m";
static const char BCKGR_RED[] =    "\x1B[41m";

#endif // COLORS_H_

// //--------------------------------------- DEFINER ----------------------------------------
// static void_sex DefineMode(node_t* root_ptr, Akinator* akinator) {
//     printf("%sDefine mode selected.%s\n", TEXT_YELLOW, DEFAULT);

//     //принимаем слово от пользователя, ищем его в дереве
//     //1) если слова нет, возвращаем ошибку, очищаем стэк в структуре акинатора
//     //2) по мере поиска запоминаем признаки в стэк в структуре акинатора
//     //причем при проходе налево вместе с признаком записываем в стэк НЕ
//     //выводим само слово, "- это" и возвращаем признаки pop()-ом из стэка

// }

// //--------------------------------------- COMPARER ---------------------------------------
// static void_sex CompareObjects(node_t* root_ptr, Akinator* akinator) {
//     printf("%sCompare Two Objects mode selected.%s\n", TEXT_YELLOW, DEFAULT);
//     //два выхова дефайнера и стркмп?
// }

// //--------------------------------------- SHOW TREE --------------------------------------
// static void_sex ShowTree(node_t* root_ptr, Akinator* akinator) {
//     printf("%sShow Tree mode selected.%s\n", TEXT_YELLOW, DEFAULT);

//     //дамп по сути

// }
