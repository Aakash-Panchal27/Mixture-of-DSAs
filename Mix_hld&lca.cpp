#include<bits/stdc++.h>
#include<iostream>
#include<string>
#include<vector>
#include<ctime>
#include<iomanip>
#include<fstream>
#include<stack>
#include<queue>
#include<cassert>
#include<sstream>
#include<algorithm>
#include<unordered_map>
#include<map>
#include<functional>
#include<exception>
#include<utility>
#define forr(i,z,u,l) for(i=z;i<u;i+=l)
#define f(i,n) for(i=0;i<n;i++)
using namespace std;
#define F first
#define S second
#define vi vector<int>
#define vll vector<long long>
#define vld vector<long double>
#define vvi vector<vector<int>>
#define vs vector<string>
#define vb vector<bool>
#define all(v) v.begin(),v.end()
#define ump unordered_map
#define dbg(x) cout<<#x<<" : "<<x
using ll = long long;
using ld = long double;
using ull = unsigned long long;
#define pii pair<int,int>
#define pll pair<ll,ll>
#define vpll vector<pll>
#define vvpll vector<vector<pll>>
#define endl "\n"
//#define Onlaptop

const int N = 100001;  // limit for array size
const long long int M =1000000007;
ll n;  // array size
ll t[2 * N];

void build() {  // build the tree
  for (ll i = n - 1; i > 0; --i)
	   t[i] = (t[i<<1] + t[i<<1|1])%M;
}

void modify(ll p) {  // set value at position p
	p+=n;
	t[p] = (t[p]*2)%M;
  for (; p > 1; p >>= 1){
	   t[p>>1] = (t[p] + t[p^1])%M;
  }
}

ll segment_tree_query(ll l, ll r)
{  // sum on llerval [l, r)
  ll res = 0;
  for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
    if (l&1) res += t[l++] %= M;
    if (r&1) res += t[--r] %= M;
  }
  return res;
}

ll l;
vector<vector<ll>> adj;

ll timer;
vector<ll> tin, tout;
vector<vector<ll>> up;

void dfs_lca(ll v, ll p)
{
    tin[v] = ++timer;
    up[v][0] = p;
    for (ll i = 1; i <= l; ++i)
        up[v][i] = up[up[v][i-1]][i-1];

    for (ll u : adj[v]) {
        if (u != p){
            dfs_lca(u, v);
			//level[u]=level[v]+1;
		}
    }

    tout[v] = ++timer;
}

bool is_ancestor(ll u, ll v)
{
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

ll lca(ll u, ll v)
{
    if (is_ancestor(u, v))
        return u;
    if (is_ancestor(v, u))
        return v;
    for (ll i = l; i >= 0; --i) {
        if (!is_ancestor(up[u][i], v))
            u = up[u][i];
    }
    return up[u][0];
}

void preprocess(ll root=0) {
    tin.resize(n);
    tout.resize(n);
    timer = 0;
    l = ceil(log2(n));
    up.assign(n, vector<ll>(l + 1));
    dfs_lca(root, root);
}

vector<ll> parent, depth, heavy, head, pos;
ll cur_pos;

ll dfs_hld(ll v, vector<vector<ll>> const& adj) {
    ll size = 1;
    ll max_c_size = 0;
    for (ll c : adj[v]) {
        if (c != parent[v]) {
            parent[c] = v, depth[c] = depth[v] + 1;
            ll c_size = dfs_hld(c, adj);
            size += c_size;
            if (c_size > max_c_size)
                max_c_size = c_size, heavy[v] = c;
        }
    }
    return size;
}

void decompose(ll v, ll h, vector<vector<ll>> const& adj) {
    head[v] = h, pos[v] = cur_pos++;
    if (heavy[v] != -1)
        decompose(heavy[v], h, adj);
    for (ll c : adj[v]) {
        if (c != parent[v] && c != heavy[v])
            decompose(c, c, adj);
    }
}

void init() {
    parent = vector<ll>(n);
    depth = vector<ll>(n);
    heavy = vector<ll>(n, -1);
    head = vector<ll>(n);
    pos = vector<ll>(n);
    cur_pos = 0;

    dfs_hld(0, adj);
    decompose(0, 0, adj);
}

void add(ll& a,ll b){
	a=(a+b)%1000000007;
}

ll query(ll a, ll b) {
    ll res = 0;
    for (; head[a] != head[b]; b = parent[head[b]]) {
        if (depth[head[a]] > depth[head[b]])
            swap(a, b);
        ll cur_heavy_path_max = segment_tree_query(pos[head[b]], pos[b]+1);
        add(res, cur_heavy_path_max);
    }
    if (depth[a] > depth[b])
        swap(a, b);
    ll last_heavy_path_max = segment_tree_query(pos[a], pos[b]+1);
    add(res, last_heavy_path_max);
    return res;
}

struct edge{
	ll u,v,wgt;
	edge(ll uu,ll vv,ll wgtt){
		u=uu;
		v=vv;
		wgt=wgtt;
	}
};

vector<ll> level,val;
vector<vector<edge>> grh;

void dfs(ll v,ll p=-1,ll vvv=0){
	val[v]=vvv;
	for(auto i:grh[v]){
		if(i.v!=p){
			dfs(i.v,v,i.wgt);
			adj[v].push_back(i.v);
		}
	}
}

void solve()
{
	ll q;
	cin>>n>>q;
	//cout << "whw" << endl;
	grh.assign(n,vector<edge>());
	adj.assign(n,vector<ll>());
	level.resize(n);
	val.resize(n);
	
	//cerr << n << endl;
	for(int i=0;i<n-1;i++){
		ll a,b,wgt;
		cin>>a>>b>>wgt;
		//cout << a << b << wgt << endl;
		grh[a].push_back(edge(a,b,wgt));
		grh[b].push_back(edge(b,a,wgt));
	}
	
	dfs(0);

	preprocess();
	init();

	for(int i=0;i<n;i++){
		t[pos[i]+n]=val[i];
	}
	
	build();

	while(q--){
		int a,b;
		cin>>a;
		if(a==0){
			cin>>b;
			for(auto i:adj[b])
				modify(pos[i]);
			if(b!=0)
				modify(pos[b]);
		}
		else{
			cin>>a>>b;
			//cout << query(a,b) << endl;
			ll ans = (query(a,b)-t[n+pos[lca(a,b)]]+M)%M;
			cout << ans << endl;
		}
	}
	
}

signed main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
	ll t;
	//cin>>t;
	t=1;
	while(t--){
		solve();
	}
	return 0;
}
