#include "CConfig.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

CConfig::CConfig() {

}

void CConfig::read_process(std::vector<CPartition> &vpart, std::string filepath) {
    //reading config.txt
    std::ifstream file(filepath.c_str());
    std::string line;
    std::string word, word2;

    std::vector<int> vposition;
    std::vector<std::string> tab_words;
    int time; //number which will contain the result

    if (file) {
        //opening, ok, let's parse
        std::cout << "read file config" << std::endl;


        while (getline(file, line)) //reading line by line
        {
            if (line[0] != '%') //every line that begin with % is considered as commented
            {
                vposition.clear();
                std::vector<int>(vposition).swap(vposition);

                tab_words.clear();
                std::vector<std::string>(tab_words).swap(tab_words);
                vwords(tab_words, line); //fill a word vector from a string line	
                time = atoi(tab_words[2].c_str());

                //if ( !(convert >> time) )					//give the value to Result using the characters in the string
                if (!time) {
                    time = 0; //if that fails set Result to 0
                }
                CPartition new_part(tab_words[0], tab_words[1], time); //enregistrement caractéristiques de la ligne dans l'objet partition
                vpart.push_back(new_part); //write partition vector
            } //end if condition
        } //end while loop
        file.close();
    }
    else {
        std::cout << "ERROR: impossible to open file" << std::endl;
    }
}

void CConfig::read_communication(std::vector<CPartition> &vpart, std::string filepath) {
    //lecture du fichier communication.txt
    std::string line;
    std::ifstream file(filepath.c_str());
    int wPartition, //num de la partition qui écrit (position dans le vecteur)
    rPartition, //num de la partition qui reçoit (position dans le vecteur)
    n1, n2,
    port;
    if (file) {
        while (getline(file, line)) {
            if (line[0] != '%') {
                std::vector<std::string> tab_words;
                tab_words.clear();
                vwords(tab_words, line); //remplit un vecteur de mots à partir d'une ligne type string



                std::istringstream iss1(tab_words[0]);
                std::istringstream iss2(tab_words[1]);

                iss1 >> n1;
                iss2 >> n2;
                wPartition = (n1) - 1;
                rPartition = (n2) - 1;

                if (tab_words[2] == "S") {
                    //save sampling port
                    std::istringstream iss(tab_words[3]);
                    iss >> port;
                    (vpart[wPartition]).wSport_add(port);
                    (vpart[rPartition]).rSport_add(port);
                } else if (tab_words[2] == "Q") {
                    //save queuing port

                    std::istringstream iss(tab_words[3]);
                    iss >> port;
                    (vpart[wPartition]).wQport_add(port);
                    (vpart[rPartition]).rQport_add(port);
                }

            }
        }
        file.close();
    } else {
        std::cout << "ERROR: impossible to open file" << std::endl;
    }

}

void CConfig::vwords(std::vector<std::string> &vwords, std::string line) {
    std::vector<int> vposition; //fill a word vector from a string line	
    int pos = 0;

    vposition.clear();

    for (unsigned int i = 0; i < line.length(); i++) //search for space character
    {
        if (line[i] == ' ') {
            vposition.push_back(i);
        }
    }
    vposition.push_back(line.length() + 1); //add a sapce after the last word



    for (unsigned int i = 0; i < vposition.size(); i++) {
        if (pos == 0) {
            vwords.push_back(line.substr(pos, vposition[i] - pos));
        } else {
            vwords.push_back(line.substr(pos + 1, vposition[i]-(pos + 1))); //position de depart et nombre de caracteres à récupére
        }
        pos = vposition[i];
    }

}











