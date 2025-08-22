#include <bits/stdc++.h>
#include "json.hpp"
#include <fstream>

using namespace std;
using json = nlohmann::json;

long long decodeValue(const string &val, int base) {
    long long result = 0;
    for (char c : val) {
        int digit;
        if (isdigit(c)) digit = c - '0';
        else if (isalpha(c)) digit = 10 + (tolower(c) - 'a');
        else throw runtime_error("Invalid digit");
        if (digit >= base) throw runtime_error("Digit out of range for base");
        if (result > LLONG_MAX / base) throw runtime_error("Overflow decoding value");
        result = result * base + digit;
    }
    return result;
}

double lagrangeConstantTerm(const vector<pair<double, long long>> &points) {
    int n = points.size();
    double c = 0.0;
    for (int i = 0; i < n; i++) {
        double term = (double)points[i].second;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                term *= (0.0 - points[j].first) / (points[i].first - points[j].first);
            }
        }
        c += term;
    }
    return c;
}

int main() {
    ifstream file("input.json");
    if (!file.is_open()) {
        cerr << "Error: Could not open input.json" << endl;
        return 1;
    }

    json j;
    file >> j;

    int n = j["keys"]["n"];
    int k = j["keys"]["k"];

    vector<pair<double, long long>> points;
    for (auto &el : j.items()) {
        if (el.key() == "keys") continue;
        int x = stoi(el.key());
        int base = stoi(el.value()["base"].get<string>());
        string val = el.value()["value"];
        long long y = decodeValue(val, base);
        points.emplace_back((double)x, y);
    }

    sort(points.begin(), points.end(),
         [](auto &a, auto &b) { return a.first < b.first; });

    vector<pair<double, long long>> selected(points.begin(), points.begin() + k);

    double c = lagrangeConstantTerm(selected);

    cout << fixed << setprecision(0);
    cout << c << endl;

    return 0;
}
