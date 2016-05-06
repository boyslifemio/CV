/*
#include <stdio.h>
#include <iostream>

using namespace std;

class mystr {
private:
    const char *str;

public:
    mystr(const char *s) {
        str = s;
    }

    void printn() {
        printf("%s\n", str);
    }
};

int main() {
	char test[16];
	cin >> test;
    mystr s = test;
    s.printn();
}
*/

#include <stdio.h>
#include <iostream>

using namespace std;

class mystr {
private:
    string str;

public:
    mystr(string s) {
        str = s;
    }

    void printn() {
        cout << str << endl;
    }
};

int main() {
	string test;
	cin >> test;
    mystr tmp = test;
    tmp.printn();
}