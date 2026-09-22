#include<bits/stdc++.h>
using namespace std;

namespace ly
{
    namespace IO
    {
        #define SIZE (1<<20)
        char in[SIZE], out[SIZE], *p1 = in, *p2 = in, *p3 = out;
        
        #define getchar() (p1 == p2 && (p2 = (p1 = in) + fread(in, 1, SIZE, stdin), p1 == p2) ? EOF : *p1++)
        #define flush() (fwrite(out, 1, p3 - out, stdout), p3 = out)
        #define putchar(ch) ((p3 == out + SIZE) ? (flush(), *p3++ = (ch)) : (*p3++ = (ch)))
        
        class Flush { public: ~Flush() { flush(); } } _;

        inline bool isBlank(char ch) {
            return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
        }

        template<typename type>
        inline typename enable_if<is_integral<type>::value, void>::type read(type &x)
        {
            x = 0; bool flag(0); char ch = getchar();
            while (!isdigit(ch) && ch != EOF) flag ^= (ch == '-'), ch = getchar();
            while (isdigit(ch)) x = (x << 1) + (x << 3) + (ch ^ 48), ch = getchar();
            if (flag) x = -x;
        }

        inline void read(string &s)
        {
            s.clear();
            char ch = getchar();
            while (ch != EOF && isBlank(ch)) ch = getchar();
            while (ch != EOF && !isBlank(ch)) {
                s += ch;
                ch = getchar();
            }
        }

        template<typename type>
        inline typename enable_if<is_integral<type>::value, void>::type write(type x, bool flag = 1)
        {
            if (x < 0) x = -x, putchar('-');
            static short Stack[50]; int top = 0;
            do Stack[++top] = x % 10, x /= 10; while (x);
            while (top) putchar(Stack[top--] | 48);
            flag ? putchar('\n') : putchar(' ');
        }

        inline void write(const string &s, bool flag = 1)
        {
            for (char ch : s) putchar(ch);
            flag ? putchar('\n') : putchar(' ');
        }

        #undef SIZE
        #undef getchar
        #undef putchar
        #undef flush
    }
}using namespace ly::IO;