#include <iostream>
#include <algorithm>
using namespace std;

#include "Matrix.h"

int main() {
    using  Matrix = matrix<int, 2, 3>;
    Matrix m;
    for (auto& e : m) {
        e = 0;
    }
    for (int row = 0; row < m.numRows(); ++row) {
        for (int column = 0; column < m.numCols(); ++column) {
            cout << m(row, column) << " ";
        }
        cout << endl;
    }
    for (auto i = m.begin(); i != m.end(); ++i) {
        cout << *i << " ";
    }
    cout << endl;
    for_each(m.begin(), m.end(), [](int& i){
        i += 1;
    });
    for_each(m.begin(), m.end(), [](const int& i){
        cout << i << " ";
    });
    cout << endl;
    for_each(m.cbegin(), m.cend(), [](const int& i){
        cout << i << " ";
    });
    cout << endl;

    const Matrix cm = m;
    for_each(cm.begin(), cm.end(), [](const int& i){
        cout << i << " ";
    });
    cout << endl;
    // Should give compiler error!
    // for_each(cm.begin(), cm.end(), [](int& i){
    //     i += 1;
    // });

    Matrix m2 = {1, 2, 3, 
                 4, 5, 6};
    for (auto e : m2) {
        cout << e << " ";
    }
    cout << endl;

    // Should give compiler error!
    // Matrix m2 = {{1, 2, 3, 
    //               4, 5, 6, 7}};

    Matrix m3 = {1, 2, 3, 
                 4, 5, 6};

    try {
        Matrix m4 = {1, 2, 3, 
                     4, 5, 6, 7};
    } catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
    }        
                
    cin.get();
    return 0;
}