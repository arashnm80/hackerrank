#include <cmath>
#include <cstdio>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>
#include <regex>
#include <fstream>
#include <iterator>

#include <filesystem>
#include <unistd.h>

using namespace std;

string search(string s, string r){
    smatch m;
    regex_search(s,m,regex(r));
    if(m.empty()){
        return "empty";
    }else{
        cout << m.size() << endl;
        return m[0].str();
    }
}

string func(string text, string address);
string newText(string text, string tag);
string getValue(string text, string attribute);


int main() {
    // read data
    ifstream input("2.txt");
    int n,q;
    input >> n >> q >> ws;
    string text;
    text = string(istreambuf_iterator<char>{input}, {});
    input.close();

    // process
    int i;
    string queries = text;
    for(i = 0; i < n; i++){
        // cout << queries.find('\n') << endl;
        queries = queries.substr(queries.find('\n') + 1);
    }
    queries += '\n';
    for(i = 0; i < q; i++){
        cout << func(text,queries.substr(0,queries.find('\n'))) << endl;
        queries = queries.substr(queries.find('\n') + 1);
    }
}

string func(string text, string address){
    string tag, attribute;
    int i = address.find('.');
    if(-1 != i){
        tag = address.substr(0, i);
        address = address.substr(i + 1);
        text = newText(text, tag);
        cout << "\ndebug:" << endl;
        cout << "tag: " << tag << endl;
        cout << "address: " << address << endl;
        cout << "text: " << text << endl;
        return func(text, address);
    }else{
        i = address.find('~');
        tag = address.substr(0, i);
        attribute = address.substr(i + 1);
        text = newText(text, tag);
        cout << "\ndebug:" << endl;
        cout << "tag: " << tag << endl;
        cout << "address: " << address << endl;
        cout << "attribute: " << attribute << endl;
        cout << "text: " << text << endl;
        string firstLine = text.substr(0, text.find('\n'));
        return getValue(firstLine, attribute);
    }
}

string newText(string text, string tag){
    int offset = 2;
    int find1, find2;
    find1 = text.find("<" + tag);
    text = text.substr(find1);
    find2 = text.substr(offset).find(tag + ">");
    text = text.substr(0, find2 + tag.size() + 1 + offset);
    return text;
}

string getValue(string text, string attribute){
    // test start
        string name, value;
        text.pop_back();
        int j = text.find(" ");
        if(-1 != j){
            text = text.substr(j + 1);
        }else{
            return "Not Found!";
        }
        // cout << "text till here: " << text << endl;
        while(j != -1){
            j = text.find(" = ");
            name = text.substr(0, j);
            value = text.substr(text.find("\"") + 1);
            value = value.substr(0, value.find("\""));
            if(name == attribute){
                return value;
            }else{
                text = text.substr(text.find("\"") + 1);
                if(text.size() == text.find("\"") + 1){
                    return "Not Found!";
                }
                text = text.substr(text.find("\"") + 2);
                // cout << "next search: " << text << endl;
            }
            j = text.find(" = ");
        }
        return "fuck";
    // test end


    // int find;
    // find = text.find(attribute);
    // if(-1 == find){
    //     return "Not Found!";
    // }
    // text = text.substr(find + attribute.size() + 4);
    // // cout << "inside getValue: " << text << endl;
    // find = text.find('\"');
    // text = text.substr(0, find);
    // // cout << "inside getValue: " << text << endl;
    // return text;
}