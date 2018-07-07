// maybeMonad.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "KKSmonad.h"


template<class T>
void print(kks::Maybe<T>& m)
{
    if (m) {
        printf("%s -> %d\n", m.ClassID(), m.Get());
    }
    else {
        printf("%s -> Nothing\n", m.ClassID());
    }
}

template<class T_n>
class MF_Decrement
    : public kks::Functor<T_n>
{
    kksCLASS
        kksCLASS_defaultImplement
    
    maybeMonadFUNCTOR(T_n)

public:
    kks::Maybe<T_n> Proc(T_n a) {
        //printf("Dec x : %d\n", a);
        return --a >= 0 ? kks::Maybe<T_n>(a) : kks::Nothing();
    }
};

template<class T_n>
class MF_IntPrint
    : public kks::Functor<T_n>
{
    kksCLASS
        kksCLASS_defaultImplement

    maybeMonadFUNCTOR(T_n)

public:
    kks::Maybe<T_n> Proc(T_n a) {
        printf("%d\n", a);
        return a;
    }
};


int main()
{
    kks::Maybe<int>    x(64);
    kks::Maybe<int>    y;

    x = kks::Nothing();
    x = 128;

    kks::Bind(x, [](int a) -> kks::Maybe<int>
        {
            printf("bind x : %d\n", a);
            return kks::Nothing();
        }
    );
    kks::Bind(y, [](int a) -> kks::Maybe<int>
        {
            printf("bind y : %d\n", a);
            return a; 
        }
    );

    print(x);
    print(y);

    kks::maybeMonad<int>    m;
    kks::maybeMonad<int>    n;

    auto declement = [](int a) -> kks::Maybe<int>
    {
        printf("lambda Dec x : %d\n", a);
        return --(a) >= 0 ? kks::Maybe<int>(a) : kks::Nothing();
    };

    MF_Decrement<int>        dec;
    MF_IntPrint<int>            intPrint;

    x = 10;
    kks::Bind(x, intPrint.Functor());
    x = x ? kks::Bind(x, dec.Functor()) : kks::Nothing();
    kks::Bind(x, intPrint.Functor());
    x = x ? kks::Bind(x, declement) : kks::Nothing();
    kks::Bind(x, intPrint.Functor());
    x = x ? kks::Bind(x, declement) : kks::Nothing();
    kks::Bind(x, intPrint.Functor());

    n = 3;

    n >>= dec.Functor();
    n >>= intPrint.Functor();
    n >>= dec.Functor();
    n >>= intPrint.Functor();
    n >>= dec.Functor();
    n >>= intPrint.Functor();
    n >>= dec.Functor();
    n >>= intPrint.Functor();

    n = ((n >>= dec.Functor()) >>= dec.Functor()) >>= dec.Functor();
    n = ((n >>= dec.Functor()) >>= dec.Functor()) >>= dec.Functor();
    n = ((n >>= dec.Functor()) >>= dec.Functor()) >>= dec.Functor();
    n = (n >>= dec.Functor()) >>= dec.Functor();

    // バインドストリーム
    n = 5;
    n 
        >> intPrint.Functor() >> dec.Functor()
        >> intPrint.Functor() >> dec.Functor()
        >> intPrint.Functor() >> dec.Functor()
        >> intPrint.Functor() >> dec.Functor()
        >> intPrint.Functor() >> dec.Functor()
        >> intPrint.Functor() >> dec.Functor()
        >> intPrint.Functor() >> dec.Functor()
        >> intPrint.Functor() >> dec.Functor()
        >> intPrint.Functor() >> dec.Functor()
        ;

    if (m) intPrint(*m);
    if (n) intPrint(*n);

    if (x)
    {
        printf("meybe x : %d\n", *x);
        if (y)
        {
            printf("meybe y : %d\n", *y);
        }
    }

    return 0;
}

