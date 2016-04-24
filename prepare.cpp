#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char **argv)
{
    const std::string funcname("GetAction");
    const std::string tname("AntAction");
    if (argc < 3) {
        std::cout << "<3" << std::endl;
        return 0;
    }
    std::fstream fin(argv[1]);
    if (!fin.is_open()) {
        std::cout << "no file" << std::endl;
        return 0;
    }
    std::string text;
    std::string tmp;
    while (getline(fin, tmp)) {
        text += tmp + "\n";
    }
    fin.close();
    for (int i = 0; i < (int)text.size() - (int)funcname.size(); i++) {
        if (text.substr(i, funcname.size()) == funcname) {
            int j = i - 1;
            while (j >= 0 && (text[j] == ' ' || text[j] == '\n' || text[j] == '\t')) {
                j--;
            }
            while (j >= 0 && ((text[j] >= 'a' && text[j] <= 'z') || (text[j] >= 'A' && text[j] <= 'Z'))) {
                j--;
            }
            if (j < 0) {
                continue;
            }
            text = text.substr(0, j + 1) + "extern \"C\" " + text.substr(j + 1);
            break;
        }
    }

    std::ofstream fout(argv[2]);
    if (!fout.is_open()) {
        std::cout << "bad" << std::endl;
        return 0;
    }
    fout << text;
    fout.close();
}
