#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

const string FALSE = "false";
const string TRUE = "true";

bool StrToBool(string value);
string GetInversionValue(string name);
string parsingAssigment(char *s, string &operand);
string parsingVariable(char *s, string &operand);
string parsingInversion(char *s, string &operand);
string parsingOr(char *s, string &operand, string left);
string parsingAnd(char *s, string &operand, string left);

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
    }
    fileWrite << "End parsing." << endl;
    fileWrite << Indicators.count << " variables defined:" << endl;
    for (int i = 0; i < Indicators.count; i++) {
        fileWrite << Indicators.variables[i].name << " = " << Indicators.variables[i].value << endl;
    }
    fileWrite.close();
    return 0;
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
            switch (*s) {
                case '|': {
                    value = parsingOr(s, operand, value);
                    break;
                }
                case '&': {
                    value = parsingAnd(s, operand, value);
                    break;
                }
            }

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
            switch (*s) {
                case '|': {
                    value = parsingOr(s, operand, value);
                    break;
                }
                case '&': {
                    value = parsingAnd(s, operand, value);
                    break;
                }
            }
            Indicators.Add(*(new Variable(nameVariable, value)));
            cout << nameVariable << " " << Indicators.Get(nameVariable)->value << endl;
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
        switch (*s) {
            case '|': {
                value = parsingOr(s, operand, GetInversionValue(value));
                break;
            }
            case '&': {
                value = parsingAnd(s, operand, GetInversionValue(value));
                break;
            }
        }
        return GetInversionValue(value);
    } /*else if (*s == '|') {
        value = parsingOr(s, operand, GetInversionValue(value));
    } else if (*s == '&') {
        value = parsingAnd(s, operand, GetInversionValue(value));*/
    if (*s == '(') {
        fileRead.get(*s);
        operand += *s;
        if (isalpha(*s)) {
            value = parsingVariable(s, operand);
            switch (*s) {
                case '|': {
                    value = parsingOr(s, operand, value);
                    break;
                }
                case '&': {
                    value = parsingAnd(s, operand, value);
                    break;
                }
            }
            return GetInversionValue(value);
        }
        if (*s == '~') {
            value = parsingInversion(s, operand);
            switch (*s) {
                case '|': {
                    value = parsingOr(s, operand, value);
                    break;
                }
                case '&': {
                    value = parsingAnd(s, operand, value);
                    break;
                }
            }
            if (*s == ')') {
                fileRead.get(*s);
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

string parsingOr(char *s, string &operand, string left) {
    bool leftValue = StrToBool(left);
    bool rightValue;
    string right = "";
    fileRead.get(*s);
    operand += *s;
    if (isalpha(*s)) {
        right = parsingVariable(s, operand);
        switch (*s) {
            case '|': {
                right = parsingOr(s, operand, right);
                break;
            }
            case '&': {
                right = parsingAnd(s, operand, right);
                break;
            }
            case '~': {
                right = parsingInversion(s, operand);
                break;
            }
        }
        rightValue = StrToBool(right);
        if (rightValue | leftValue)
            return "true";
        else
            return "false";
    } else if (*s == '~') {
        right = parsingInversion(s, operand);
        switch (*s) {
            case '|': {
                right = parsingOr(s, operand, right);
                break;
            }
            case '&': {
                right = parsingAnd(s, operand, right);
                break;
            }
        }
        rightValue = StrToBool(right);
        if (rightValue & leftValue)
            return "true";
        else
            return "false";
    }
}
string parsingAnd(char *s, string &operand, string left) {
    bool leftValue = StrToBool(left);
    bool rightValue;
    string right = "";
    fileRead.get(*s);
    operand += *s;
    if (isalpha(*s)) {
        right = parsingVariable(s, operand);
        switch (*s) {
            case '|': {
                right = parsingOr(s, operand, right);
                break;
            }
            case '&': {
                right = parsingAnd(s, operand, right);
                break;
            }
            case '~': {
                right = parsingInversion(s, operand);
                break;
            }
        }
        rightValue = StrToBool(right);
        if (rightValue & leftValue)
            return "true";
        else
            return "false";
    } else if (*s == '~') {
        right = parsingInversion(s, operand);
        switch (*s) {
            case '|': {
                right = parsingOr(s, operand, right);
                break;
            }
            case '&': {
                right = parsingAnd(s, operand, right);
                break;
            }
        }
        rightValue = StrToBool(right);
        if (rightValue & leftValue)
            return "true";
        else
            return "false";
    }
}

bool StrToBool(string value) {
    if (value != FALSE && value != TRUE) {
        Variable * med = Indicators.Get(value);
        if (med == nullptr) {
            fileWrite << "Error " << UsingNotDeclaredVariable << ": Using not declared variable." << endl << "Abort parsing." << endl;
            fileWrite.close();
            fileRead.close();
            exit(UsingNotDeclaredVariable);
        }
        return !(med->value == FALSE);
    } else {
        return !(value == FALSE);
    }
}

string GetInversionValue(string name) {
    if (name == FALSE) {
        return TRUE;
    } else if (name == TRUE) {
        return FALSE;
    } else {
        Variable* med = Indicators.Get(name);
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