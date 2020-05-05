#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

const string FALSE = "false";
const string TRUE = "true";

string parsingDigit(char *s, const string &dig);
string digitFalse(char *s, const string &dig, bool isBlock);
string digitTrue(char *s, const string &dig, bool isBlock);
string parsingAssigment(char *s, string &operand);
string parsingVariable(char *s, string &operand);
string parsingInversion(char *s, string &operand);

ofstream fileWrite("output.txt");
ifstream fileRead;

enum Errors {
    None,
    IncorrectAssigment,
    UsingNotDeclaredVariable,
    IncorrectInversion,
    IncorrectNameVariable,
};

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
    Variable(string n, string v) {
        name = n;
        value  = v;
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

TableVariable Indicators;

int main(int argc, char*argv[]) {
    string fileName = argv[1];
    int amountOperators = 0;
    char symbol;
    string operand = "";
    fileRead = ifstream(fileName);
    fileWrite << "Begin parsing." << endl;
    while (!fileRead.eof()) {
        fileRead.get(symbol);
        if (fileRead.eof()) break;
        switch (thisCondition) {
            case Normal: {
                if (symbol == '(') {
                    thisCondition = Assigment;
                    operand += symbol;
                    string res = parsingAssigment(&symbol, operand);
                    fileWrite << "Operator " << ++amountOperators << " : " << operand << endl;
                    operand = "";
                    thisCondition = Normal;
                }
                if (symbol == '~') {

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
    fileWrite << Indicators.count << " variables defined:" << endl;
    for (int i = 0; i < Indicators.count; i++) {
        fileWrite << Indicators.variables[i].name << " = " << Indicators.variables[i].value << endl;
    }
    fileWrite.close();
    return 0;
}

string digitFalse(char *s, const string &dig, bool isBlock) {
    string res = dig;
    if (FALSE.find(res + *s) == string::npos || FALSE.find(res + *s) > 0)
        isBlock = true;
    if (isBlock) {
        res += *s;
        while (isalpha(*s) && !fileRead.eof()) {
            fileRead.get(*s);
            if (fileRead.eof()) break;
            res += *s;
        }
        cout << "not digit: " << res;
        return res;
    }
    if (FALSE.find(res + *s) == 0 && !(FALSE == res + *s)) {
        res += s;
        fileRead.get(*s);
        return digitFalse(s, res, isBlock);
    }
    if (FALSE.find(res + *s) == 0 && FALSE == res + *s) {
        res += *s;
        cout << "digit: " << res << endl;
        return res;
    }

    return res;
}

string digitTrue(char *s, const string &dig, bool isBlock) {
    string res = dig;
    if (TRUE.find(res + *s) == string::npos || TRUE.find(res + *s) > 0)
        isBlock = true;

    if (TRUE.find(res + *s) == 0 && !(TRUE == res + *s)) {
        res += *s;
        fileRead.get(*s);
        return digitTrue(s, res, isBlock);
    }
    if (TRUE.find(res + *s) == 0 && TRUE == res + *s) {
        res += *s;
        cout << "digit: " << res << endl;
        return res;
    }

    return res;
}
//Разбор присваивания
string parsingAssigment(char *s, string &operand) {
    string res = "";
    string nameVariable = "";
    string value = "";
    bool isEndPars = false;
    fileRead.get(*s);
    operand += *s;
    if (isalpha(*s)) {
        nameVariable = parsingVariable(s, operand);
        if (nameVariable == TRUE || nameVariable == FALSE || *s != ',') {
            fileWrite << "Error " << IncorrectAssigment << ": Incorrect form of assigment." << endl << "Abort parsing." << endl;
            fileWrite.close();
            fileRead.close();
            exit(IncorrectAssigment);
        }
        fileRead.get(*s);
        operand += *s;
        if (isalpha(*s)) {
            value = parsingVariable(s, operand);
            if (value == FALSE) {
                Indicators.Add(*(new Variable(nameVariable, "false")));
            } else if (value == TRUE) {
                Indicators.Add(*(new Variable(nameVariable, "true")));
            } else {
                Variable* med = Indicators.Get(value);
                if (med == nullptr) {
                    fileWrite << "Error " << UsingNotDeclaredVariable << ": Using not declared variable." << endl << "Abort parsing." << endl;
                    fileWrite.close();
                    fileRead.close();
                    exit(UsingNotDeclaredVariable);
                }
                Indicators.Add(*(new Variable(nameVariable, med->value)));
            }
            if (*s != ')') {
                fileWrite << "Error " << IncorrectAssigment << ": Incorrect form of assigment." << endl << "Abort parsing." << endl;
                fileWrite.close();
                fileRead.close();
                exit(IncorrectAssigment);
            }
            cout << nameVariable << " " << Indicators.Get(nameVariable)->value << endl;
            res = "(" + nameVariable + "," + value + ")";
            return res;
        }
        if (*s == '~') {
            string med = parsingInversion(s, operand);
            if (med == "none") {
                fileWrite << "Error " << IncorrectInversion << ": Incorrect struct of inversion." << endl << "Abort parsing." << endl;
                fileWrite.close();
                fileRead.close();
                exit(IncorrectInversion);
            }
            value = med;
            Indicators.Add(*(new Variable(nameVariable, value)));
            res = "(" + nameVariable + "," + value + ")";
            return res;
        }
        return "none";
    }

    /*while (!fileRead.eof() && !isEndPars) {

    }*/
}
//Разбор имени переменной
string parsingVariable(char *s, string &operand) {
    string res = "";
    res = *s;
    while (!fileRead.eof() && isalpha(*s)) {
        fileRead.get(*s);
        operand += *s;
        if (fileRead.eof()) break;
        if (!isalpha(*s))
            break;
        res += *s;
    }
    return res;
}

string parsingInversion(char *s, string &operand) {
    string value = "";
    fileRead.get(*s);
    operand += *s;
    if (isalpha(*s)) {
        value = parsingVariable(s, operand);
        if (value == FALSE) {
            return TRUE;
        } else if (value == TRUE) {
            return FALSE;
        } else {
            Variable* med = Indicators.Get(value);
            if (med == nullptr) {
                fileWrite << "Error " << UsingNotDeclaredVariable << ": Using not declared variable." << endl << "Abort parsing." << endl;
                fileWrite.close();
                fileRead.close();
                exit(UsingNotDeclaredVariable);
            }
            if (med->value == FALSE)
                return TRUE;
            else
                return FALSE;
        }
    }
    if (*s == '(') {
        fileRead.get(*s);
        operand += *s;
        if (*s == '~') {
            value = parsingInversion(s, operand);
            if (*s == ')') {
                operand += *s;
                if (value == FALSE)
                    return TRUE;
                else if (value == TRUE)
                    return FALSE;
                else
                    return "none";
            }
        }
    }
    return "none";
}