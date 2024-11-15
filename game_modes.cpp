#include "game_modes.h"

#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include <cstdlib>
#include <cstdint>

#include "akinator.h"
#include "colors.h"
#include "str_cmp.h"
#include "stack.h"

static void_sex  MainPage(node_t* root_ptr, Akinator* akinator);

static void_sex  ShowTree(node_t* root_ptr, Akinator* akinator);
static void_sex  GuessMode(node_t* root_ptr, Akinator* akinator);
static void_sex  DefineMode(node_t* root_ptr, Akinator* akinator);
static void_sex  ExitProgram(node_t* root_ptr, Akinator* akinator);
static void_sex  CompareObjects(node_t* root_ptr, Akinator* akinator);

static Answers   AskQuestions(node_t* root_ptr, Akinator* akinator);
static void_sex  GetDataFromUser(Akinator* akinator, elem_t users_word, elem_t users_feature);

//======================================== PUBLIC ========================================
//----------------------------------------- GAME -----------------------------------------
void_sex Game() {
    //PrintDisneyGenie();
    printf("Welcome to the Akinator Game!\n\n");

    Akinator* akinator = AkinatorCtor();
                                            // FIXME вынести хотя бы в константу
    node_t* root_ptr = GetTreeFromDataBaseFile("database.txt", akinator);

    MainPage(root_ptr, akinator);
}
//--------------------------------------- MAIN PAGE --------------------------------------
static void_sex MainPage(node_t* root_ptr, Akinator* akinator) { // NOTE лучше основную структуру(в данном случае акинатор) передавать первым
    assert(root_ptr != nullptr);
    assert(akinator != nullptr);

    while (1) { // NOTE можно true
        printf("%sPlease, choose the game mode:%s\n", TEXT_BLUE, DEFAULT);


        printf("%s[G]%suess\n", BCKGR_YELLOW, TEXT_YELLOW);
        printf("%s[D]%sefine\n", BCKGR_YELLOW, TEXT_YELLOW);
        printf("%s[S]%show Tree\n", BCKGR_YELLOW, TEXT_YELLOW);
        printf("%s[C]%sompare Two Objects\n", BCKGR_YELLOW, TEXT_YELLOW);
        printf("%s[E]%sxit\n", TEXT_RED, DEFAULT);

        printf("%sEnter your choice: %s", TEXT_BLUE, DEFAULT);

        int choice_num = GetAnswer();

        switch (choice_num) {
            case GUESS:
                GuessMode(root_ptr, akinator);
                break;

            case DEFINE:
                DefineMode(root_ptr, akinator);
                break;

            case SHOW_TREE:
                ShowTree(root_ptr, akinator);
                break;

            case COMPARE:
                CompareObjects(root_ptr, akinator);
                break;

            case EXIT:
                ExitProgram(root_ptr, akinator);
                break;

            default:
                printf("%sInvalid choice%s u stupid ass. Please enter one of the letters %s('G', 'D', 'S', 'C', 'E')%s.\n", TEXT_BLUE, DEFAULT, BCKGR_BLUE, DEFAULT);
                break;
        }
    }
}

//***************************************** MODES ****************************************
//----------------------------------------- GUESS ----------------------------------------
static void_sex GuessMode(node_t* root_ptr, Akinator* akinator) {
    assert(root_ptr != nullptr);
    assert(akinator != nullptr);

    printf("%sGuess mode selected.%s\n", TEXT_YELLOW, DEFAULT);

    if (AskQuestions(root_ptr, akinator) == YES) {
        printf("%sIt's so easy to read people's minds these days, lol, almost boring. Do you dare challenge me once more? Or will you, the greatest, return to the menu to shamefully admit defeat and leave the game? Which will you choose, player?%s",  TEXT_BLUE, DEFAULT);
        printf("%s[K]%eep up%s    %s[M]%senu%s ", BCKGR_BLUE, TEXT_BLUE, DEFAULT, BCKGR_BLUE, TEXT_BLUE, DEFAULT);
        printf("\t%s[E]%sxit\n", TEXT_RED, DEFAULT);

        int choice_num = GetAnswer();

        switch (choice_num) {
                case EXIT:
                    ExitProgram(root_ptr, akinator);
                    break;
                case MENU:
                    MainPage(root_ptr, akinator);
                    break;
                case KEEP_UP:
                    GuessMode(root_ptr, akinator);
                    break;
                default:
                    printf("%sInvalid choice%s u stupid ass. Please enter one of the letters %s('K'/'M'/'E')%s.\n", TEXT_BLUE, DEFAULT, BCKGR_BLUE, DEFAULT);
                    break;
            }
    } // NOTE } else {
    else {
        printf("%sI've never known about this before... Share your knowledge, will you?%s", TEXT_BLUE, DEFAULT);

        char users_word[kMaxStringSize] = {};
        char users_feature[kMaxStringSize] = {};

        printf("%sWhat object were you thinking of? %s", TEXT_BLUE, DEFAULT);

        if (fgets(users_word, kMaxStringSize, stdin)) {
            size_t word_len = strlen(users_word);

            if (word_len > 0 && users_word[word_len - 1] == '\n') {
                users_word[word_len - 1] = '\0';
            }
        }

        printf("%sHow is it different from %s? %s", TEXT_BLUE, akinator->last_node->data, DEFAULT);

        if (fgets(users_feature, kMaxStringSize, stdin)) {
            size_t feature_len = strlen(users_feature);

            if (feature_len > 0 && users_feature[feature_len - 1] == '\n') {
                users_feature[feature_len - 1] = '\0';
            }
        }

       //? GetDataFromUser(akinator, users_word, users_feature);

        InsertNewNode(root_ptr, users_word, users_feature, akinator->last_node);
        printf("%sThank you, player. I'll consider that in the future.\n Do you wish to keep up?%s", TEXT_BLUE, DEFAULT);
        printf("%s[K]%seep up%s    %s[M]%senu%s ", BCKGR_BLUE, TEXT_BLUE, DEFAULT, BCKGR_BLUE, TEXT_BLUE, DEFAULT);
        printf("\t%s[E]%sxit\n", TEXT_RED, DEFAULT);

        int choice_num = GetAnswer();

        switch (choice_num) {
                case EXIT:
                    ExitProgram(root_ptr, akinator);
                    break;
                case MENU:
                    MainPage(root_ptr, akinator);
                    break;
                case KEEP_UP:
                    GuessMode(root_ptr, akinator);
                    break;
                default:
                    printf("%sInvalid choice%s. Please enter one of the letters %s('C'/'M'/'E')%s.\n", TEXT_BLUE, DEFAULT, BCKGR_BLUE, DEFAULT);
                    break;
            }
    }

}

