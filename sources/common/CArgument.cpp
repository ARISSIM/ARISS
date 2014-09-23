#include "include/CArgument.h"
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <stdlib.h>

CArgument::CArgument() {
}

std::vector<int> CArgument::split_arg(std::string Arg) {
    std::vector<int> vposition;
    std::vector<std::string> v_str;
    std::vector<int> varg;
    int pos = 0;
    int ag;

    for (unsigned int i = 0; i < Arg.length(); i++) //recherche des positions des espaces
    {
        if (Arg[i] == ' ') {
            vposition.push_back(i);
        }
    }

    vposition.push_back(Arg.length() + 1); // ajoute espace après le dernier mot

    v_str.clear();
    std::vector<std::string>(v_str).swap(v_str);


    for (unsigned int i = 0; i < vposition.size(); i++) {
        if (pos == 0) {
            v_str.push_back(Arg.substr(pos, vposition[i] - pos));
        } else {
            v_str.push_back(Arg.substr(pos + 1, vposition[i]-(pos + 1))); //position de depart et nombre de caracteres à récupérer
        }
        pos = vposition[i];
    }

    for (unsigned j = 0; j < v_str.size(); j++) {
        ag = atoi((v_str[j]).c_str());
        varg.push_back(ag);
    }

    return (varg);
}

