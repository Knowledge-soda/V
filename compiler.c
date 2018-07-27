#include<stdlib.h>
#include<stdio.h>

#include"compiler.h"
#include"tools.h"

static int compile_node_type(ParserNode *node){
    /* compiles node by type */
    char tmp_name[200];
    byte t = get_type(node);
    if (t == PS_NUM){
        printf("mov eax, %i\n", node -> name);
    }
    if (t == PS_VAR){
        num_name(node -> name, tmp_name);
        printf("mov eax, $%s\n", tmp_name);
    }
    if (t == PS_VAL){
        printf("mov eax, [ebp - %i]\n", 4 + 4 * (node -> name));
    }
    if (t == PS_PLC){
        num_name(node -> name, tmp_name);
        printf("jmp $ + 6\n");
        printf("$%s:\n", tmp_name);
        printf("pop ebp\n");
        printf("add esp, ecx\n");
        printf("pop ebx\n");
    }
    if (t == PS_COM){
        printf("push dword eax\n");
    }
    if (t == PS_BLN && node -> name == BL_LAST){
        printf("mov eax, edi\n");
    }
    return NO_ERROR;
}

static int compile_built_in(int type, int node_size){
    if (type == BL_NOP){
    } else if (type == BL_LAST){
    } else if (type == BL_SUM){
        printf("mov eax, [ebp - 4]\n");
        printf("add eax, [ebp - 8]\n");
    } else if (type == BL_MUL){
        printf("mov eax, [ebp - 4]\n");
        printf("imul eax, [ebp - 8]\n");
    } else if (type == BL_VAL){
        printf("mov eax, [ebp - 4]\n");
        printf("mov eax, [eax]\n");
    } else if (type == BL_MOV){
        printf("mov dword eax, [ebp - 8]\n");
        printf("mov dword ecx, [ebp - 4]\n");
        printf("mov [ecx], eax\n");
    } else if (type == BL_EQU){
        printf("xor eax, eax\n");
        printf("mov ecx, [ebp - 8]\n");
        printf("cmp dword [ebp - 4], ecx\n");
        printf("sete al\n");
    } else if (type == BL_MORE){
        printf("xor eax, eax\n");
        printf("mov ecx, [ebp - 8]\n");
        printf("cmp dword [ebp - 4], ecx\n");
        printf("setg al\n");
    } else if (type == BL_LESS){
        printf("xor eax, eax\n");
        printf("mov ecx, [ebp - 8]\n");
        printf("cmp dword [ebp - 4], ecx\n");
        printf("setl al\n");
    } else if (type == BL_IF){
        /* if(a, b, c) = !a*b + a*c */
        printf("xor eax, eax\n");
        printf("xor ecx, ecx\n");
        printf("cmp dword [ebp - 4], 0\n");
        printf("sete cl\n");
        printf("setne al\n");
        printf("imul eax, [ebp - 8]\n");
        printf("imul ecx, [ebp - 12]\n");
        printf("add eax, ecx\n");
    } else if (type == BL_IJMP){
        printf("cmp dword [ebp - 4], 0\n");
        printf("je $ + 6\n");
        printf("mov ecx, %i\n", node_size * 4);
        printf("jmp [ebp - 8]\n");
    } else if (type == BL_PRNT){
        printf("pusha\n");
        printf("push dword [ebp - 4]\n");
        printf("push prINT\n");
        printf("call printf\n");
        printf("add esp, 8\n");
        printf("popa\n");
    } else if (type == BL_LOAD){
        printf("pusha\n");
        printf("push dword [ebp - 4]\n");
        printf("push scINT\n");
        printf("call scanf\n");
        printf("add esp, 8\n");
        printf("popa\n");
    } else if (type == BL_NOT){
        printf("xor eax, eax\n");
        printf("cmp dword [ebp - 4], 0\n");
        printf("sete al\n");
    } else if (type == BL_AND){
        printf("xor eax, eax\n");
        printf("xor ecx, ecx\n");
        printf("cmp dword [ebp - 4], 0\n");
        printf("setne al\n");
        printf("cmp dword [ebp - 8], 0\n");
        printf("setne cl\n");
        printf("and al, cl\n");
    } else if (type == BL_OR){
        printf("xor eax, eax\n");
        printf("xor ecx, ecx\n");
        printf("cmp dword [ebp - 4], 0\n");
        printf("setne al\n");
        printf("cmp dword [ebp - 8], 0\n");
        printf("setne cl\n");
        printf("or al, cl\n");
    } else if (type == BL_SUB){
        printf("mov eax, [ebp - 4]\n");
        printf("sub eax, [ebp - 8]\n");
    } else if (type == BL_DIV){
        printf("mov eax, [ebp - 4]\n");
        printf("xor edx, edx\n");
        printf("idiv dword [ebp - 8]\n");
    } else if (type == BL_MOD){
        printf("mov eax, [ebp - 4]\n");
        printf("xor edx, edx\n");
        printf("idiv dword [ebp - 8]\n");
        printf("mov eax, edx\n");
    } else if (type == BL_JMP){
        printf("jmp [ebp - 4]\n");
    } else if (type == BL_STR){
        printf("push dword [ebp - 4]\n");
        printf("push prSTR\n");
        printf("call printf\n");
        printf("add esp, 8\n");
    } else if (type == BL_IFC){
        printf("cmp dword [ebp - 4], 0\n");
        printf("je $+10\n");
        printf("sub ebp, 8\n");
        printf("mov ebx, esp\n");
        printf("call dword [ebp]\n");
        printf("add ebp, 8\n");
    } else if (type == BL_CALL){
        printf("sub ebp, 4\n");
        printf("mov ebx, esp\n");
        printf("call dword [ebp]\n");
        printf("add ebp, 4\n");
    } else if (type == BL_PRNR){
        printf("pusha\n");
        printf("push dword [ebp - 4]\n");
        printf("push prINTR\n");
        printf("call printf\n");
        printf("add esp, 8\n");
        printf("popa\n");
    } else if (type == BL_IFEC){
        printf("xor eax, eax\n");
        printf("xor ecx, ecx\n");
        printf("cmp dword [ebp - 4], 0\n");
        printf("sete cl\n");
        printf("setne al\n");
        printf("imul eax, [ebp - 8]\n");
        printf("imul ecx, [ebp - 12]\n");
        printf("add eax, ecx\n");
        printf("sub ebp, 12\n");
        printf("mov ebx, esp\n");
        printf("call dword eax\n");
        printf("add ebp, 12\n");
    } else if (type == BL_AT){
        printf("mov eax, [ebp - 4]\n");
        printf("mov ecx, [ebp - 8]\n");
        printf("lea eax, [eax + 4 * ecx]\n");
    } else if (type == BL_ATMV){
        printf("mov eax, [ebp - 4]\n");
        printf("mov ecx, [ebp - 8]\n");
        printf("mov edx, [ebp - 12]\n");
        printf("mov dword [eax + 4 * ecx], edx\n");
    } else if (type == BL_INDX){
        printf("mov eax, [ebp - 4]\n");
        printf("mov ecx, [ebp - 8]\n");
        printf("mov eax, [eax + 4 * ecx]\n");
    } else if (type == BL_PRNF){
        printf("pusha\n");
        printf("fld dword [ebp - 4]\n");
        printf("sub esp, 8\n");
        printf("fstp qword [esp]\n");
        printf("push prFLOAT\n");
        printf("call printf\n");
        printf("add esp, 12\n");
        printf("popa\n");
    } else if (type == BL_PRFR){
        printf("pusha\n");
        printf("fld dword [ebp - 4]\n");
        printf("sub esp, 8\n");
        printf("fstp qword [esp]\n");
        printf("push prFLOATR\n");
        printf("call printf\n");
        printf("add esp, 12\n");
        printf("popa\n");
    } else if (type == BL_FSUM){
        printf("fld dword [ebp - 8]\n");
        printf("fadd dword [ebp - 4]\n");
        printf("fstp dword [ebp - 4]\n");
        printf("mov eax, [ebp - 4]\n");
    } else if (type == BL_FMUL){
        printf("fld dword [ebp - 8]\n");
        printf("fmul dword [ebp - 4]\n");
        printf("fstp dword [ebp - 4]\n");
        printf("mov eax, [ebp - 4]\n");
    } else if (type == BL_FSUB){
        printf("fld dword [ebp - 8]\n");
        printf("fsubr dword [ebp - 4]\n");
        printf("fst dword [ebp - 4]\n");
        printf("mov eax, [ebp - 4]\n");
    } else if (type == BL_FDIV){
        printf("fld dword [ebp - 8]\n");
        printf("fdivr dword [ebp - 4]\n");
        printf("fst dword [ebp - 4]\n");
        printf("mov eax, [ebp - 4]\n");
    } else if (type == BL_LDF){
        printf("pusha\n");
        printf("push dword [ebp - 4]\n");
        printf("push prFLOATR\n");
        printf("call scanf\n");
        printf("add esp, 8\n");
        printf("popa\n");
    } else if (type == BL_FEQU){
        printf("fld dword [ebp - 8]\n");
        printf("fcom dword [ebp - 4]\n");
        printf("fstsw ax\n");
        printf("sahf\n");
        printf("mov eax, 0\n");
        printf("sete al\n");
    } else if (type == BL_FORE){
        printf("fld dword [ebp - 8]\n");
        printf("fcom dword [ebp - 4]\n");
        printf("fstsw ax\n");
        printf("sahf\n");
        printf("mov eax, 0\n");
        printf("setb al\n");
    } else if (type == BL_FESS){
        printf("fld dword [ebp - 8]\n");
        printf("fcom dword [ebp - 4]\n");
        printf("fstsw ax\n");
        printf("sahf\n");
        printf("mov eax, 0\n");
        printf("seta al\n");
    } else if (type == BL_ITF){
        printf("fild dword [ebp - 4]\n");
        printf("fstp dword [ebp - 4]\n");
        printf("mov eax, [ebp - 4]\n");
    } else if (type == BL_FTI){
        printf("fld dword [ebp - 4]\n");
        printf("fistp dword [ebp - 4]\n");
        printf("mov eax, [ebp - 4]\n");
    } else {
        return UNDEF_ERROR;
    }
    return NO_ERROR;
}

