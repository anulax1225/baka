#include <bakatools.h>
#include <math.h>
#include <stdlib.h>

int random(int max, int min = 0)
{
    int n = rand();
    while (n > max || n < min) n = rand();
    return n;
}

std::vector<int> prime_range(int max, int min = 2) 
{
    //Sieve of Eratosthenes algo from https://www.baeldung.com/cs/prime-number-algorithms
    std::vector<int> primes;
    std::vector<bool> primes_check(max);
    std::fill(primes_check.begin(), primes_check.end(), true);

    for (int i = 2; i < sqrt(max); i++)
        if (primes_check[i])
        {
            int j = i * i;
            while (j <= max)
            {
                primes_check[j] = false;
                j += i;
            }
        }

    for (int i = min; i < primes_check.size(); i++) 
        if (primes_check[i]) primes.push_back(i);

    return primes;
}

int gcd(int a, int b)
{
    if (b == 0) return a;
    else return gcd(b, a % b);
}

void rsa() 
{
    auto primes = prime_range(100);

    //int p = primes[random(primes.size())];
    //int q = primes[random(primes.size())];
    int p = 3;
    int q = 11;
    int n = p * q;
    int phi = (p - 1) * (q - 1);

    int e = 2;
    while(e < phi)
    {
        if(gcd(e, phi) == 1) break;
        e += 1;
    }
    float d = (e^-1) % phi;
    BK_INFO("n:{0} phi:{1} e:{2} d:{3}", n, phi, e, d);

    int msg = 11;
    float C = pow(msg, e);
    C = fmod(C, n);
    BK_INFO(C);
    float M = pow(C,d);
    M = fmod(M, n);
    BK_INFO(M);
}

int main()
{
    Bk::Log::init("Bakacypher");
    rsa();
    return 0;
}