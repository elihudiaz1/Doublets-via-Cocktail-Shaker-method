#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
using namespace std;


int main(int argc, char ** argv) {

    stringstream arg1converter(argv[1]);
    stringstream arg2converter(argv[2]);
    stringstream arg3converter(argv[3]);
    string argument1, argument2, argument3;
    if (!(arg1converter >> argument1))
        argument1 = "blank";
    if (!(arg2converter >> argument2))
        argument1 = "blank";
    if (!(arg3converter >> argument3))
        argument1 = "blank";

    struct Node {
        string word;
        Node *parent;
    };

    //READING IN FILE ELEMENTS OF N-SIZE INTO A LIST
    list<Node> s_active_list;
    list<Node> t_active_list;
    int wordsReadIn = 0;
    int wordsActive = 0;

    ifstream fin;
    fin.open(argv[1]);
    if (!fin) {
        cout << "No file has been opened";
        return false;
    }
    while (!fin.eof()) {
        Node nLetter;
        getline(fin, nLetter.word, '\n');
        wordsReadIn++;
        if (nLetter.word.size() == argument2.size()) {
            s_active_list.push_back(nLetter);
            t_active_list.push_back(nLetter);
            wordsActive++;
        }
    }
    vector<list<Node> > s_levels;
    vector<list<Node> > t_levels;
    map<string, Node *> s_found;
    map<string, Node *> t_found;
    list<Node> s_firstword;
    list<Node> t_firstword;
    Node s_starter;
    Node t_starter;

    Node centerWord;
    Node *s_endPtr;
    Node *t_endPtr;
    int s_index = 0;
    int t_index = 0;
    int flag = 1;
    s_starter.word = argument2;
    s_starter.parent = nullptr;
    t_starter.word = argument3;
    t_starter.parent = nullptr;
    s_firstword.push_back(s_starter);
    t_firstword.push_back(t_starter);
    s_levels.push_back(s_firstword);
    t_levels.push_back(t_firstword);
    s_index++;
    t_index++;

    do{
        list<Node> s_change;
        list<Node> t_change;
        for(auto & entry : s_levels[s_index-1])
        {
            for(int j = 0; j < argument2.size(); j++)
            {
                Node rep;
                rep.word = entry.word;
                rep.word.erase(j, 1);
                for(auto nodeItr = s_active_list.begin(); nodeItr != s_active_list.end() && flag == 1; nodeItr++)
                {
                    Node substr;
                    Node *mapPtr;
                    substr.word = nodeItr->word;
                    substr.word.erase(j, 1);
                    if(rep.word == substr.word)
                    {
                        Node n;
                        n.word = nodeItr->word;
                        n.parent = &entry;
                        mapPtr = &n;
                        s_active_list.erase(nodeItr++);
                        s_change.push_back(n);
                        if(t_found.find(n.word) != t_found.end() )
                        {
                            centerWord.word = n.word;
                            centerWord.parent = nullptr;
                            flag = 0;
                        } else {
                            s_found.insert({n.word, mapPtr});
                        }
                    }
                }
            }
        }
        s_levels.push_back(s_change);
        s_index++;
        if(flag != 0)
        {
            for(auto & entry : t_levels[t_index-1])
            {
                for(int j = 0; j < argument3.size(); j++)
                {
                    Node rep;
                    rep.word = entry.word;
                    rep.word.erase(j, 1);
                    for(auto nodeItr = t_active_list.begin(); nodeItr != t_active_list.end(); nodeItr++)
                    {
                        Node substr;
                        Node *mapPtr;
                        substr.word = nodeItr->word;
                        substr.word.erase(j, 1);
                        if(rep.word == substr.word)
                        {
                            Node n;
                            n.word = nodeItr->word;
                            n.parent = &entry;
                            mapPtr = &n;
                            t_active_list.erase(nodeItr++);
                            t_change.push_back(n);
                            if(s_found.find(n.word) != s_found.end())
                            {
                                centerWord.word = n.word;
                                centerWord.parent = nullptr;
                                flag = 0;
                            } else {
                                t_found.insert({n.word, mapPtr});
                            }
                        }
                    }
                }
            }
            t_levels.push_back(t_change);
            t_index++;
        }
    }while(flag==1);

    for(auto x : s_levels[s_levels.size()-1])
    {
        if(x.word == centerWord.word)
        {
            s_endPtr = &x;
            break;
        }
    }

    for(auto x : t_levels[t_levels.size()-1])
    {
        if(x.word == centerWord.word)
        {
            t_endPtr = &x;
            break;
        }
    }


    vector<string> s_printOut;
    vector<string> t_printOut;
    Node *s_currentPtr = s_endPtr;
    Node *t_currentPtr = t_endPtr;

    while(s_currentPtr != 0)
    {
        s_printOut.push_back(s_currentPtr->word);
        s_currentPtr = s_currentPtr->parent;
    }

    while(t_currentPtr != 0)
    {
        t_printOut.push_back(t_currentPtr->word);
        t_currentPtr = t_currentPtr->parent;
    }

    reverse(s_printOut.begin(), s_printOut.end());

    cout << wordsReadIn << " words read in, " << wordsActive << " are active!" << endl;


    cout << "Trying shaker search" << endl;
    cout << argument2 << " ";
    for(int i = 1; i < s_printOut.size(); i++)
    {
        cout << " --> " << "[" << i << "]: " << s_printOut[i];
    }

    for(int j = 1; j < t_printOut.size(); j++)
    {
        cout << " --> " << "[" << j << "]: " << t_printOut[j];
    }

    int count_s = 0;
    int count_t = 0;
    cout << "\n\n\n#####------#####" << endl;
    cout << "## Shaker tables data, source side:" << endl;
    for(int i = 0; i < s_levels.size(); i++)
    {
        cout << "## ... s_level " << i << " has " << s_levels[i].size() << " elements " << endl;
        count_s = count_s+s_levels[i].size();
    }
    cout << "## Total data items: " << count_s << endl;
    cout << "##\n##" << endl;
    cout << "## Shaker tables data, target side:" << endl;
    for(int i = 0; i < t_levels.size(); i++)
    {
        cout << "## ... t_level " << i << " has " << t_levels[i].size() << " elements " << endl;
        count_t = count_t+t_levels[i].size();
    }
    cout << "## Total data items: " << count_t << endl;
    cout << "#####------#####\n$" << endl;
    return 0;
}