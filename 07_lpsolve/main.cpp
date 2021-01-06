#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include<cassert>
#include<vector>
#include "lp_lib.h"

std::string get_edge_name(int i);
std::vector<std::string> sort(std::vector<std::string>);

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
    REAL objective[32] = {12, 2, 3, 12, 3, 1, 15, 2, 4, 3, 3, 4, 
        3, 2, 6, 1, 3, 4, 2, 2, 2, 2, 2, 4, 6, 2, 3, 15, 2, 10, 3, 10};
    int helper0[32] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,
        23,24,25,26,27,28,29,30,31,32};
    set_obj_fnex(lp, 32, objective, helper0);
    //now we tell we want to minimize
    set_minim(lp);
    //first constraint: no incoming edges from A
    REAL row1[3];
    row1[0] = 1.0; //B->A
    row1[1] = 1.0; //C->A
    row1[2] = 1.0; //D->A
    int helper1[3] = {4,8,10};
    if (!add_constraintex(lp, 3, row1, helper1, EQ, 0.0)) {
        std::cerr << "Hilfe\n";
    }
    //second constraint: exactly one outgoing edge from A
    REAL row2[3];
    row2[0] = 1.0; //A->B
    row2[1] = 1.0; //A->C
    row2[2] = 1.0; //A->D
    int helper2[3] = {1,2,3};
    if (!add_constraintex(lp, 3, row2, helper2, EQ, 1.0)) {
        std::cerr << "oh nein\n";
    }
    //third constraint: exactly one incoming edge from M
    REAL row3[2] = {1.0, 1.0};
    int helper3[2] = {27,30};
    add_constraintex(lp, 2, row3, helper3, EQ, 1.0);
    //fourth constraint: for all B....L #incoming edges = #outgoing edges
    //B
    REAL rowB[8] = {1.0, 1.0, 1.0, 1.0, -1.0, -1.0, -1.0, -1.0};
    int helperB[8] = {4, 5, 6, 7, 1, 13, 16, 28}; 
    add_constraintex(lp, 8, rowB, helperB, EQ, 0.0);
    //C
    REAL rowC[33];
    for (int i = 0; i < 33; i++) {
        rowC[i] = 0;
    }
    //outgoing edges
    rowC[8] = 1; //C->A
    rowC[9] = 1; //C->J
    //incoming edges
    rowC[2] = -1; //A->C
    rowC[24] = -1; //J->C
    add_constraint(lp, rowC, EQ, 0);
    //D
    REAL rowD[33];
    for (int i = 0; i < 33; i++) {
        rowD[i] = 0;
    }
    //outgoing edges
    rowD[10] = 1; //D->A
    rowD[11] = 1; //D->F
    rowD[12] = 1; //D->G
    //incoming edges
    rowD[3] = -1; //A->D
    rowD[17] = -1; //F->D
    rowD[18] = -1; //G->D
    add_constraint(lp, rowD, EQ, 0);
    //E
    REAL rowE[33];
    for (int i = 0; i < 33; i++) {
        rowE[i] = 0;
    }
    //outgoing edges
    rowE[13] = 1; //E->B
    rowE[14] = 1; //E->I
    rowE[15] = 1; //E->J
    //incoming edges
    rowE[5] = -1; //B->E
    rowE[22] = -1; //I->E
    rowE[25] = -1; //J->E
    add_constraint(lp, rowE, EQ, 0);
    //F
    REAL rowF[33];
    for (int i = 0; i < 33; i++) {
        rowF[i] = 0;
    }
    //outgoing edges
    rowF[16] = 1; //F->B
    rowF[17] = 1; //F->D
    //incoming edges
    rowF[6] = -1; //B->F
    rowF[11] = -1; //D->F
    add_constraint(lp, rowF, EQ, 0);
    //G
    REAL rowG[33];
    for (int i = 0; i < 33; i++) {
        rowG[i] = 0;
    }
    //outgoing edges
    rowG[18] = 1; //G->D
    rowG[19] = 1; //G->H
    //incoming edges
    rowG[12] = -1; //D->G
    rowG[20] = -1; //H->G
    add_constraint(lp, rowG, EQ, 0);
    //H
    REAL rowH[33];
    for (int i = 0; i < 33; i++) {
        rowH[i] = 0;
    }
    //outgoing edges
    rowH[20] = 1; //H->G
    rowH[21] = 1; //H->I
    //incoming edges
    rowH[19] = -1; //G->H
    rowH[23] = -1; //I->H
    add_constraint(lp, rowH, EQ, 0);
    //I
    REAL rowI[33];
    for (int i = 0; i < 33; i++) {
        rowI[i] = 0;
    }
    //outgoing edges
    rowI[22] = 1; //I->E
    rowI[23] = 1; //I->H
    //incoming edges
    rowI[14] = -1; //E->I
    rowI[21] = -1; //H->I
    add_constraint(lp, rowI, EQ, 0);
    //J
    REAL rowJ[33];
    for (int i = 0; i < 33; i++) {
        rowJ[i] = 0;
    }
    //outgoing edges
    rowJ[24] = 1; //J->C
    rowJ[25] = 1; //J->E
    //incoming edges
    rowJ[9] = -1; //C->J
    rowJ[15] = -1; //E->J
    add_constraint(lp, rowJ, EQ, 0);
    //K
    REAL rowK[33];
    for (int i = 0; i < 33; i++) {
        rowK[i] = 0;
    }
    //outgoing edges
    rowK[26] = 1; //K->L
    rowK[27] = 1; //K->M
    //incoming edges
    rowK[29] = -1; //L->K
    rowK[31] = -1; //M->K
    add_constraint(lp, rowK, EQ, 0);
    //L
    REAL rowL[33];
    for (int i = 0; i < 33; i++) {
        rowL[i] = 0;
    }
    //outgoing edges
    rowL[28] = 1; //L->B
    rowL[29] = 1; //L->K
    rowL[30] = 1; //L->M
    //incoming edges
    rowL[7] = -1; //B->L
    rowL[26] = -1; //K->L
    rowL[32] = -1; //M->L
    add_constraint(lp, rowL, EQ, 0);
    //all our variables must be binary
    for (int i = 1; i <= 32; i++) {
        set_binary(lp, i, 1);
    }
    if (!solve(lp)) {
        // we have a solution now we want to now it
        REAL solution[32];
        get_variables(lp, solution);
        std::vector<std::string> erg;
        for (int i = 0; i < 32; i++) {
            if (solution[i]) {
                erg.push_back(get_edge_name(i+1));
            }
        }
        erg = sort(erg);
        std::cout << "computed path: \n";
        for (std::vector<std::string>::iterator iter = erg.begin(); iter != erg.end(); iter++) {
            std::cout << *iter;
        }
        std::cout << std::endl;
        delete_lp(lp);
    return 0;
    } else {
        std::cerr << "No optimal solution found\n";
        delete_lp(lp);
        return 1;
    }
}


std::string get_edge_name(int i) {
    std::string res = "fehler ";
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

std::vector<std::string> sort(std::vector<std::string> vector) {
    char start = 'A';
    std::vector<std::string> erg;
    while (start != 'M') {
        std::vector<std::string>::iterator iter = vector.begin();
        for (; iter != vector.end(); iter++) {
            std::string actual = *iter;
            if (actual.at(0) == start) {
                start = actual.at(3);
                erg.push_back(actual);
                break;
            }
        }
    }
    return erg;
}
