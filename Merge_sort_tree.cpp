struct mergesortTree {
	vector<vector<ll> > mTree;
	vector<ll> vals;
	mergesortTree(ll n, vector<ll>& values)
	{
		vals = values;
		mTree.assign(n * 4, vector<ll>());
	}

	void build(ll l, ll r, ll id)
	{
		if (l == r)
		{
			mTree[id].push_back(vals[l]);
			return;
		}
		ll mid = (l + r) / 2;
		build(l, mid, id * 2 + 1);
		build(mid + 1, r, id * 2 + 2);
		mTree[id].resize(mTree[id * 2 + 1].size() + mTree[id * 2 + 2].size());
		merge(mTree[id * 2 + 1].begin(), mTree[id * 2 + 1].end(),
			mTree[id * 2 + 2].begin(), mTree[id * 2 + 2].end(),
			mTree[id].begin());
	}

	ll range_query(ll l, ll r, ll id, ll ql, ll qr, ll num)
	{
		if (l > r || l > qr || r < ql)return 0;
		if (l == ql && r == qr)
		{
			auto it = upper_bound(mTree[id].begin(), mTree[id].end(), num);
			return ll(it - mTree[id].begin());
		}
		ll mid = (l + r) / 2;
		return range_query(l, mid, id * 2 + 1, ql, min(mid, qr), num) +
			range_query(mid + 1, r, id * 2 + 2, max(mid + 1, ql), qr, num);
	}
};
