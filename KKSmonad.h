#pragma once

#include "KKSbase.h"

namespace kks
{
    //-----------------------------------------------------
    // �N���X�������e���v���[�g�N���X
    template<class T>
    class ClassInitializer
        : public kks::Base
    {
        kksCLASS
            kksCLASS_defaultImplement

        T    mpVal;

    public:
        ClassInitializer(void)
            : mpVal() {}
        ClassInitializer(const T& a)
            : mpVal(a) {}
        ~ClassInitializer() {}

        const ClassInitializer<T>& operator=(const ClassInitializer<T>& obj) {
            mpVal = obj.mpVal;
            return *this;
        }
        const T& Get(void) const { return mpVal; }
        T& operator*() { return mpVal; }
    };

    //-----------------------------------------------------
    // �I�u�W�F�N�g�������Ă���Ȃ�^�ƂȂ�I�u�W�F�N�g�Ǘ��e���v���[�g�N���X
    // Ft: �U�N���X�iNothing ��W���Ƃ��Ē񋟂��Ă���j
    // T: �I�u�W�F�N�g�N���X�i����ɑΉ����Ă��Ȃ��I�u�W�F�N�g�́A�|�C���^�ŊǗ�����j
    class Nothing {};
    template<class Ft, class T>
    class Just
        : public ClassInitializer<T>
    {
        kksCLASS
            kksCLASS_defaultImplement

        bool        mbT;        // �^��Ԃ��ۂ�

    public:
        Just(void)
            : ClassInitializer<T>()
            , mbT(false) {}     // ����������ԁ��u�U�v
        Just(const Ft& f)
            : ClassInitializer<T>()
            , mbT(false) {}     // �U���󂯎���ď��������u�U�v
        Just(const T& a)
            : ClassInitializer<T>(a)
            , mbT(true) {}      // �����l���󂯎���ď��������u�^�v
        ~Just() {}

        operator bool() const { return mbT; }

        const Just<Ft, T>& operator=(const Just<Ft, T>& obj) {
            mbT = obj.mbT;
            ClassInitializer<T>::operator=(obj);
            return *this;
        }

        const T& Get(void) const {
            ASSERT(mbT);
            return ClassInitializer<T>::Get();
        }
        T& operator*() {
            ASSERT(mbT);
            return ClassInitializer<T>::operator*();
        }
    };

//-----------------------------------------------------
// class Maybe<T> ���p�����邽�߂̃R���X�g���N�^�錾
// ���Œ���K�v�Ȃ��̂��}�N���ł܂Ƃ߂Ē񋟂���
#define    maybeCONSTRUCTOR_BASE(x,t)            \
private:                                         \
    using SUPER = kks::Just<Nothing, t>;         \
public:                                          \
    x()                                          \
        : SUPER() {}                             \
    x(const T& a)                                \
        : SUPER(a) {}                            \
    x(const Nothing& a)                          \
        : SUPER(a) {}

#define    maybeCONSTRUCTOR(x)        maybeCONSTRUCTOR_BASE(x,T)

//-----------------------------------------------------
// Just<Nothing, T> �� Maybe<T> �Ƃ��Ē�`
    template<class T>
    class Maybe
        : public Just<Nothing, T>
    {
        kksCLASS
            kksCLASS_defaultImplement

    public:
        maybeCONSTRUCTOR(Maybe)
            ~Maybe() {}
    };

    template<class T>
    kks::Maybe<T> MaybeOrNothing(bool th, T a) {
        return th ? kks::Maybe<T>(a) : kks::Nothing();
    }

    //-----------------------------------------------------
    // Maybe �p�o�C���h�֐�
    template<class T, class F>
    const Maybe<T>& Bind(Maybe<T>& m, F f) {
        m = m ? f(*m) : Nothing();
        return m;
    }

#define MAYBE_FUNC(x,a)   [](x a) -> kks::Maybe<x>


