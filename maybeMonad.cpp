// maybeMonad.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "KKSmonad.h"

//-----------------------------------------------------
// maybe の状態出力
template<class T_n>
void print(kks::Maybe<T_n>& m)
{
    if (m) {
        //printf("%s -> %d\n", m.ClassID(), *m);
        printf("%d\n", *m);
    }
    else {
        //printf("%s -> %d\n", m.ClassID(), *m);
        printf("Nothing\n");
    }
}

//-----------------------------------------------------
// 値のデクリメント用ファンクタ
template<class T_n>
class MF_Decrement
    : public kks::Functor<T_n>
{
    maybeMonadFUNCTOR(T_n)

public:
    Return Proc(T_n a) {
        return MaybeOrNothing(--a >= 0, a);
    }
};

//-----------------------------------------------------
// 値の出力用ファンクタ
template<class T_n>
class MF_Print
    : public kks::Functor<T_n>
{
    maybeMonadFUNCTOR(T_n)

public:
    Return Proc(T_n a) {
        printf("%d\n", a);
        return a;
    }
};

//-----------------------------------------------------
int main()
{
    kks::Maybe<int>    x(64);
    kks::Maybe<int>    y;
    kks::Maybe<int>    z(32);

    x = kks::Nothing();
    x = 128;

    // maybe x に対して x の値を出力してから偽にする処理をバインドする
    kks::Bind(
        x,
        MAYBE_FUNC(int, a) {
            printf("bind x : %d to Nothing.\n", a);
            return kks::Nothing();
        }
    );

    // maybe y に対して y の値を出力する処理をバインドする
    kks::Bind(
        y,
        MAYBE_FUNC(int, a) {
            printf("bind y : %d\n", a);
            return a; 
        }
    );

    print(x);
    print(y);
    print(z);

    kks::maybeMonad<int>    m;
    kks::maybeMonad<int>    n;

    // 値のデクリメント用ラムダ式
    auto declement = MAYBE_FUNC(int, a) {
        printf("lambda declement : %d\n", --a);
        return kks::MaybeOrNothing(a >= 0, a);
    };

    MF_Decrement<int>   dec;
    MF_Print<int>       intPrint;

    puts("maybe バインド:");
    x = 10;

    // 出力ファンクタをバインド
    kks::Bind(x, intPrint.Functor());

    // 出力ラムダ式をバインド
    x = kks::Bind(x, intPrint.Functor())
        ? kks::Bind(x, declement) : kks::Nothing();
    
    // 出力ファンクタをバインド
    x = Bind(x, intPrint.Functor())
        ? kks::Bind(x, dec.Functor()) : kks::Nothing();

    puts("普通のmeybeモナド:");
    // ファンクタやラムダ式を受け取って内部でバインドを呼んで評価する
    n = 4;
    n >>= declement;
    n >>= intPrint.Functor();
    n >>= dec.Functor();
    n >>= intPrint.Functor();
    n >>= declement;
    n >>= intPrint.Functor();
    n >>= dec.Functor();
    n >>= intPrint.Functor();

    ((n >>= dec.Functor()) >>= intPrint.Functor()) >>= dec.Functor();
    
    ((n >>= dec.Functor())
        >>= intPrint.Functor())
        >>= intPrint.Functor();

    puts("バインドストリーム:");
    // ファンクタやラムダ式を受け取って内部でバインドを呼んで評価する
    // ※偽であれば、処理結果を取り下げる
    n = 6;
    n 
        >> intPrint.Functor()
        >> dec.Functor() >> intPrint.Functor()
        >> declement >> intPrint.Functor()
        >> dec.Functor() >> intPrint.Functor()
        >> dec.Functor() >> intPrint.Functor()
        >> dec.Functor() >> intPrint.Functor()
        >> declement >> intPrint.Functor()
        ;

    // 真なら値を出力する
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

