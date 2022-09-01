#include <bits/stdc++.h>
using namespace std;



bool valid(vector<string> tags, vector<int> starts, vector<int> ends, vector<string> addresses);
string getAttrib(string tag, string attribName, vector<vector<string>> attribNames, vector<vector<string>> attribValuess);

int main(){
    ifstream in("2.txt");
    int n,q;
    in >> n >> q >> ws;
    vector<string> text(n);
    vector<string> queries(q);
    vector<vector<string>> attribNames (n / 2);
    vector<vector<string>> attribValues (n / 2);
    vector<string> tags (n / 2);
    vector<int> starts (n / 2);
    vector<int> ends (n / 2);
    for(int i = 0; i < n; i++){
        getline(in, text[i]);
    }
    for(int i = 0; i < q; i++){
        getline(in, queries[i]);
    }
    in.close();

    //process
    string line, tag, attribName, attribValue;
    int i, j, k;
    int index = 0, found;
    for(i = 0; i < n; i++){
        line = text[i];
        //check for ending tag
        if('/' == line[1]){
            tag = line.substr(2, line.size() - 3);
            found = 0;
            for(j = 0; j < tags.size(); j++){
                if(tags[j] == tag){
                    found = 1;
                    ends[j] = i;
                    break;
                }
            }
            if(0 == found){
                cout << "error on ending tag" << endl;
            }
        }else{
            // process tag
            tag.clear();
            for(j = 1; line[j] != ' ' && line[j] != '>'; j++){
                tag += line[j];
            }
            j++;
            tags[index] = tag;
            starts[index] = i;

            // process attributes:
            while(j < line.size() - 1){
                // attribName
                attribName.clear();
                while(' ' != line[j]){
                    attribName += line[j];
                    j++;
                }
                j += 4;

                // attribValue
                attribValue.clear();
                while('\"' != line[j]){
                    attribValue += line[j];
                    j++;
                }
                j += 2;

                // add to 2D vectors
                attribNames[index].push_back(attribName);
                attribValues[index].push_back(attribValue);
            }
            // change index at the end
            index++;
        }
    }

    // queries section
    bool flag;
    bool exitFlag;
    vector<string> addresses;
    int attribIndex;
    int attribFlag;
    for(i = 0; i < q; i++){
        cout << "---------------" << endl;

        // parse query
        addresses.clear();
        line = queries[i];
        attribName = line.substr(line.find('~') + 1);
        line = line.substr(0, line.find('~'));
        j = 0;
        tag.clear();
        while(j < line.size()){
            if('.' != line[j]){
                tag += line[j];
            }else{
                addresses.push_back(tag);
                tag.clear();
            }
            j++;
        }
        addresses.push_back(tag);

        // tag nesting validation
        attribIndex = -1;
        attribFlag = -1;
        exitFlag = 0;
        flag = valid(tags, starts, ends, addresses);
        if(flag){
            cout << "my attrib: " << attribName << endl;
            cout << i << ": true nesting" << endl;
            for(k = 0; k < tags.size(); k++){
                if(tag == tags[k]){
                    attribIndex = k;
                    break;
                }
            }
            if(-1 == attribIndex){
                cout << "error on attribIndex" << endl;
            }
            for(int z = 0; z < attribNames[attribIndex].size(); z++){
                if(attribNames[attribIndex][z] == attribName){
                    cout << attribValues[attribIndex][z] << endl;
                    exitFlag = 1;
                    break;
                }
            }
            if(!exitFlag){
                cout << "attribName " << attribName << " " << "Not Found!" << endl;
                cout << "Not Found!" << endl;
            }
        }else{
            cout << i << ": false nesting" << endl;
            cout << "Not Found!" << endl;
        }
    }

}

bool valid(vector<string> tags, vector<int> starts, vector<int> ends, vector<string> addresses){
    if(addresses.empty()){
        return true;
    }
    string tag = addresses[0];
    int index = -1;
    for(int i = 0; i < tags.size(); i++){
        if(tag == tags[i]){
            index = i;
            break;
        }
    }
    if(-1 == index){
        cout << "error: index in valid function is -1" << endl;
        return false;
    }
    for(int i = 0; i < tags.size(); i++){
        if(starts[i] < starts[index] && ends[i] > ends[index]){
            cout << "wrong pattern. tag " << tags[i] << " is around " << tags[index] << endl;
            return false;
        }
    }
    if(addresses.size() > 1){
        string tag2 = addresses[1];
        int index2 = -1;
        for(int i = 0; i < tags.size(); i++){
            if(tag2 == tags[i]){
                index2 = i;
                break;
            }
        }
        if(-1 == index2){
            cout << "error: index2 in valid function is -1" << endl;
            return false;
        }
        for(int i = 0; i < tags.size(); i++){
            if(starts[index2] < starts[index] || ends[index2] > ends[index]){
                cout << "wrong pattern. tag " << tags[index2] << " doesn't exist in " << tags[index] << endl;
                return false;
            }
        }
    }
    tags.erase(tags.begin() + index);
    starts.erase(starts.begin() + index);
    ends.erase(ends.begin() + index);
    addresses.erase(addresses.begin());
    return valid(tags, starts, ends, addresses);
}