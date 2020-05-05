#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

const string FALSE = "false";
const string TRUE = "true";

string parsingDigit(char s, const string &dig);
string digitFalse(char s, const string &dig, bool isBlock);
string digitTrue(char s, const string &dig, bool isBlock);
void parsingAssigment(char s, bool isBlock);
void isVariable();
void isConst();

ofstream fileWrite("output.txt");
ifstream fileRead;

enum Conditions {
    Normal,
    Assigment,
    Indicator,
    Const,
    Digit,
    Implementation,
} thisCondition;

class Variable{
public:
    string name;
    string value;
    Variable() {
        name = string();
        value  = string();
    }
    Variable& operator =(Variable el) {
        name = el.name;
        value = el.value;
        return *this;
    }
};

class TableVariable {
public:
    int count = 0;
    Variable *variables = NULL;
    void Add(Variable &variable) {
        if (variables == NULL) {
            count++;
            variables = new Variable[1];
            variables[count - 1] = variable;
            return;
        }
        if (IsDuplicate(variable)) {
            (Get(variable.name))->value = variable.value;
        } else {
            count++;
            Variable* newArr = new Variable[count];
            for (int i = 0; i < count - 1; i++) {
                newArr[i] = variables[i];
            }
            newArr[count - 1] = variable;
            variables = newArr;
        }

    }
    bool IsDuplicate(const Variable& variable) const {
        for (int i = 0; i < count; i++) {
            if (variables[i].name == variable.name) {
                return true;
            }
        }
        return false;
    }
    Variable* Get(string name) {
        for (int i = 0; i < count; i++){
            if (variables[i].name == name)
                return &(variables[i]);
        }
        return NULL;
    }
};

int main(int argc, char*argv[]) {
    string fileName = argv[1];
    char symbol;
    string digit = "";
    /*Variable element, element2;
    element.name = "a";
    element.value ="true";
    element2.name = "b";
    element2.value = "true";
    TableVariable table;
    table.Add(element);
    table.Add(element2);
    cout << table.count << endl << table.Get("a")->value << endl;
    element.value = "false";
    table.Add(element);
    cout << table.count << endl << table.Get("a")->value << endl;*/
    fileRead = ifstream(fileName);
    fileWrite << "Begin parsing." << endl;
    while (!fileRead.eof()) {
        fileRead.get(symbol);
        if (fileRead.eof()) break;
        switch (thisCondition) {
            case Normal: {
                if (symbol == '(') {
                    thisCondition = Assigment;
                }
                break;
            }
            case Assigment: {

                break;
            }
            case Indicator: {
                break;
            }
            case Const: {
                break;
            }
            case Digit: {
                break;
            }
            case Implementation: {
                break;
            }
        }
        /*if (symbol == 'f') {
            cout << isdigitFalse(symbol, digit, false) << endl;
            continue;
        }
        if (symbol == 't') {
            cout << isdigitTrue(symbol, digit, false) << endl;
            continue;
        }*/

    }
    fileWrite << "End parsing." << endl;
    fileWrite.close();
    return 0;
}

string digitFalse(char s, const string &dig, bool isBlock) {
    string res = dig;
    if (FALSE.find(res + s) == string::npos || FALSE.find(res + s) > 0)
        isBlock = true;
    if (isBlock) {
        res += s;
        while (isalnum(s) && !fileRead.eof()) {
            fileRead.get(s);
            if (fileRead.eof()) break;
            res += s;
        }
        cout << "not digit: " << res;
        return "none";
    }
    if (FALSE.find(res + s) == 0 && !(FALSE == res + s)) {
        res += s;
        fileRead.get(s);
        return digitFalse(s, res, isBlock);
    }
    if (FALSE.find(res + s) == 0 && FALSE == res + s) {
        res += s;
        cout << "digit: " << res << endl;
        return res;
    }

    return "none";
}

string digitTrue(char s, const string &dig, bool isBlock) {
    string res = dig;
    if (TRUE.find(res + s) == string::npos || TRUE.find(res + s) > 0)
        isBlock = true;
    if (isBlock) {
        res += s;
        while (isalnum(s) && !fileRead.eof()) {
            fileRead.get(s);
            if (fileRead.eof()) break;
            res += s;
        }
        cout << "not digit: " << res;
        return "none";
    }
    if (TRUE.find(res + s) == 0 && !(TRUE == res + s)) {
        res += s;
        fileRead.get(s);
        return digitTrue(s, res, isBlock);
    }
    if (TRUE.find(res + s) == 0 && TRUE == res + s) {
        res += s;
        cout << "digit: " << res << endl;
        return res;
    }

    return "none";
}
//Разбор присваивания
void parsingAssigment(char s, bool isBlock) {

}