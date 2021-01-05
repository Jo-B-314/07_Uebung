#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include<cassert>
#include "lp_lib.h"

std::string get_edge_name(int i);


int main (int argc, char* argv[]) {
    lprec *lp;
    lp = make_lp(0, 32);
    //we have 16 edges in our graph but we need 32 variables because we need a directed graph
    if (lp == NULL) {
        std::cerr << "could not create new LP model\n";
        return 1;
    }
    /**
     * 1 => A->B
     * 2 => A->C
     * 3 => A->D
     * 4 => B->A
     * 5 => B->E
     * 6 => B->F
     * 7 => B->L
     * 8 => C->A
     * 9 => C->J
     * 10 => D->A
     * 11 => D->F
     * 12 => D->G
     * 13 => E->B
     * 14 => E->I
     * 15 => E->J
     * 16 => F->B
     * 17 => F->D
     * 18 => G->D
     * 19 => G->H
     * 20 => H->G
     * 21 => H->I
     * 22 => I->E
     * 23 => I->H
     * 24 => J->C
     * 25 => J->E
     * 26 => K->L
     * 27 => K->M
     * 28 => L->B
     * 29 => L->K
     * 30 => L->M
     * 31 => M->K
     * 32 => M->L
     */
    // now we add the objective function
    REAL objective[33] = {0, 12, 2, 3, 12, 3, 1, 15, 2, 4, 3, 3, 4, 
        3, 2, 6, 1, 3, 4, 2, 2, 2, 2, 2, 4, 6, 2, 3, 15, 2, 10, 3, 10};
    set_obj_fnex(lp, 32, objective, NULL);
    //now we tell we want to minimize
    set_minim(lp);
    //first constraint: no incoming edges from A
    REAL row1[33];
    for (int i = 0; i < 33; i++) {
        row1[i] = 0;
    }
    row1[4] = 1; //B->A
    row1[8] = 1; //C->A
    row1[10] = 1; //D->A
    add_constraint(lp, row1, 3, 0);
    //second constraint: exactly one outgoing edge from A
    REAL row2[33];
    for (int i = 0; i < 33; i++) {
        row2[i] = 0;
    }
    row2[1] = 1; //A->B
    row2[2] = 2; //A->C
    row2[3] = 3; //A->D
    add_constraint(lp, row2, 3, 1);
    //third constraint: exactly one incoming edge from M
    REAL row3[33];
    for (int i = 0; i < 33; i++) {
        row3[i] = 0;
    }
    row3[27] = 1; //K->M
    row3[30] = 1; //L->M
    add_constraint(lp, row3, 3, 1);
    //fourth constraint: for all B....L #incoming edges = #outgoing edges
    //TODO
    //all our variables must be binary
    for (int i = 1; i <= 32; i++) {
        set_binary(lp, i, 1);
    }
    if (solve(lp)) {
        // we have a solution now we want to now it
        REAL solution[32];
        for (int i = 1; i<= 32; i++) {
            solution[i-1] = get_var_primalresult(lp, i);
        }
        std::string erg;
        for (int i = 0; i < 32; i++) {
            REAL comp1 = 1;
            if (solution[i] == comp1) {
                erg += get_edge_name(i+1);
            }
        }
        std::cout << "computed path: \n" << erg << std::endl;
    return 0;
    } else {
        std::cerr << "No optimal solution found\n";
        return 1;
    }
}


std::string get_edge_name(int i) {
    std::string res;
    switch (i) {
        case 1: res = "A->B, "; break;
        case 2: res = "A->C, "; break;
        case 3: res = "A->D, "; break;
        case 4: res = "B->A, "; break;
        case 5: res = "B->E, "; break;
        case 6: res = "B->F, "; break;
        case 7: res = "B->L, "; break;
        case 8: res = "C->A, "; break;
        case 9: res = "C->J, "; break;
        case 10: res = "D->A, "; break;
        case 11: res = "D->F, "; break;
        case 12: res = "D->G, "; break;
        case 13: res = "E->B, "; break;
        case 14: res = "E->I, "; break;
        case 15: res = "E->J, "; break;
        case 16: res = "F->B, "; break;
        case 17: res = "F->D, "; break;
        case 18: res = "G->D, "; break;
        case 19: res = "G->H, "; break;
        case 20: res = "H->G, "; break;
        case 21: res = "H->I, "; break;
        case 22: res = "I->E, "; break;
        case 23: res = "I->H, "; break;
        case 24: res = "J->C, "; break;
        case 25: res = "J->E, "; break;
        case 26: res = "K->L, "; break;
        case 27: res = "K->M"; break;
        case 28: res = "L->B, "; break;
        case 29: res = "L->K, "; break;
        case 30: res = "L->M"; break;
        case 31: res = "M->K, "; break;
        case 32: res = "M->L, "; break;        
    }
    return res;
}
