#include "akinator.h"

#include <ctype.h>
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "colors.h"
#include "str_cmp.h"
#include "stack.h"

static node_t*   NodeCtor(elem_t new_str, node_t* parent);

static node_t**  FindRoot(node_t** current_node);
static char*     ReadString(FILE* database_file, Akinator* akinator);
static node_t*   ParseDataBaseFile(FILE* database_file, Akinator* akinator, node_t* parent);

//======================================== STATIC ========================================
//--------------------------------------- NODE CTOR --------------------------------------
node_t* NodeCtor(elem_t new_str, node_t* parent) {
    assert(new_str != nullptr);

    size_t str_length = strlen(new_str);

    node_t* new_node = (node_t*)calloc(1, sizeof(node_t));
    if (new_node == nullptr) {
        ReportError(" memory allocation failed in Ctor.");
        return nullptr;
    }

    new_node->parent = parent;

    strncpy(new_node->data, new_str, str_length);

    new_node->right = nullptr;
    new_node->left =  nullptr;

    return new_node;
}

//--------------------------------------- TREE DTOR --------------------------------------
Status TreeDtor(node_t* root) {
    if (root == nullptr) {
        return SUCCESS;
    }

    TreeDtor(root->left);
    TreeDtor(root->right);

    root->data = 0;

    free(root);
    root = nullptr;

    return SUCCESS;
}
//------------------------------------- AKINATOR CTOR ------------------------------------
Akinator* AkinatorCtor() {
    Akinator* akinator = (Akinator*)calloc(1, sizeof(Akinator));
    if (akinator == nullptr) {
        ReportError("couldn't create an Akinator.");
        return nullptr;
    }

   akinator->error =     AkinatorError_kOk;
   akinator->last_node = nullptr;

    //вот здесь поиграться со стеком

   return akinator;
}

//------------------------------------- AKINATOR DTOR ------------------------------------
Status AkinatorDtor(Akinator* akinator) {
    assert(akinator != nullptr);

    akinator->error =     AkinatorError_kOk;
    akinator->last_node = nullptr;

    memset(akinator, 0, sizeof(Akinator));
    akinator = nullptr;

    return SUCCESS;
}
//---------------------------------- GET TREE FROM FILE ----------------------------------
node_t* GetTreeFromDataBaseFile(const char* name_of_database_file, Akinator* akinator) {
    assert(name_of_database_file != NULL);

    FILE* database_file = fopen(name_of_database_file, "r");
    if (database_file == nullptr) {
        ReportError("failed to open database file.");
        akinator->error = AkinatorError_kReadingFromFileError;
        return nullptr;
    }
    node_t* root = ParseDataBaseFile(database_file, akinator, nullptr);

    fclose(database_file);

    return root;
}

//------------------------------------- PARSING TREE -------------------------------------
static node_t* ParseDataBaseFile(FILE* database_file, Akinator* akinator, node_t* parent) {
    assert(database_file != nullptr);
    assert(akinator !=      nullptr);

   // char current_char = fgetc(database_file);
    char current_char = '\0';
    int current_char_int = 0;

    // while ((current_char_int = fgetc(database_file)) != EOF) {
    //     current_char = (char)current_char_int;
    // }

    // fclose(database_file);


    while (current_char != EOF) {
        if (current_char == '(') {
            char* current_string = ReadString(database_file, akinator);
            if (current_string == nullptr) {
                akinator->error = AkinatorError_kReadingFromFileError;
                ReportError("empty string.");
                return nullptr;
            }

            if (strcmp(current_string, "nil") != 0) {
                node_t* new_node = NodeCtor(current_string, parent);
                if(new_node == nullptr) {
                    akinator->error = AkinatorError_kMemoryAllocationError;
                    ReportError("couldn't parse a string from file.");
                    free(current_string);
                    return nullptr;
                }

                free(current_string);

                new_node->left =  ParseDataBaseFile(database_file, akinator, new_node);
                new_node->right = ParseDataBaseFile(database_file, akinator, new_node);

                return new_node;
            }
            else {
                free(current_string);
                return nullptr;
            }
        }
        else {
            return nullptr;
        }
    }

    return nullptr;
}

//-------------------------------------- READ STRING ------------------------------------
static char* ReadString(FILE* database_file, Akinator* akinator) {
    assert(database_file != nullptr);

    char* string_buffer = (char*)calloc(kMaxStringSize, sizeof(char));
    if (string_buffer ==  nullptr) {
        akinator->error = AkinatorError_kMemoryAllocationError;
        ReportError(" memory allocation failed in ReadString.");
        return nullptr;
    }

    size_t index = 0;
    char current_char = '0';

    while (index < kMaxStringSize - 1 && fread(&current_char, sizeof(char), 1, database_file) == 1) {
        if (isspace(current_char)
           || current_char == ')') {
            fseek(database_file, 1, SEEK_CUR); // было -1
            break;
        }

        index++;
        string_buffer[index] = current_char;
    }

    string_buffer[index] = '\0';

    return string_buffer;
}