    //-----------------------------------------------------
    // class Functor<T> ���p�����邽�߂̃R���X�g���N�^�錾
    // ���Œ���K�v�Ȃ��̂��}�N���ł܂Ƃ߂Ē񋟂���
#define        maybeMonadFUNCTOR(x)             \
    kksCLASS                                    \
    kksCLASS_defaultImplement                   \
private:                                        \
    using SUPER = kks::Functor<x>;              \
    using Return = kks::Maybe<x>;               \
    Return maybe(x a) {                         \
        return Return(a);                       \
    }                                           \
    Return MaybeOrNothing(bool th, x a) {       \
        return th ? maybe(a) : kks::Nothing();  \
    }                                           \
public:                                         \
    SUPER* Functor(void) {                      \
        return static_cast<SUPER*>(this);       \
    }

    //-----------------------------------------------------
    // maybeMonad �p�t�@���N�^�N���X
    template<class T>
    class Functor
        : public kks::Base
    {
        kksCLASS
            kksCLASS_defaultImplement

    public:
        Functor() {}
        ~Functor() {}
        virtual Maybe<T> Process(T a) = 0;
        Maybe<T> operator()(T& a) {
            return Process(a);
        }
    };

    //-----------------------------------------------------
    // Maybe �p�o�C���h�֐�
    template<class T>
    const Maybe<T>& Bind(Maybe<T>& m, Functor<T>* f = nullptr) {
        if (f == nullptr)
            m = m ? m : Nothing();
        else
            m = m ? (*f)(*m) : Nothing();
        return m;
    }


//-----------------------------------------------------
// class maybeMonad<T> ���p�����邽�߂̃R���X�g���N�^�錾
// ���Œ���K�v�Ȃ��̂��}�N���ł܂Ƃ߂Ē񋟂���
#define    maybeMonadCONSTRUCTOR_BASE(x,t)    \
private:                                      \
    using SUPER = kks::Maybe<t>;              \
public:                                       \
    x(MonadFunctor* fn = nullptr)             \
        : SUPER()                             \
        , mpOnFailedFunctor() {}              \
    x(const T& a)                             \
        : SUPER(a)                            \
        , mpOnFailedFunctor() {}              \
    x(const Nothing& a)                       \
        : SUPER(a)                            \
        , mpOnFailedFunctor() {}

#define    maybeMonadCONSTRUCTOR(x)     maybeMonadCONSTRUCTOR_BASE(x,T)

    // meybe���i�h�N���X
    template<class T>
    class maybeMonad
        : public Maybe<T>
    {
        kksCLASS
            kksCLASS_defaultImplement

    public:
        using MonadFunctor = kks::Functor<T>;

        //-----------------------------------------------------
        // �o�C���h�X�g���[���i>>�j�U�]�����p�W���t�@���N�^
        // ���U�ł���΁A�������ʂ���艺����
        template<class T>
        class MF_Reset
            : public MonadFunctor
        {
            maybeMonadFUNCTOR(T)

        public:
            Return Process(T a) override {
                return a;
            }
        };
        static MF_Reset<T> mMFOnFailed;

    private:
        MonadFunctor* mpOnFailedFunctor; // �o�C���h�X�g���[���i>>�j�U�]���������p�t�@���N�^

    public:
        maybeMonadCONSTRUCTOR(maybeMonad)
            ~maybeMonad() {}

        // �o�C���h�X�g���[���i>>�j�U�]���������p�t�@���N�^��o�^����
        maybeMonad<T>& BindOnFailedFunctor(MonadFunctor* f = mMFOnFailed.Functor()) {
            mpOnFailedFunctor = f;
            return *this;
        }

        const Maybe<T>& Bind(Functor<T>* f = nullptr) { return kks::Bind(*this, f); }
        maybeMonad<T>& operator>>=(MonadFunctor* f) { kks::Bind(*this, f); return *this; }
        maybeMonad<T>& operator>>(MonadFunctor* f) { return operator>><MonadFunctor*>(f); }
        template<class F> const Maybe<T>& Bind(F f)& { return kks::Bind(*this, f); }
        template<class F> maybeMonad<T>& operator>>=(F f) { kks::Bind(*this, f); return *this; }
        template<class F> maybeMonad<T>& operator>>(F f) {
            ASSERT(mpOnFailedFunctor);
            Maybe<T> bk(*this);
            if (!Bind(f))
                Maybe<T>::operator=((*mpOnFailedFunctor)(*bk));
            return *this;
        }
    };

    template<class T>
    maybeMonad<T>::MF_Reset<T> maybeMonad<T>::mMFOnFailed;

}