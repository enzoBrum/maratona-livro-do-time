#include <iostream>
#include <cmath>
#define ll long long
#define ull unsigned long long
 
int main() {
   /*
   2x + y = a
   2y + x = b
 
    y = a - 2x --> a - 2*(2a - b)/3
    x = b + 4x - 2a --> x = (2a - b )/3
 
    
 
   */
    
    
    std::ios::sync_with_stdio(false);
 
    int t;
    std::cin>> t;
 
    while (t--)
    {
        ll a,b;
        std::cin >> a >> b;
 
        ll max = (a > b) ? a : b;
        ll min = (a > b) ? b : a;
 
        //ll x = (2*max-min)/3; 
        //ll y = max - 2*(2*max-min)/3;
 
        
        if ( (2*max-min) % 3 == 0 && 2*(2*max-min)/3 <= max )
            std::cout << "YES\n";
        else
            std::cout << "NO\n";
    }
 
}
