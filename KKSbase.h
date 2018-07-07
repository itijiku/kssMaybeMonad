#pragma once

#include    <assert.h>

namespace kks
{

// アサート定義
#define        ASSERT        assert

//-----------------------------------------------------
// クラスID管理定義
    using CLASS_ID = const char*;
#define        CLASS_HASH(x)        static_cast<kks::CLASS_ID>(x)
#define        _kksCLASS            public: static kks::CLASS_ID ClassID(void) { return CLASS_HASH(__FUNCTION__); }
#define        GetClassName()       ClassID()

// クラスIDを作成する
// ※クラスの静的メソッド ClassID() を呼べない時に使う
#define        MAKE_CLASS_ID(x)    CLASS_HASH(#x "::ClassID")

//-----------------------------------------------------
// ベースクラス
    class Base
    {
        _kksCLASS
    public:
        virtual CLASS_ID GetClassID(void) const { return ClassID(); }
        virtual bool Initialize(void) = 0; // 初期化
        virtual void Destroy(void) = 0;    // 破棄
        virtual ~Base() {}
    };

// kksBaseClass クラスを継承してこのマクロをクラス宣言の先頭で行う
#define kksCLASS        _kksCLASS kks::CLASS_ID GetClassID(void) const override { return ClassID(); }

// kksCLASS の標準実装を行う
#define    kksCLASS_defaultImplement                   \
public:                                                \
    bool Initialize(void) override { return true; }    \
    void Destroy(void) override {}                     \
private:

}