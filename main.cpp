#include <iostream>
#include <fstream>
#include <set>
#include <unordered_set>
#include <string>
#include <string.h>
#include <ctype.h>
#include <algorithm>
using namespace std;

#define ll long long
#define ipair pair<ll, int>

FILE *fin;
int in_loc;
char in_buff[4096];

void read_init(const char *nume)
{
    fin=fopen(nume, "r");
    in_loc=4095;
}

char read_ch()
{
    ++in_loc;
    if(in_loc==4096)
    {
        in_loc=0;
        fread(in_buff, 1, 4096, fin);
    }
    return in_buff[in_loc];
}

int read_u32()
{
    int u32=0;
    char c;
    while(!isdigit(c=read_ch()) && c!='-');
    int sgn=1;
    if(c=='-')
        sgn=-1;
    else
        u32=c-'0';
    while(isdigit(c=read_ch()))
        u32=u32*10+c-'0';
    return u32*sgn;
}

ll read_u64()
{
    ll u64=0;
    char c;
    while(!isdigit(c=read_ch()) && c!='-');
    int sgn=1;
    if(c=='-')
        sgn=-1;
    else
        u64=c-'0';
    while(isdigit(c=read_ch()))
        u64=u64*10+c-'0';
    return u64*sgn;
}

ofstream fout("sezon.out");

const int mxN=1e5+1;
int n, dep[mxN], p[mxN][17];
ipair v[mxN];
unordered_set<int> G[mxN];
bool vis[mxN];

void dfs(int u)
{
    vis[u]=1;
    for(int v : G[u])
        if(!vis[v])
        {
            p[v][0]=u;
            dep[v]=dep[u]+1;
            dfs(v);
        }
}

void read()
{
    n=read_u32();
    for(int i=1; i<=n; i++)
    {
        v[i].first=read_u64();
        v[i].second=i;
        G[i].clear();
        vis[i]=0;
        p[i][0]=0;
        dep[i]=0;
    }
    for(int i=1; i<n; i++)
    {
        int a, b;
        a=read_u32();
        b=read_u32();
        G[a].insert(b);
        G[b].insert(a);
    }
    sort(v+1, v+n+1);
    dfs(1);
    for(int j=1; j<17; j++)
        for(int i=1; i<=n; i++)
            p[i][j]=p[p[i][j-1]][j-1];
}

int ancestor(int u, int k)
{
    for(int i=16; i>=0; i--)
        if((k&(1<<i)))
            u=p[u][i];
    return u;
}

int LCA(int a, int b)
{
    if(dep[a]>dep[b])
        a=ancestor(a, dep[a]-dep[b]);
    else
        b=ancestor(b, dep[b]-dep[a]);
    if(a==b)
        return a;
    for(int i=16; i>=0; i--)
        if(p[a][i]!=p[b][i])
        {
            a=p[a][i];
            b=p[b][i];
        }
    return p[a][0];
}

int distance(int a, int b)
{
    return dep[a]+dep[b]-2*dep[LCA(a, b)];
}

bool solve()
{
    if(v[n-1].first+1!=v[n].first)
        return false;
    for(int i=1; i<n-1; i++)
    {
        int u=v[i].second, nxt=v[i+1].second;
        if(G[u].size()>1)
            return false;
        int d=distance(u, nxt);
        if(v[i].first+d>v[i+1].first)
            return false;
        if((v[i].first+d)%2!=v[i+1].first%2)
            return false;
        int pu=*G[u].begin();
        G[u].clear();
        G[pu].erase(u);
    }
    return true;
}

int main()
{
    read_init("sezon.in");
    int t=read_u32();
    string ans;
    for(int i=0; i<t; i++)
    {
        read();
        if(solve())
            ans+='1';
        else
            ans+='0';
    }
    fout << ans;
    return 0;
}
