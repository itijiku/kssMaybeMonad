#pragma once

#include    <assert.h>

namespace kks
{

// �A�T�[�g��`
#define        ASSERT        assert

//-----------------------------------------------------
// �N���XID�Ǘ���`
    using CLASS_ID = const char*;
#define        CLASS_HASH(x)        static_cast<kks::CLASS_ID>(x)
#define        _kksCLASS            public: static kks::CLASS_ID ClassID(void) { return CLASS_HASH(__FUNCTION__); }
#define        GetClassName()       ClassID()

// �N���XID���쐬����
// ���N���X�̐ÓI���\�b�h ClassID() ���ĂׂȂ����Ɏg��
#define        MAKE_CLASS_ID(x)    CLASS_HASH(#x "::ClassID")

//-----------------------------------------------------
// �x�[�X�N���X
    class Base
    {
        _kksCLASS
    public:
        virtual CLASS_ID GetClassID(void) const { return ClassID(); }
        virtual bool Initialize(void) = 0; // ������
        virtual void Destroy(void) = 0;    // �j��
        virtual ~Base() {}
    };

// kksBaseClass �N���X���p�����Ă��̃}�N�����N���X�錾�̐擪�ōs��
#define kksCLASS        _kksCLASS kks::CLASS_ID GetClassID(void) const override { return ClassID(); }

// kksCLASS �̕W���������s��
#define    kksCLASS_defaultImplement                   \
public:                                                \
    bool Initialize(void) override { return true; }    \
    void Destroy(void) override {}                     \
private:

}