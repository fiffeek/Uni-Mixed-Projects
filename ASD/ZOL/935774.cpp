#include <iostream>
#include <set>

using namespace std;

int main() {
    int n;
    cin >> n;

    set<int> s;

    for (int i = 1; i <= n; ++i) {
        s.insert(i);
    }

    for (int i = 0; i < n; ++i) {
        int aux;
        cin >> aux;

        auto found = s.find(aux);
        auto end_found = found;

        if (found == s.begin()) {
            cout << "-1 ";
        } else {
            found--;
            cout << *found << " ";
        }

        end_found++;

        if (end_found == s.end()) {
            cout << "-1" << endl;
        } else {
            cout << *end_found << endl;
        }

        s.erase(aux);
    }
}