static int compile_node_func(ParserNode *node){
    printf("; entering function\n");
    printf("push ebp\n");
    printf("mov ebp, ebx\n");
    printf("mov ebx, esp\n");
    if (get_type(node) == PS_BLN){
        compile_built_in(node -> name, node -> size);
    } else {
        printf("call eax\n");
    }
    printf("mov edi, eax\n");
    printf("pop ebp\n");
    printf("add esp, %i\n", 4 * (node -> size));
    printf("mov ebx, esp\n");
    return NO_ERROR;
}

static int compile_function(ParserFunction *func){
    char tmp_name[200];
    int err;
    if (func -> name == -1){
        printf("main:\n");
    } else {
        num_name(func -> name, tmp_name);
        printf("$%s:\n", tmp_name);
    }
    printf("; defining function\n");
    printf("mov ebx, esp\n");

    ParserNode *stack[MAXDEPTH];
    int stack_sz = 0;
    ParserNode *curr;
    curr = func -> first;
    while (stack_sz >= 0){
        printf("; ");
        dbg_parser(curr);
        printf("\n");
        if (is_func(curr)){
            printf("; fn\n");
            printf("push ebx\n");
            printf("mov ebx, esp\n");
            stack[stack_sz] = curr;
            stack_sz++;
            curr = curr -> first;
        } else {
            printf("; not fn\n");
            err = compile_node_type(curr);
            if (err) return err;
            curr = curr -> next;
            if (!curr){
                printf("; returning from function\n");
                stack_sz--;
                if (stack_sz < 0) break;
                curr = stack[stack_sz];
                printf("; ");
                dbg_parser(curr);
                printf("\n");
                printf("push eax\n");
                err = compile_node_type(curr);
                if (err) return err;
                err = compile_node_func(curr);
                if (err) return err;
                printf("pop ebx\n");
                curr = curr -> next;
            }
        }
    }

    printf("ret\n");
    return NO_ERROR;
}

int compile_file(ParserFile *file){
    char tmp_name[200];
    int err;
    printf("section .text\n");
    printf("global main\n");
    printf("extern printf\n");
    printf("extern scanf\n");

    ParserFunction *f;
    for (f = file -> first;f;f = f -> next){
        err = compile_function(f);
        if (err) return err;
    }

    printf("section .data\n");
    printf("prINT db \"%%i\", 10, 0\n");
    printf("prINTR db \"%%i\", 0\n");
    printf("scINT db \"%%i\", 0\n");
    printf("prSTR db '%%s', 0\n");
    printf("prFLOAT db \"%%f\", 10, 0\n");
    printf("prFLOATR db '%%f', 0\n");
    int i;
    for (i = 0;i < file -> str_size;i++){
        printf(";%i\n", (file -> str_vals)[i]);
        num_name((file -> str_vals)[i], tmp_name);
        printf("$%s db `%s`, 0\n", tmp_name, (file -> strings)[i]);
    }
    printf("section .bss\n");
    for (i = 0;i < file -> data_size;i++){
        num_name((file -> data)[i], tmp_name);
        printf("$%s resd %i\n", tmp_name, (file -> sizes)[i]);
    }

    return NO_ERROR;
}
