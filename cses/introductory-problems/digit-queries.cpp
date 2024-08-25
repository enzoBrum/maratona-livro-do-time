#include <iostream>
#include <algorithm>
#include <array>
#include<vector>
#include <cmath>
#include <tuple>
 
typedef long long ll;
typedef uint64_t ull;
typedef std::vector<int> vi;
typedef std::vector<ll> vll;
typedef std::vector<ull> vull;
 
#define sz(x) (int (x.size()))
#define fast_io() std::ios::sync_with_stdio(0); std::cin.tie(0);
 
std::tuple<ull, ull, ull> find_min_pos(ull n)
{
    ull pos = 0, num = 1, nd = 1;
    while ( true ) {
        pos += ( 9ll * nd * (ll) pow( 10, nd-1) );
        if ( pos > n )
            break;
        nd++;
    }
    
    
    num = pow( 10, nd-1 );
    return std::make_tuple( num, nd,  pos - ( 9ll * nd * (ll) pow( 10, nd-1) ) );
}
 
 
 
 
int main() {
    fast_io();
    int q;
    std::cin >> q;
 
    while (q--)
    {
        ull k;
        std::cin >> k;
        k--; 
        auto[ num, nd, pos ] = find_min_pos(k);
 
        ull diff = k - pos;
        ull mult = diff / nd; //integer division, it isnt redundant
        ull left_pos = pos + mult * nd;
 
 
        num = num + mult;
        
        std::string val = std::to_string(num);
        std::cout << val[k - left_pos] << '\n';
    }
    
}