//--------------------------------------- DEFINER ----------------------------------------
static void_sex DefineMode(node_t* root_ptr, Akinator* akinator) {
    assert(root_ptr != nullptr);
    assert(akinator != nullptr);

    printf("%sDefine mode selected.%s\n", TEXT_YELLOW, DEFAULT);



    //!принимаем слово от пользователя, ищем его в дереве
    //!1) если слова нет, возвращаем ошибку, очищаем стэк в структуре акинатора
    //!2) по мере поиска запоминаем признаки в стэк в структуре акинатора
    //!причем при проходе налево вместе с признаком записываем в стэк НЕ
    //!выводим само слово, "- это" и возвращаем признаки pop()-ом из стэка




}

//--------------------------------------- COMPARER ---------------------------------------
static void_sex CompareObjects(node_t* root_ptr, Akinator* akinator) {
    assert(root_ptr != nullptr);
    assert(akinator != nullptr);

    printf("%sCompare Two Objects mode selected.%s\n", TEXT_YELLOW, DEFAULT);
    //? два выхова дефайнера и стркмп?
}

//--------------------------------------- SHOW TREE --------------------------------------
static void_sex ShowTree(node_t* root_ptr, Akinator* akinator) {
    assert(root_ptr != nullptr);
    assert(akinator != nullptr);

    printf("%sShow Tree mode selected.%s\n", TEXT_YELLOW, DEFAULT);

    //! дамп по сути

}

//------------------------------------------ EXIT ----------------------------------------
static void_sex ExitProgram(node_t* root_ptr, Akinator* akinator) {
    assert(root_ptr != nullptr);
    assert(akinator != nullptr);

    printf("%sAre you sure you want to exit the programm?%s", TEXT_BLUE, DEFAULT);
    printf("%s[Y]%sES%s   //   [N]O (go back to the menu)  ", BCKGR_BLUE, TEXT_BLUE, DEFAULT);

    int choice_num = GetAnswer();

    switch (choice_num) {
            case YES:
                printf("%sExiting the program.%s\n", TEXT_BLUE, DEFAULT);
                PrintSleepingGenie();
                printf("%sThank you for the game)%s\n", TEXT_BLUE, DEFAULT);

                TreeDtor(root_ptr);
                AkinatorDtor(akinator);
                //? additional info maybe (e.g. "You've played this much times for this time have added this much new info and so on")

                return;

            case NO:
                MainPage(root_ptr, akinator);
                break;

            default:
                printf("%sInvalid choice%s u stupid ass. Please enter one of the letters %s('Y'/'N')%s.\n", TEXT_BLUE, DEFAULT, BCKGR_BLUE, DEFAULT);
                break;
        }
}

//======================================== STATIC ========================================
//------------------------------------ ASK QUESTIONS -------------------------------------
static Answers AskQuestions(node_t* current_node, Akinator* akinator) {
    assert(current_node != nullptr);
    assert(akinator !=     nullptr);

    printf("%s?", current_node->data);

    int choice_num = GetAnswer();

    switch(choice_num) {
        case YES:
            return AskQuestions(current_node->right, akinator);
            akinator->last_node = current_node->right;
            break;

        case NO:
            return AskQuestions(current_node->left, akinator);
            akinator->last_node = current_node->left;
            break;

        default:
            printf("%sInvalid choice%s u stupid ass. Please enter one of the letters %s('Y'/'N')%s.\n", TEXT_BLUE, DEFAULT, BCKGR_BLUE, DEFAULT);
            return UNKNOWN_COMMAND;
            break;
    }

    if (current_node->left == nullptr && current_node->right == nullptr) {
        printf("%sAccording to the information I received, you were thinking of a word %s! Am I correct? %s", TEXT_BLUE, current_node->data, DEFAULT);
        return GetAnswer();
    }
}

//---------------------------------- GET DATA FROM USER ----------------------------------
static void_sex GetDataFromUser(Akinator* akinator, elem_t users_word, elem_t users_feature) {
    assert(akinator !=      nullptr);
    assert(users_word !=    nullptr);
    assert(users_feature != nullptr);

    char new_word[kMaxStringSize] = {};
    printf("%sWhat object were you thinking of? \n%s", TEXT_BLUE, DEFAULT);
    fgets(new_word, kMaxStringSize, stdin); // ? лучше gets или getline?

    *users_word = *strdup(new_word);

    char new_feature[kMaxStringSize] = {};
    printf("%sHow different is it from %s?%s", TEXT_BLUE, akinator->last_node->data, DEFAULT);
    fgets(new_feature, kMaxStringSize, stdin);

    *users_feature = *strdup(new_feature);
}