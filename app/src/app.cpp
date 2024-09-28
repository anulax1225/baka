#include <bakatools.h>
#include <math.h>
#include <stdlib.h>

long long int random(long long int max, long long int min = 0)
{
    long long int n = rand();
    while (n > max || n < min) n = rand();
    return n;
}

std::vector<long long int> prime_range(long long int max, long long int min = 2) 
{
    //Sieve of Eratosthenes algo from https://www.baeldung.com/cs/prime-number-algorithms
    std::vector<long long int> primes;
    std::vector<bool> primes_check(max);
    std::fill(primes_check.begin(), primes_check.end(), true);

    for (long long int i = 2; i < sqrt(max); i++)
        if (primes_check[i])
        {
            long long int j = i * i;
            while (j <= max)
            {
                primes_check[j] = false;
                j += i;
            }
        }

    for (long long int i = min; i < primes_check.size(); i++) 
        if (primes_check[i]) primes.push_back(i);

    return primes;
}

long long int gcd(long long int a, long long int b) 
{
    long long int temp;
    while (b != 0)
    {
        temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

long long int power(long long int val, long long int power)
{
    long long int res = val;
    for(long long int i = 1; i < power; i++) res *= val;
    return res;
}


void rsa() 
{ 
    auto primes = prime_range(100, 50);
    //long long int p = primes[random(primes.size())];
    //long long int q = primes[random(primes.size())];
    long long int p = 13;
    long long int q = 17;
    long long int n = p * q;
    long long int phi = (p - 1) * (q - 1);

    long long int e = 2;
    long long int k = 0;
    while(e < phi)
    {
        if (gcd(e, phi) == 1) k += 1;
        if (k >= 20) break;
        e += 1;
    }

    long long int d = 0;
    long long int j = 0;
    while (j < 2) 
    {
        d += 1;
        if (1 == (e * d) % phi) 
        {
            j += 1;
        }
    }
    BK_INFO("p:{4} q:{5} n:{0} phi:{1} e:{2} d:{3}", n, phi, e, d, p, q);

    long long int msg = 20;
    long long int C = power(msg, e) % n;
    BK_INFO("{0} {1}", C, power(msg, e));
    long long int M = C ^ d % n;
    BK_INFO(M);
}

void diffi()
{
    long long int a, b, g, p, ga, gb, gab, gba;
    a = 10030;
    b = 10294;
    g = 10927;
    p = prime_range(1000, 900)[3];
    ga = g^a % p;
    gb = g^b % p;
    gab = ga^b % p;
    gba = gb^a % p;
    BK_INFO("a{0} b{1} g{2} p{3} ga{4} gb{5} gab{6} gba{7}", a, b, g, p, ga, gb, gab, gba);

}

int main()
{
    Bk::Log::init("Bakacypher");
    diffi();
    return 0;
}