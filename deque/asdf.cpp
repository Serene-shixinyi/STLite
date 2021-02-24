#include<bits/stdc++.h>
using namespace std;

int main() {
	deque<int> a, b;
	a.push_back(1); b.push_back(2);
	b.erase(a.end());
	return 0;
}