//------------------------------------ INSERT NEW DATA -----------------------------------
void_sex InsertNewNode(node_t* root, elem_t new_data, elem_t new_question, node_t* previous_data) {
    assert(root !=          nullptr);
    assert(previous_data != nullptr);
    assert(new_data !=      nullptr);
    assert(new_question !=  nullptr);

    node_t* new_question_node = (node_t*)calloc(1, sizeof(node_t));
        if (new_question_node == nullptr) {
        ReportError("memory allocation failed while inserting new data.");
        return;
    }

    node_t* new_node = (node_t*)calloc(1, sizeof(node_t));
        if (new_node == nullptr) {
        ReportError("memory allocation failed while inserting new data.");
        return;
    }

    //new_question_node->data =   new_question;
    new_question_node->data = strdup(new_question);
    new_question_node->parent = previous_data->parent;
    new_question_node->left =   previous_data;
    new_question_node->right =  new_node;

    //new_node->data =   new_data;
    new_node->data = strdup(new_data);
    new_node->parent = new_question_node;
    new_node->left =   nullptr;
    new_node->right =  nullptr;

    previous_data->parent = new_question_node;
}

//--------------------------------------- FIND ROOT -------------------------------------
static node_t** FindRoot(node_t** current_node) {
    assert(current_node != nullptr);

    while ((*current_node)->parent != nullptr) {
        *current_node = (*current_node)->parent;
    }
    //сюда можно внедрить запись с конца всех вопросов для выдачи определения
    //отмена это будет реализовано через хранение обхода дерева в структуре акинатора
    return current_node;
}

//--------------------------------------- GET ANSWER ------------------------------------
Answers GetAnswer() {
    printf("%sEnter your choice: %s", TEXT_BLUE, DEFAULT);

    char choice;
    choice = (char)getchar();

    if (choice == 'g' || choice == 'G') {
        return GUESS;
    }
    if (choice == 'd' || choice == 'D') {
        return DEFINE;
    }
    if (choice == 's' || choice == 'S') {
        return SHOW_TREE;
    }
    if (choice == 'c' || choice == 'C') {
        return COMPARE;
    }

    if (choice == 'e' || choice == 'E') {
        return EXIT;
    }
    if (choice == 'k' || choice == 'K') { //? два раза сравниваем с с нужно это исправиь иначе будет крашиться
        return KEEP_UP;
    }

    if (choice == 'y' || choice == 'Y') {
        return YES;
    }
    if (choice == 'n' || choice == 'N') {
        return NO;

    }
    if (choice == 'm' || choice == 'M') {
        return MENU;
    }

    else return UNKNOWN_COMMAND;
}

//-------------------------------------- REPORT ERROR -------------------------------------
void_sex ReportError(const char* message) {
    assert(message != nullptr);

    fprintf(stderr, "%sError: %s%s\n", TEXT_RED, message, DEFAULT);
}

//--------------------------------------- GENIE MEOW --------------------------------------
void_sex PrintDisneyGenie() {
    printf("             .-'\"\"\"\"\"`-.          \n");
    printf("            /           \\             \n");
    printf("           (   o     o   )             \n");
    printf("           |    (_)      |             \n");
    printf("           \\    \\_/     /            \n");
    printf("            '-._______.-'              \n");
    printf("              _( )_( )_                \n");
    printf("           .-'  /     \\  '-.          \n");
    printf("          /     \\     /     \\        \n");
    printf("         |       \\___/       |        \n");
    printf("         |                   |         \n");
    printf("          \\                 /         \n");
    printf("           '.             .'           \n");
    printf("             '._       _.'             \n");
    printf("                `\"\"\"\"\"`           \n");
    printf("                (_/|\\_)               \n");
    printf("                 /   \\                \n");
    printf("                |     |                \n");
    printf("               /       \\              \n");
    printf("              |         |              \n");
    printf("              \\_________/             \n");
    printf("              (_|_____|_)              \n");
    printf("\n");
}

void_sex PrintSleepingGenie() {
    printf("                  zZzZz...                \n");
    printf("                 _______                  \n");
    printf("              .-'       `-.               \n");
    printf("            .'             `.             \n");
    printf("           /    U       U   \\            \n");
    printf("          ;    ___       ___  :           \n");
    printf("          |         __        |           \n");
    printf("          :          0        ;           \n");
    printf("           \\                 /           \n");
    printf("            `.             .'             \n");
    printf("              `-.._____..-'               \n");
    printf("                    ||                    \n");
    printf("             ,  .-' || `-.                \n");
    printf("            | \\/    ||    \\             \n");
    printf("            \\_/|    ||    |_             \n");
    printf("              ||\\   ||   /  `-.          \n");
    printf("              || `-._|_.-'     \\         \n");
    printf("              ||     ||         \\        \n");
    printf("              ||     ||    ___   |        \n");
    printf("              ||     ||   /   \\  |       \n");
    printf("              ||     ||   \\___/ /        \n");
    printf("              ||     ||        /          \n");
    printf("             /  \\   / \\      /          \n");
    printf("            (_,' `-(_,' `-..-'            \n");
    printf("\n");
}
