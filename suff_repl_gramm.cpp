using namespace std;
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cstdio>
#include <vector>
#include <queue>
#include <deque>
#include <stack>
#include <list>
#include <map>
#include <set>

#define foreach(x, v) for (typeof (v).begin() x=(v).begin(); x !=(v).end(); ++x)



inline bool endsWith(const string &s, const string &t) 
{
	if (s.size() < t.size()) return false;
	for (int k = 0; k < t.size(); ++k) 
	{
		if ( s[s.size() - 1 - k] != t[t.size() - 1 - k] ) 
		{
			return false;
		}
}
	return true;
}

int main()
{
	map<string, vector< string > > g[256];
	int R;
	static char buf1[256], buf2[256];
	int cases = 1;
	while(scanf(" %s %s %d ", &buf1, &buf2, &R) == 3) 
	{
		string S(buf1), T(buf2);
		printf("Case %d: ", cases++);
		if (S.size() != T.size())
		{
			cout << "No solution" << endl;
			continue;
		}
		
		for(int k = 0; k < 256; k ++) g[k].clear();

		for (int i = 0; i < R; i++) 
		{
			string from, to; cin >> from >> to;
			g[from[from.size() - 1]][from].push_back( to );
		}
		map< string, int > been;
		queue< string > q;
		been[S] = 1;
		q.push( S );
		while (q.size() > 0) 
		{
			const string u = q.front(); q.pop();
			if (u == T) break;
			char last = u[u.size() - 1];
			foreach(out, g[last]) 
			{
				if (!endsWith(u, out->first)) continue;
				foreach(next, out->second) 
				{
					string v = u.substr(0, u.size() - next->size());
					v += *next;
					if (been.count(v) > 0) continue;
					been[v] = been[u] + 1;
					q.push( v );
				}
			}
		}
		if (been[T] > 0) 
		{
			printf("%d\n", been[T] - 1);
		} 
		else 
		{
			cout << "No solution" << endl;
		}
	}
return 0;
}