#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

const string FALSE = "false";
const string TRUE = "true";

bool ProcS(char *s, string &operand);
bool ProcE(char *s, string &operand);
bool ProcT(char *s, string &operand);
bool ProcM(char *s, string &operand);
bool ProcI(char *s, string &operand);
/*bool ProcC(void);*/
string ProcL(char *s, string &operand);
void Get(char *s, string &operand);
void Error(const char * msg, const char * param);

ofstream fileWrite("output.txt");
ifstream fileRead;

class Variable{
public:
    string name;
    bool value;
    Variable() {
        name = string();
        value;
    }
    Variable(string n, bool v) {
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
        Error("Identifier missing.", 0);
        return NULL;
    }
};

TableVariable Indicators;

void Error(const char * msg, const char * param)
{
    fileWrite << "Error: ";        // вместо этой функции можно использовать
    fileWrite << msg << param << endl;       // собственный класс исключения, объект
    fileRead.close();                // описание ошибки, ее код и т.п.,
    fileWrite.close();
    system("pause");          // а на верхнем уровне его обрабатывать
    exit(7);
}

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
        if (symbol == '(') {
            operand += symbol;
            ProcS(&symbol, operand);
            fileWrite << "Operator " << ++amountOperators << " : " << operand << endl;
            operand = "";
        }
    }
    fileWrite << "End parsing." << endl;
    fileWrite << Indicators.count << " variables defined:" << endl;
    for (int i = 0; i < Indicators.count; i++) {
        string value = Indicators.variables[i].value == true ? "true" : "false";
        fileWrite << Indicators.variables[i].name << " = " <<  value << endl;
    }
    fileWrite.close();
    return 0;
}

void Get(char *s, string &operand) {
    fileRead.get(*s);
    operand += *s;
}

bool ProcS(char *s, string &operand) {
    string name = ProcL(s, operand);
    if (*s != ',')
        Error("\',\' missing.", 0);
    Get(s, operand);
    bool value = ProcE(s, operand);
    if (*s != ')')
        Error("\')\' missing.", 0);
    Indicators.Add(*(new Variable(name, value)));
    Get(s, operand);
}
bool ProcE(char *s, string &operand) {
    bool x = ProcT(s, operand);
    while (*s == '|') {
        Get(s, operand);
        x &= ProcT(s, operand);
    }
    return x;
}
bool ProcT(char *s, string &operand) {
    bool x = ProcM(s, operand);
    while (*s == '&') {
        Get(s, operand);
        x &= ProcM(s, operand);
    }
    return x;
}
bool ProcM(char *s, string &operand) {
    bool x;
    if (*s == '(') {
        Get(s, operand);
        x = ProcE(s, operand);
        if (*s != ')')
            Error("\')\' missing.", 0);
        Get(s, operand);
    } else if (*s == '~') {
        Get(s, operand);
        x = !ProcM(s, operand);
    } else if (*s >= 'a' && *s <= 'z')
        x = ProcI(s, operand);
    else
        Error("Syntax error.", 0);

    return x;
}
bool ProcI(char *s, string &operand) {
    string x = "";

    while ((*s >= 'a' && *s <= 'z')) {
        x += *s;
        Get(s, operand);
    }
    if (x == TRUE)
        return true;
    if (x == FALSE)
        return false;
    return Indicators.Get(x)->value;
}
/*bool ProcC(void);*/
string ProcL(char *s, string &operand) {
    string x = "";
    Get(s, operand);
    while ((*s >= 'a' && *s <= 'z')) {
        x += *s;
        Get(s, operand);
    }
    if (x == TRUE || x == FALSE)
        Error("Unexpected literal.", 0);
    return x;
}